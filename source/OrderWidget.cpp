#include "OrderWidget.h"
#include "ui_OrderWidget.h"

using Redistorium::Redis;
using Redistorium::Reply::ReplyElement;

OrderTableWidget::OrderTableWidget(QWidget *parent) : QTableWidget(parent), ui(new Ui::OrderWidget) {

    ui->setupUi(this);
    m_orderInformation = new RedisClient(this);
    m_orderPagetimer = new QTimer();
    m_headerColumns << "OrderID"
                    << "Priority"
                    << "CustomerName";
    setColumnCount(3);
    setSortingEnabled(false);
    setHorizontalHeaderLabels(m_headerColumns);
    verticalHeader()->setVisible(false);

    // connect(this, &MainWindow::SendCommand, m_RedisClient, &RedisClient::SendCommand);
    connect(m_orderInformation, &RedisClient::ReceivedJSONString, m_orderInformation, &RedisClient::on_ReadFromJsonString);
    connect(m_orderInformation, &RedisClient::SubscriptionMessage, this, &OrderTableWidget::on_SubscriptionMessage);
    connect(m_orderInformation, &RedisClient::ParsedJson, this, &OrderTableWidget::on_MakeOrderTable);
    connect(m_orderInformation, &RedisClient::SubscriptionMessage, [](QString eChannel, QString eMessage) {
        //        std::cout << "New subscription message on channel \"" << eChannel.toStdString() << "\": " << eMessage.toStdString() << std::endl;
    });
    connect(m_orderPagetimer, &QTimer::timeout, [&]() { OrderTableWidget::MockOrderPage(); }); // only for testing purposes
    connect(this, &OrderTableWidget::ReceivedNewSubscription, m_orderInformation, &RedisClient::on_ReadFromJsonString);
    connect(this, &OrderTableWidget::cellDoubleClicked, this, &OrderTableWidget::on_TableCellDoubleClicked);
    connect(this, &OrderTableWidget::cellClicked, this, &OrderTableWidget::on_TableCellClicked);
    m_orderPagetimer->start(1000);
    //    m_orderInformation->m_Redis->SUBSCRIBE("OrderPage");
    m_orderInformation->m_Redis->SUBSCRIBE("orderChannel");
    //    ReplyElement orderPage_data_received = m_orderInformation->m_Redis->GET("DataOrderPage");
    //    if (orderPage_data_received.GetBulkString().has_value()) {
    //        //        qDebug() << "This is what OrderPage_data looks like" << orderPage_data_received.GetBulkString().value();
    //        emit m_orderInformation->ReceivedJSONString(orderPage_data_received.GetBulkString());
    //    }

    ReplyElement orderData = m_orderInformation->m_Redis->LPOP("order");
    if (orderData.GetBulkString().has_value()) {
        qDebug() << "CLP data looks like this " << orderData.GetBulkString().value();
        emit m_orderInformation->ReceivedJSONString(orderData.GetBulkString());
    } else {
        std::cout << "CONTAINS NO DATA" << std::endl;
    }
}

void OrderTableWidget::MockOrderPage() {
    nlohmann::json OrderPage_data = m_orderInformation->make_json_orderpage();
    QString OrderPage_data_stringified = m_orderInformation->stringify_json(OrderPage_data);
    // TODO: SEGMENTATION FAULT here
    m_orderInformation->m_Redis->SET("DataOrderPage", OrderPage_data_stringified);
    m_orderInformation->m_Redis->PUBLISH("OrderPage", OrderPage_data_stringified);
}

void OrderTableWidget::on_SubscriptionMessage(QString eChannel, QString eMessage) {
    // qDebug() << "Received Message from subscribed channel " << eChannel << ": \n" << eMessage;
    std::optional<QString> systemMonitor_received = eMessage;
    emit ReceivedNewSubscription(systemMonitor_received);
}

void OrderTableWidget::on_MakeOrderTable(nlohmann::json eParsed) {
    qDebug() << "Now in MAKETABLE";
    setSortingEnabled(false);
    m_order = new OrderInformation(
        QString::fromStdString(std::string(eParsed["order"][0]["id"])), "0", QString::fromStdString(std::string(eParsed["order"][0]["customerName"])));
    setRowCount(eParsed["DataOrderPage"].size() + m_orderNumber);
    setItem(m_orderNumber, 0, new QTableWidgetItem(m_order->GetOrderID()));
    setItem(m_orderNumber, 1, new QTableWidgetItem(m_order->GetOrderPriority()));
    setItem(m_orderNumber, 2, new QTableWidgetItem(m_order->GetCustomerName()));
    ++m_orderNumber;
    // You can't sort while setting !
    horizontalHeader()->setSortIndicatorShown(true);
    horizontalHeader()->setSortIndicator(0, Qt::DescendingOrder);
    setSortingEnabled(true);
}

// void OrderTableWidget::on_MakeOrderTable(nlohmann::json eParsed) {
//    setSortingEnabled(false);
//    m_order = new OrderInformation(
//        QString::fromStdString(std::string(eParsed["DataOrderPage"][0]["orderID"])),
//        QString::fromStdString(std::string(eParsed["DataOrderPage"][0]["priority"])),
//        QString::fromStdString(std::string(eParsed["DataOrderPage"][0]["firstName"])),
//        QString::fromStdString(std::string(eParsed["DataOrderPage"][0]["lastName"])));
//    setRowCount(eParsed["DataOrderPage"].size() + m_orderNumber);
//    setItem(m_orderNumber, 0, new QTableWidgetItem(m_order->GetOrderID()));
//    setItem(m_orderNumber, 1, new QTableWidgetItem(m_order->GetOrderPriority()));
//    setItem(m_orderNumber, 2, new QTableWidgetItem(m_order->GetCustomerFirstName()));
//    setItem(m_orderNumber, 3, new QTableWidgetItem(m_order->GetCustomerLastName()));
//    ++m_orderNumber;
//    // You can't sort while setting...
//    horizontalHeader()->setSortIndicatorShown(true);
//    horizontalHeader()->setSortIndicator(0, Qt::DescendingOrder);
//    setSortingEnabled(true);
//}

void OrderTableWidget::on_TableCellDoubleClicked(int row, int column) {
    if (column != 1) {
        this->item(row, column)->setFlags(this->item(row, column)->flags() & ~Qt::ItemIsEditable);
    } else {
        m_dialog = new QInputDialog();
        std::cout << " Clicked Row: " << row << " Column: " << column << std::endl;
        int n = QInputDialog::getInt(this, "Alter OrderPriority", "Confirm by entering a different value");
        setItem(row, column, new QTableWidgetItem(QString::number(n)));
        // make the json
        nlohmann::json ChangedOrder_json;
        auto order_arr = nlohmann::json::array();
        nlohmann::json order_json{{"orderID", item(row, 0)->text().toStdString()},
                                  {"priority", std::to_string(n)},
                                  {"firstName", item(row, 2)->text().toStdString()},
                                  {"lastName", item(row, 3)->text().toStdString()}};
        order_arr.push_back(order_json);
        ChangedOrder_json["DataOrderPage"] = order_arr;
        // stringify it
        std::string tempstdstr = ChangedOrder_json.dump();
        QString tempqstring = tempstdstr.c_str();
        tempqstring.replace("\"", "\\\"");
        tempqstring = QString("\"" + tempqstring + "\"");
        // make a QStringList
        //        m_RedisClient->m_Redis->LPUSH()
        m_orderInformation->m_Redis->SET("AlteredDataOrderPage", tempqstring);
        m_orderInformation->m_Redis->PUBLISH("AltOrderPage", tempqstring);
    }
}

void OrderTableWidget::on_TableCellClicked(int row, int column) {
    if (column != 1) { this->item(row, column)->setFlags(this->item(row, column)->flags() & ~Qt::ItemIsEditable); }
}

OrderTableWidget::~OrderTableWidget() {
    delete ui;
    delete m_orderInformation;
    delete m_orderPagetimer;
    delete m_order;
}
