#ifndef ORDERWIDGET_H
#define ORDERWIDGET_H
#include "nlohmann/json.hpp"
#include "redisclient.h"
#include "orderinformation.h"

#include <QWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QStringList>
#include <QInputDialog>

namespace Ui {
class OrderWidget;
}

class OrderWidget : public QTableWidget{
    Q_OBJECT

public:
    explicit OrderWidget(QWidget *parent = nullptr);
    ~OrderWidget();
    void MockOrderPage();

signals:
    void SendCommand(QString);
    void SendMES_Data(nlohmann::json);
    void GetReply();
    void ReceivedNewSubscription(std::optional<QString> eParsed);
    void OrderPriorityChanged(int);

public slots:
    void on_MakeOrderTable(nlohmann::json);

private slots:
    void on_TableCellDoubleClicked(int row, int column);
    void on_TableCellClicked(int row, int column);
    void on_SubscriptionMessage(QString eChannel, QString eMessage);

private:
    Ui::OrderWidget *ui;
    RedisClient *m_orderInformation = nullptr;
    QStringList m_headerColumns;
    OrderInformation *m_Order = nullptr;
    int m_OrderNumber = 0;
    QInputDialog *m_dialog = nullptr;
};

#endif // ORDERWIDGET_H
