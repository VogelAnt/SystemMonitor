#include "OrderWidget.h"
#include "ui_OrderWidget.h"

using Redistorium::Redis;
using Redistorium::Reply::ReplyElement;

OrderWidget::OrderWidget(QWidget *parent) :
      QTableWidget(parent),
      ui(new Ui::OrderWidget) {
    RedisClient *m_orderInformation = new RedisClient(this);
    QTimer *t_order_page = new QTimer();

    ui->setupUi(this);
    // Redis Client here
    setColumnCount(4);
    setSortingEnabled(false);
    m_headerColumns << "OrderID"
                  << "Priority"
                  << "FirstName"
                  << "LastName";
    setHorizontalHeaderLabels(m_headerColumns);
    verticalHeader()->setVisible(false);

    connect(m_orderInformation, &RedisClient::ReceivedJSONString, m_orderInformation, &RedisClient::on_ReadFromJsonString);
    connect(m_orderInformation, &RedisClient::SubscriptionMessage, this, &OrderWidget::on_SubscriptionMessage);
    connect(m_orderInformation, &RedisClient::ParsedJson, this, &OrderWidget::on_MakeOrderTable);
    connect(m_orderInformation, &RedisClient::SubscriptionMessage, [](QString eChannel, QString eMessage) {
         std::cout << "New subscription message on channel \"" << eChannel.toStdString() << "\": " << eMessage.toStdString() << std::endl;});
    connect(t_order_page, &QTimer::timeout, [&]() { OrderWidget::MockOrderPage(); }); // only for testing purposes
    connect(this, &OrderWidget::ReceivedNewSubscription, m_orderInformation, &RedisClient::on_ReadFromJsonString);
    connect(this, &OrderWidget::cellDoubleClicked, this, &OrderWidget::on_TableCellDoubleClicked);
    connect(this, &OrderWidget::cellClicked, this, &OrderWidget::on_TableCellClicked);
    t_order_page->start(10000);
    m_orderInformation->m_Redis->SUBSCRIBE("OrderPage");

    // TODO: accessor for m_Redis ?
    ReplyElement orderPage_data_received = m_orderInformation->m_Redis->GET("DataOrderPage");
    if (orderPage_data_received.GetBulkString().has_value()) {
        qDebug() << "This is what OrderPage_data looks like" << orderPage_data_received.GetBulkString().value();
        emit m_orderInformation->ReceivedJSONString(orderPage_data_received.GetBulkString());
    }
}

void OrderWidget::MockOrderPage() {
    nlohmann::json OrderPage_data = m_orderInformation->make_json_orderpage();
    QString OrderPage_data_stringified = m_orderInformation->stringify_json(OrderPage_data);
    // TODO: SEGMENTATION FAULT here
    m_orderInformation->m_Redis->SET("DataOrderPage", OrderPage_data_stringified);
    m_orderInformation->m_Redis->PUBLISH("OrderPage", OrderPage_data_stringified);
}


void OrderWidget::on_SubscriptionMessage(QString eChannel, QString eMessage){
    // qDebug() << "Received Message from subscribed channel " << eChannel << ": \n" << eMessage;
    std::optional<QString> systemMonitor_received = eMessage;
    emit ReceivedNewSubscription(systemMonitor_received);
}

void OrderWidget::on_MakeOrderTable(nlohmann::json eParsed) {
    setSortingEnabled(false);
    m_Order = new OrderInformation(
        QString::fromStdString(std::string(eParsed["DataOrderPage"][0]["orderID"])),
        QString::fromStdString(std::string(eParsed["DataOrderPage"][0]["priority"])),
        QString::fromStdString(std::string(eParsed["DataOrderPage"][0]["firstName"])),
        QString::fromStdString(std::string(eParsed["DataOrderPage"][0]["lastName"])));
    //    order->SendOrderToRedis();
    setRowCount(eParsed["DataOrderPage"].size() + m_OrderNumber);
    setItem(m_OrderNumber, 0, new QTableWidgetItem(m_Order->GetOrderID()));
    setItem(m_OrderNumber, 1, new QTableWidgetItem(m_Order->GetOrderPriority()));
    setItem(m_OrderNumber, 2, new QTableWidgetItem(m_Order->GetCustomerFirstName()));
    setItem(m_OrderNumber, 3, new QTableWidgetItem(m_Order->GetCustomerLastName()));
    ++m_OrderNumber;
    // You can't sort while still setting...
    horizontalHeader()->setSortIndicatorShown(true);
    horizontalHeader()->setSortIndicator(0, Qt::DescendingOrder);
    setSortingEnabled(true);
}

void OrderWidget::on_TableCellDoubleClicked(int row, int column){
    if (column != 1) {
        this->item(row, column)->setFlags(this->item(row,column)->flags() & ~Qt::ItemIsEditable);
    }
    else {
        m_dialog = new QInputDialog();
        std:: cout << " Clicked Row: " << row << "Column: " << column << std::endl;
        int n = QInputDialog::getInt(this, "Alter OrderPriority", "Confirm by entering a different value");
        setItem(row, column, new QTableWidgetItem(QString::number(n)));
        // make the json
        nlohmann::json ChangedOrder_json;
        auto order_arr = nlohmann::json::array();
        nlohmann::json order_json{{"orderID", item(row, 0)->text().toStdString()},
                                  {"priority", std::to_string(n)},
                                  {"firstName",item(row, 2)->text().toStdString()},
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

void OrderWidget::on_TableCellClicked(int row, int column){
    if (column != 1) {
        this->item(row, column)->setFlags(this->item(row,column)->flags() & ~Qt::ItemIsEditable);
    }
}

OrderWidget::~OrderWidget()
{
    delete ui;
}

