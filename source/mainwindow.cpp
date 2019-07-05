#include "mainwindow.h"
#include "dynamiccustomtab.h"
#include "redisclient.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    m_RedisClient = new RedisClient(this);

    connect(this, &MainWindow::SendCommand, m_RedisClient, &RedisClient::SendCommand); // superfluous ?
    connect(this, &MainWindow::on_sendMES_data, m_RedisClient, &RedisClient::sendMESdata);
    connect(m_RedisClient, &RedisClient::sendMESdata, this, &MainWindow::on_sendMES_data);
    connect(m_RedisClient, &RedisClient::sendOrderPageData, this, &MainWindow::on_sendOrderPageData);

    ui->setupUi(this);
    ui->tabWidget->clear();
    // TODO: This should be set to length of OrderList JSON data
    ui->tableWidget->setRowCount(OrderList.size());
    ui->tableWidget->setColumnCount(2);
    m_TabStyle = new TabStyle_HorizontalText();
    ui->tabWidget->tabBar()->setStyle(m_TabStyle);

    headerColumns << "Orders"
                  << "Priority";
    ui->tableWidget->setHorizontalHeaderLabels(headerColumns);
    ui->tableWidget->verticalHeader()->setVisible(false);

    nlohmann::json MES_data = m_RedisClient->MES_json();
    nlohmann::json OrderPage_data = m_RedisClient->OrderPage_json();

    QString MES_data_stringified = m_RedisClient->stringify_json(MES_data);
    QString OrderPage_data_stringified = m_RedisClient->stringify_json(OrderPage_data);

    m_RedisClient->m_Redis->Set("MES_data", MES_data_stringified);
    m_RedisClient->m_Redis->Set("OrderPage_data", OrderPage_data_stringified);
    std::optional<QString> mes_data_received = m_RedisClient->m_Redis->Get("MES_data");
    std::optional<QString> orderPage_data_received = m_RedisClient->m_Redis->Get("OrderPage_data");

    if (mes_data_received.has_value()) {

        QString temp = mes_data_received.value();
        qDebug() << "HMMMMM" << temp;
        temp.replace("\\\"", "\"");
        nlohmann::json parsed = nlohmann::json::parse(temp.toStdString());
        if (parsed["MES_data"] != nullptr) { emit m_RedisClient->sendMESdata(parsed); }
    }
    if (orderPage_data_received.has_value()) {

        QString temp = orderPage_data_received.value();
        qDebug() << "HMMMMM" << temp;
        temp.replace("\\\"", "\"");
        nlohmann::json parsed = nlohmann::json::parse(temp.toStdString());
        if (parsed["OrderPage_data"] != nullptr) { emit m_RedisClient->sendOrderPageData(parsed); }
    }
}

// Read out the JSON file and put information in table from here
void MainWindow::on_sendMES_data(nlohmann::json eReply) {
    int i = 0;
    for (nlohmann::json::iterator it = eReply["MES_data"].begin(); it != eReply["MES_data"].end(); ++it) {
        DynamicCustomTab *newTab = new DynamicCustomTab();
        connect(this, &MainWindow::SendMES_Data, newTab, &DynamicCustomTab::on_SendMESData);
        std::string dummystring = eReply["MES_data"][i]["ModuleName"];
        QString qdummystring = QString::fromStdString(dummystring);
        qDebug() << qdummystring;
        ui->tabWidget->addTab(newTab, qdummystring);
        ++i;
    }
}

void MainWindow::on_sendOrderPageData(nlohmann::json eReply) {
    static int ordernumber = 0;
    ui->tableWidget->setRowCount(eReply["OrderPage_data"].size() + ordernumber);
    for (nlohmann::json::iterator it = eReply.begin(); it != eReply.end(); ++it) {
        //        std::string dummystring = eReply["OrderPage_data"][ordernumber][]
        ui->tableWidget->setItem(ordernumber, 0, new QTableWidgetItem("Order#" + QString::number(ordernumber)));
        //        ui->tableWidget->setItem(ordernumber)
        ++ordernumber;
    }
    // row count = length of std vector
}

MainWindow::~MainWindow() {
    delete ui;
    delete m_RedisClient;
    delete m_TabStyle;
    if (prioritybox != nullptr) delete prioritybox;
}

// ui->label_Reply->setText(eReply);

// Push button to SET MES / OrderPage values (JSON)
// Push button to GET values (parse JSON)

void MainWindow::on_SendCommand_clicked() {
    // redo this
    std::cout << "send message";

    std::string MES_message = R"delimeter(ModuleName {
                          "Skill1" : "error/warnings/ok",
                          "Skill2" : "error/warnings/ok",
                          "Skill3" : "error/warnings/ok",
                          "Skill4" : "error/warnings/ok",
                          "Skill5" : "error/warnings/ok"
                                                      })delimeter";
    std::cout << "send message";
    emit SendCommand(QString::fromStdString(MES_message));
}

void MainWindow::on_GetReply_clicked() {
    emit GetReply();
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
