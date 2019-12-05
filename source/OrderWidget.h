#ifndef ORDERWIDGET_H
#define ORDERWIDGET_H
#include "nlohmann/json.hpp"
#include "redisclient.h"
#include "orderinformation.h"

#include <QWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QTimer>
#include <QStringList>
#include <QInputDialog>

namespace Ui {
class OrderWidget;
}

class OrderTableWidget : public QTableWidget{
    Q_OBJECT

public:
    explicit OrderTableWidget(QWidget *parent = nullptr);
    ~OrderTableWidget();
    void MockOrderPage();

signals:
    void SendCommand(QString);
    void SendMES_Data(nlohmann::json);
    void GetReply();
    void ReceivedNewSubscription(std::optional<QString> eParsed);
    void OrderPriorityChanged(int);

public slots:
    void on_MakeOrderTable(nlohmann::json);
    void on_TableCellDoubleClicked(int row, int column);
    void on_TableCellClicked(int row, int column);
    void on_SubscriptionMessage(QString eChannel, QString eMessage);

private:
    Ui::OrderWidget *ui;
    RedisClient *m_orderInformation = nullptr;
    QStringList m_headerColumns;
    OrderInformation *m_order = nullptr;
    int m_orderNumber = 0;
    QInputDialog *m_dialog = nullptr;
    QTimer *m_orderPagetimer = nullptr;
};

#endif // ORDERWIDGET_H
