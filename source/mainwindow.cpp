#include "mainwindow.h"
#include "OpcuaClient.h"
#include "dynamiccustomtab.h"
#include "orderinformation.h"
#include "redisclient.h"
#include "ui_mainwindow.h"

using Redistorium::Redis;
using Redistorium::Reply::ReplyElement;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    m_RedisClient = new RedisClient(this);

    // set up TabWidget Assembly, Labeling, SuperTrak move all this to a function outside ?
    ui->tabWidget->clear();
    m_TabStyle = new TabStyle_HorizontalText();
    ui->tabWidget->tabBar()->setStyle(m_TabStyle);

    DynamicCustomTab *AssemblyTab = new DynamicCustomTab();
    DynamicCustomTab *LabelingTab = new DynamicCustomTab();
    DynamicCustomTab *SuperTrakTab = new DynamicCustomTab();

    // set default display in Tab before anything starts !
    connect(m_OpcuaClient, &OpcuaClient::sendModuleState ,AssemblyTab, &DynamicCustomTab::on_sendModuleState);
    connect(m_OpcuaClient, &OpcuaClient::sendModuleState ,LabelingTab, &DynamicCustomTab::on_sendModuleState);
    connect(m_OpcuaClient, &OpcuaClient::sendModuleState ,SuperTrakTab, &DynamicCustomTab::on_sendModuleState);

    ui->tabWidget->addTab(AssemblyTab, "AssemblyModule");
    ui->tabWidget->addTab(LabelingTab, "LabelingModule");
    ui->tabWidget->addTab(SuperTrakTab, "SuperTrakTab");
    for (int i = 0 ;i<3;++i) {
        ui->tabWidget->tabBar()->setTabTextColor(i, QColor(Qt::yellow));
    }

    QTimer *style_timer = new QTimer();
    m_OpcuaClient = new OpcuaClient;
    connect(style_timer, &QTimer::timeout, m_OpcuaClient, &OpcuaClient::ReadModuleState);

    // set up Table Widget move this to an outside function ?
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setSortingEnabled(false);
    headerColumns << "OrderID"
                  << "Priority"
                  << "FirstName"
                  << "LastName";
    ui->tableWidget->setHorizontalHeaderLabels(headerColumns);
    ui->tableWidget->verticalHeader()->setVisible(false);
    QTimer *t_order_page = new QTimer();

    connect(this, &MainWindow::SendCommand, m_RedisClient, &RedisClient::SendCommand);
    connect(m_RedisClient, &RedisClient::ReceivedJSONString, m_RedisClient, &RedisClient::on_ReadFromJsonString);
    connect(m_RedisClient, &RedisClient::SubscriptionMessage, this, &MainWindow::on_SubscriptionMessage);
    connect(m_RedisClient, &RedisClient::ParsedJson, this, &MainWindow::on_MakeOrderTable);
    connect(m_RedisClient, &RedisClient::SubscriptionMessage, [](QString eChannel, QString eMessage) {
        // std::cout << "New subscription message on channel \"" << eChannel.toStdString() << "\": " << eMessage.toStdString() << std::endl;
    });
    connect(t_order_page, &QTimer::timeout, [&]() { MainWindow::MockOrderPage(); }); // only for testing purposes
    connect(this, &MainWindow::ReceivedNewSubscription, m_RedisClient, &RedisClient::on_ReadFromJsonString);
    connect(m_OpcuaClient, &OpcuaClient::sendModuleState, this, &MainWindow::on_sendModuleState);
    t_order_page->start(3000);
    style_timer->start(3000);

    m_RedisClient->m_Redis->SUBSCRIBE("OrderPage");

    ReplyElement orderPage_data_received = m_RedisClient->m_Redis->GET("DataOrderPage");
    if (orderPage_data_received.GetBulkString().has_value()) {
        qDebug() << "This is what OrderPage_data looks like" << orderPage_data_received.GetBulkString().value();
        emit m_RedisClient->ReceivedJSONString(orderPage_data_received.GetBulkString());
    }
}

void MainWindow::on_sendModuleState(std::map<std::string, std::string> ModuleSkillMap){
    int tab_index = 0;
    std::string default_node_value = "17";
    for (std::map<std::string, std::string>::iterator it = ModuleSkillMap.begin(); it != ModuleSkillMap.end(); ++it){
        for (int i = 0; i < default_node_value.length(); ++i){
            if((it->second[i] - '0') != (default_node_value[i] - '0')){
                std::cout << "setting red" << std::endl;
                ui->tabWidget->tabBar()->setTabTextColor(tab_index, QColor(Qt::red));
            }
            else {
            std::cout << "setting green " <<std:: endl;
            ui->tabWidget->tabBar()->setTabTextColor(tab_index, QColor(Qt::green));
            }
        }
    ++tab_index;
    }
}

void MainWindow::on_MakeOrderTable(nlohmann::json eParsed) {
    ui->tableWidget->setSortingEnabled(false);
    order = new OrderInformation(
        QString::fromStdString(std::string(eParsed["DataOrderPage"][0]["orderID"])),
        QString::fromStdString(std::string(eParsed["DataOrderPage"][0]["priority"])),
        QString::fromStdString(std::string(eParsed["DataOrderPage"][0]["firstName"])),
        QString::fromStdString(std::string(eParsed["DataOrderPage"][0]["lastName"])));
    //    order->SendOrderToRedis();
    ui->tableWidget->setRowCount(eParsed["DataOrderPage"].size() + m_OrderNumber);
    ui->tableWidget->setItem(m_OrderNumber, 0, new QTableWidgetItem(order->GetOrderID()));
    ui->tableWidget->setItem(m_OrderNumber, 1, new QTableWidgetItem(order->GetOrderPriority()));
    ui->tableWidget->setItem(m_OrderNumber, 2, new QTableWidgetItem(order->GetCustomerFirstName()));
    ui->tableWidget->setItem(m_OrderNumber, 3, new QTableWidgetItem(order->GetCustomerLastName()));
    ++m_OrderNumber;
    // You can't sort while still setting...
    ui->tableWidget->horizontalHeader()->setSortIndicatorShown(true);
    ui->tableWidget->horizontalHeader()->setSortIndicator(0, Qt::DescendingOrder);
    ui->tableWidget->setSortingEnabled(true);

}

MainWindow::~MainWindow() {
    delete ui;
    delete m_RedisClient;
    delete m_TabStyle;
    delete m_OpcuaClient;
    if (prioritybox != nullptr) delete prioritybox;
}


void MainWindow::on_SubscriptionMessage(QString eChannel, QString eMessage) {
    // qDebug() << "Received Message from subscribed channel " << eChannel << ": \n" << eMessage;
    std::optional<QString> systemMonitor_received = eMessage;
    emit ReceivedNewSubscription(systemMonitor_received);
}
void MainWindow::MockOrderPage() {
    nlohmann::json OrderPage_data = m_RedisClient->make_json_orderpage();
    QString OrderPage_data_stringified = m_RedisClient->stringify_json(OrderPage_data);
    // why are these two different Keys ?
    m_RedisClient->m_Redis->SET("DataOrderPage", OrderPage_data_stringified);
    m_RedisClient->m_Redis->PUBLISH("OrderPage", OrderPage_data_stringified);
}

void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{
    if (column != 1) {
        ui->tableWidget->item(row, column)->setFlags(ui->tableWidget->item(row,column)->flags() & ~Qt::ItemIsEditable);
    }
    else {
        dialog = new QInputDialog();
        std:: cout << " Clicked Row: " << row << "Column: " << column << std::endl;
        int n = QInputDialog::getInt(this, "Alter OrderPriority", "Confirm by entering a different value");
        ui->tableWidget->setItem(row, column, new QTableWidgetItem(QString::number(n)));
        // make the json
        nlohmann::json ChangedOrder_json;
        auto order_arr = nlohmann::json::array();
        nlohmann::json order_json{{"orderID", ui->tableWidget->item(row, 0)->text().toStdString()},
                                  {"priority", std::to_string(n)},
                                  {"firstName", ui->tableWidget->item(row, 2)->text().toStdString()},
                                  {"lastName", ui->tableWidget->item(row, 3)->text().toStdString()}};
        order_arr.push_back(order_json);
        ChangedOrder_json["DataOrderPage"] = order_arr;
        // stringify it
        std::string tempstdstr = ChangedOrder_json.dump();
        QString tempqstring = tempstdstr.c_str();
        tempqstring.replace("\"", "\\\"");
        tempqstring = QString("\"" + tempqstring + "\"");
        m_RedisClient->m_Redis->SET("AlteredDataOrderPage", tempqstring);
        m_RedisClient->m_Redis->PUBLISH("AltOrderPage", tempqstring);
    }
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    if (column != 1) {
        ui->tableWidget->item(row, column)->setFlags(ui->tableWidget->item(row,column)->flags() & ~Qt::ItemIsEditable);
    }
}
