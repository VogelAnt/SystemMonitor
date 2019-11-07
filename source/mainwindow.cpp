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
    ui->tabWidget->clear();
    m_RedisClient = new RedisClient(this);

    // Tab Widget
    ui->tabWidget->setStyleSheet("QTabBar::tab { background: gray; color: white; padding: 10px; } "
                                 "QTabBar::tab:selected { background: red; } "
                                 "QTabWidget::pane { border: 0; } "
                                 "QWidget { background: lightgray; } ");

    m_TabStyle = new TabStyle_HorizontalText();
    ui->tabWidget->tabBar()->setStyle(m_TabStyle);

    // ui->tabWidget->tabBar()->setStyle(new TabStyle_HorizontalText);
    QWidget *tab1 = new QWidget();
    SkillWidget *skill1 = new SkillWidget(tab1);
    ui->tabWidget->addTab(tab1, "Tab1");
    ui->tabWidget->addTab(new QWidget, "Tab2");
    QTimer *style_timer = new QTimer();
    connect(style_timer, &QTimer::timeout, this, &MainWindow::on_ChangeColour);
    connect(style_timer, &QTimer::timeout, this, &MainWindow::on_DisplaySkillWidget);
    m_OpcuaClient = new OpcuaClient;
    connect(style_timer, &QTimer::timeout, m_OpcuaClient, &OpcuaClient::ReadModuleState);

    // Table Widget
    ui->tableWidget->setColumnCount(4);
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
        std::cout << "New subscription message on channel \"" << eChannel.toStdString() << "\": " << eMessage.toStdString() << std::endl;
    });
    connect(t_order_page, &QTimer::timeout, [&]() { MainWindow::test(); }); // only for testing purposes
    connect(this, &MainWindow::ReceivedNewSubscription, m_RedisClient, &RedisClient::on_ReadFromJsonString);

    t_order_page->start(800);
    style_timer->start(1000);

    m_RedisClient->m_Redis->SUBSCRIBE("OrderPage");

    ReplyElement orderPage_data_received = m_RedisClient->m_Redis->GET("DataOrderPage");
    if (orderPage_data_received.GetBulkString().has_value()) {
        qDebug() << "This is what OrderPage_data looks like" << orderPage_data_received.GetBulkString().value();
        emit m_RedisClient->ReceivedJSONString(orderPage_data_received.GetBulkString());
    }
}

void MainWindow::on_DisplaySkillWidget() {
    // SkillWidget *skill_widget = new SkillWidget();
}

void MainWindow::on_ChangeColour() {
    std::random_device rd; // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1, 3);

    switch (dis(gen)) {
    case 1:
        ui->tabWidget->setStyleSheet("QTabBar::tab { background: gray; color: white; padding: 10px; } "
                                     "QTabBar::tab:selected { background: green; } "
                                     "QTabWidget::pane { border: 0; } "
                                     "QWidget { background: lightgray; } ");
        break;
    case 2:
        ui->tabWidget->setStyleSheet("QTabBar::tab { background: gray; color: white; padding: 10px; } "
                                     "QTabBar::tab:selected { background: yellow; } "
                                     "QTabWidget::pane { border: 0; } "
                                     "QWidget { background: lightgray; } ");
        break;
    case 3:
        ui->tabWidget->setStyleSheet("QTabBar::tab { background: gray; color: white; padding: 10px; } "
                                     "QTabBar::tab:selected { background: red; } "
                                     "QTabWidget::pane { border: 0; } "
                                     "QWidget { background: lightgray; } ");
        break;
    default:
        ui->tabWidget->setStyleSheet("QTabBar::tab { background: gray; color: white; padding: 10px; } "
                                     "QTabBar::tab:selected { background: red; } "
                                     "QTabWidget::pane { border: 0; } "
                                     "QWidget { background: lightgray; } ");
    }
}

void MainWindow::test() {
    nlohmann::json OrderPage_data = m_RedisClient->make_json_orderpage();
    QString OrderPage_data_stringified = m_RedisClient->stringify_json(OrderPage_data);
    m_RedisClient->m_Redis->SET("DataOrderPage", OrderPage_data_stringified);
    m_RedisClient->m_Redis->PUBLISH("OrderPage", OrderPage_data_stringified);
}

void MainWindow::on_MakeOrderTable(nlohmann::json eParsed) {
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
}

MainWindow::~MainWindow() {
    delete ui;
    delete m_RedisClient;
    delete m_TabStyle;
    if (prioritybox != nullptr) delete prioritybox;
}

// select the row and column of the tableWidget to get the respective element
void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column) {
#pragma message "TODO: reconsider this design choice. Does it need to be a pop up message box,"
#pragma message "or should it just be a \"Set\" button right next to the text field,"
#pragma message "or should it be a slot of on_textfield_valueChanged or something similar?"
    if (prioritybox != nullptr) delete prioritybox;
    prioritybox = new QMessageBox();
    prioritybox->setIcon(QMessageBox::Warning);
    prioritybox->setText("Alter current Orderstatus");
    prioritybox->setInformativeText("Enter the desired priority status below");
    prioritybox->setStandardButtons(QMessageBox::Apply | QMessageBox::Cancel);
    std::cout << "Row:" << row << " Column: " << column << std::endl;
    prioritybox->show();
}

void MainWindow::on_SubscriptionMessage(QString eChannel, QString eMessage) {
    qDebug() << "Received Message from subscribed channel " << eChannel << ": \n" << eMessage;
    std::optional<QString> systemMonitor_received = eMessage;
    emit ReceivedNewSubscription(systemMonitor_received);
}
