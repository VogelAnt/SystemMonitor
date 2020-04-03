#include "OrderWidget.h"
#include "ui_OrderWidget.h"

using Redistorium::Redis;
using Redistorium::Reply::ReplyElement;
using Redistorium::Reply::ReplyType;

OrderTableWidget::OrderTableWidget(QWidget *parent) : QTableWidget(parent), ui(new Ui::OrderWidget) {
    ui->setupUi(this);
    m_orderInformation = new RedisClient(this);
    m_orderPagetimer = new QTimer();
    m_headerColumns << "OrderID"
                    << "CustomerName"
                    << "Priority"
                    << "age"
                    << "barbequeAnswer"
                    << "cookAnswer"
                    << "cupType"
                    << "email"
                    << "eventName"
                    << "firstName"
                    << "foodAnswer"
                    << "lastName"
                    << "orderID"
                    << "origin"
                    << "seedAmount"
                    << "seedTypes"
                    << "starSign"
                    << "userText"
                    << "creationTime"
                    << "uuid";
    setColumnCount(20);
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
    connect(this, &OrderTableWidget::ReceivedNewSubscription, m_orderInformation, &RedisClient::on_ReadFromJsonString);
    connect(this, &OrderTableWidget::cellDoubleClicked, this, &OrderTableWidget::on_TableCellDoubleClicked);
    connect(this, &OrderTableWidget::cellClicked, this, &OrderTableWidget::on_TableCellClicked);
    m_orderPagetimer->start(1000);
    m_orderInformation->m_Redis->SUBSCRIBE("orderChannel");
    ReplyElement initialOrderlistsize = m_orderInformation->m_Redis->LLEN("order");
    ReplyElement orderData = m_orderInformation->m_Redis->LRANGE("order", 0, initialOrderlistsize.Integer());
    if (orderData.Type == ReplyType::Array) {
        orderData.Print();
        for (auto &order : orderData.Array()) {
            try {
                order.Print();
                emit m_orderInformation->ReceivedJSONString(order.GetBulkString().value());
            } catch (std::exception e) { std::cout << __FILE__ << ":" << __LINE__ << ": " << e.what() << std::endl; }
        }
    }
}

void OrderTableWidget::on_SubscriptionMessage(QString eChannel, QString eMessage) {
    // qDebug() << "Received Message from subscribed channel " << eChannel << ": \n" << eMessage;
    std::optional<QString> systemMonitor_received = eMessage;
    if (systemMonitor_received.has_value()) { emit ReceivedNewSubscription(systemMonitor_received.value()); }
    // TODO: Make an exception here
}

void OrderTableWidget::on_MakeOrderTable(nlohmann::json eJsonArray) {
    std::cout << "Data received in table" << eJsonArray << std::endl;
    setSortingEnabled(false);
    OrderInformation order =
        OrderInformation(QString::fromStdString(std::string(eJsonArray["id"])), "0", QString::fromStdString(std::string(eJsonArray["customerName"])));
    // setRowCount(eJsonArray.size() + m_orderNumber)
    setRowCount(m_orderVector.size() + 1);
    setItem(m_orderVector.size(), 0, new QTableWidgetItem(order.GetOrderID()));
    setItem(m_orderVector.size(), 1, new QTableWidgetItem(order.GetCustomerName()));
    setItem(m_orderVector.size(), 2, new QTableWidgetItem(order.GetOrderPriority()));
    std::cout << item(m_orderVector.size(), 0) << std::endl;
    m_orderVector.push_back(order);
    // You can't sort while setting !
    horizontalHeader()->setSortIndicatorShown(true);
    horizontalHeader()->setSortIndicator(0, Qt::DescendingOrder);
    setSortingEnabled(true);
}

void OrderTableWidget::on_TableCellDoubleClicked(int row, int column) {
    if (column != 2) {
        this->item(row, column)->setFlags(this->item(row, column)->flags() & ~Qt::ItemIsEditable);
    } else {
        m_priorityDialog = new QInputDialog();
        std::cout << " Clicked Row: " << row << " Column: " << column << std::endl;
        int n = QInputDialog::getInt(this, "Alter OrderPriority", "Confirm by entering a different value");
        setItem(row, column, new QTableWidgetItem(QString::number(n)));
        // make the json
        nlohmann::json ChangedOrder_json;
        auto order_arr = nlohmann::json::array();
        nlohmann::json order_json{{"orderID", item(row, 0)->text().toStdString()},
                                  {"CustomerName", item(row, 1)->text().toStdString()},
                                  {"Priority", item(row, 2)->text().toStdString()}};
        order_arr.push_back(order_json);
        ChangedOrder_json["DataOrderPage"] = order_arr;
        // stringify it
        std::string tempstdstr = ChangedOrder_json.dump();
        QString tempqstring = tempstdstr.c_str();
        tempqstring.replace("\"", "\\\"");
        tempqstring = QString("\"" + tempqstring + "\"");
        m_orderInformation->m_Redis->SET("AlteredDataOrderPage", tempqstring);
        m_orderInformation->m_Redis->PUBLISH("AltOrderPage", tempqstring);
    }
}

void OrderTableWidget::on_TableCellClicked(int row, int column) {
    if (column != 1) {
        qDebug() << "Row: " << row << " Column:" << column;
        this->item(row, column)->setFlags(this->item(row, column)->flags() & ~Qt::ItemIsEditable);
    }
}

OrderTableWidget::~OrderTableWidget() {
    delete ui;
    delete m_orderInformation;
    delete m_orderPagetimer;
}
