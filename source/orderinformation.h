#ifndef ORDERINFORMATION_H
#define ORDERINFORMATION_H
#include <QObject>
#include <QString>
#include <nlohmann/json.hpp>

class OrderInformation {
public:
    OrderInformation();
    OrderInformation(QString id, QString priority, QString customerName) : m_orderID{id}, m_orderPriority{priority}, m_customerName{customerName} {}

    void SetOrderID(QString order_id) {
        m_orderID = order_id;
    }
    void SetOrderPriority(QString order_priority) {
        m_orderPriority = order_priority;
    }
    void SetCustomerName(QString first_name) {
        m_customerName = first_name;
    }
    QString GetOrderID() {
        return m_orderID;
    }
    QString GetOrderPriority() {
        return m_orderPriority;
    }
    QString GetCustomerName() {
        return m_customerName;
    }

private:
    QString m_orderID;
    QString m_orderPriority;
    QString m_customerName;
};

#endif // ORDERINFORMATION_H
