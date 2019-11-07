#ifndef ORDERINFORMATION_H
#define ORDERINFORMATION_H
#include <QObject>
#include <QString>
#include <nlohmann/json.hpp>

class OrderInformation : public QObject {
    Q_OBJECT
public:
    OrderInformation();
    OrderInformation(QString id, QString priority, QString firstname, QString lastname)
        : m_order_id{id}, m_order_priority{priority}, m_customer_first_name{firstname}, m_customer_last_name{lastname} {}

    void SetOrderID(QString order_id) {
        m_order_id = order_id;
    }
    void SetOrderPriority(QString order_priority) {
        m_order_priority = order_priority;
    }
    void SetCustomerFirstName(QString first_name) {
        m_customer_first_name = first_name;
    }
    void SetCustomerLastName(QString last_name) {
        m_customer_last_name = last_name;
    }

    QString GetOrderID() {
        return m_order_id;
    }
    QString GetOrderPriority() {
        return m_order_priority;
    }
    QString GetCustomerFirstName() {
        return m_customer_first_name;
    }
    QString GetCustomerLastName() {
        return m_customer_last_name;
    }
    void SendOrderToRedis();

private:
    QString m_order_id;
    QString m_order_priority;
    QString m_customer_first_name;
    QString m_customer_last_name;
};

#endif // ORDERINFORMATION_H
