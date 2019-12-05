#include "mainwindow.h"
#include "DeviceWidget.h"
#include "orderinformation.h"
#include "redisclient.h"
#include "ui_mainwindow.h"

// TODO: Write OrderWidget in there
// TODO: Write MyCustomTab in there, later on mebbe make a

using Redistorium::Redis;
using Redistorium::Reply::ReplyElement;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::MainWindow) {
    m_ui->setupUi(this);
    m_central = new QWidget(this);
    m_layout = new QHBoxLayout(m_central);
    m_orderTable = new OrderWidget(this);
    m_deviceTab = new DeviceTabWidget(this);
    m_layout->addWidget(m_orderTable);
    m_layout->addWidget(m_deviceTab);
    m_central->setLayout(m_layout);
    setCentralWidget(m_central);

//    m_RedisClient = new RedisClient(this);

//    // this part in OrderWidget
//    ui->tableWidget->setColumnCount(4);
//    ui->tableWidget->setSortingEnabled(false);
//    headerColumns << "OrderID"
//                  << "Priority"
//                  << "FirstName"
//                  << "LastName";
//    ui->tableWidget->setHorizontalHeaderLabels(headerColumns);
//    ui->tableWidget->verticalHeader()->setVisible(false);

    // this part in OrderWidget
//    connect(this, &MainWindow::SendCommand, m_RedisClient, &RedisClient::SendCommand);
//    connect(m_RedisClient, &RedisClient::ReceivedJSONString, m_RedisClient, &RedisClient::on_ReadFromJsonString);
//    connect(m_RedisClient, &RedisClient::SubscriptionMessage, this, &MainWindow::on_SubscriptionMessage);
//    connect(m_RedisClient, &RedisClient::ParsedJson, this, &MainWindow::on_MakeOrderTable);
//    connect(m_RedisClient, &RedisClient::SubscriptionMessage, [](QString eChannel, QString eMessage) {
//        // std::cout << "New subscription message on channel \"" << eChannel.toStdString() << "\": " << eMessage.toStdString() << std::endl;
//    });
//    connect(t_order_page, &QTimer::timeout, [&]() { MainWindow::MockOrderPage(); }); // only for testing purposes
//    connect(t_sds, &QTimer::timeout, [&]() { MainWindow::MockorderPageList(); }); // only for testing purposes
//    connect(this, &MainWindow::ReceivedNewSubscription, m_RedisClient, &RedisClient::on_ReadFromJsonString);
//    // this is where on:send Moduel state is actually connected !!
//    t_order_page->start(3000);

//    m_RedisClient->m_Redis->SUBSCRIBE("OrderPage");

//    ReplyElement orderPage_data_received = m_RedisClient->m_Redis->GET("DataOrderPage");
//    if (orderPage_data_received.GetBulkString().has_value()) {
//        qDebug() << "This is what OrderPage_data looks like" << orderPage_data_received.GetBulkString().value();
//        emit m_RedisClient->ReceivedJSONString(orderPage_data_received.GetBulkString());
//    }
}

MainWindow::~MainWindow() {
    delete m_ui;
    delete m_layout;
    delete m_deviceTab;
    delete m_central;
    delete m_orderTable;
}

//void MainWindow::MockorderPageList(){
//    nlohmann::json OrderPage_data = m_RedisClient->make_json_orderpage();
//    QStringList OrderPage_data_stringified;
//    OrderPage_data_stringified << m_RedisClient->stringify_json(OrderPage_data);
//    m_RedisClient->m_Redis->LPUSH("ALIST", OrderPage_data_stringified);
//    m_RedisClient->m_Redis->PUBLISH("LIST", OrderPage_data_stringified);
//}

//void MainWindow::MockOrderPage() {
//    nlohmann::json OrderPage_data = m_RedisClient->make_json_orderpage();
//    QString OrderPage_data_stringified = m_RedisClient->stringify_json(OrderPage_data);
//    // why are these two different Keys ?
//    m_RedisClient->m_Redis->SET("DataOrderPage", OrderPage_data_stringified);
//    m_RedisClient->m_Redis->PUBLISH("OrderPage", OrderPage_data_stringified);
//}


//void MainWindow::on_SubscriptionMessage(QString eChannel, QString eMessage) {
//    // qDebug() << "Received Message from subscribed channel " << eChannel << ": \n" << eMessage;
//    std::optional<QString> systemMonitor_received = eMessage;
//    emit ReceivedNewSubscription(systemMonitor_received);
//}

