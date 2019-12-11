#ifndef ORDERWIDGET_H
#define ORDERWIDGET_H
#include "nlohmann/json.hpp"
#include "orderinformation.h"
#include "redisclient.h"

#include <QHeaderView>
#include <QInputDialog>
#include <QStringList>
#include <QTableWidget>
#include <QTimer>
#include <QWidget>

namespace Ui {
class OrderWidget;
}

class OrderTableWidget : public QTableWidget {
    Q_OBJECT

public:
    explicit OrderTableWidget(QWidget *parent = nullptr);
    ~OrderTableWidget();

signals:
    void SendCommand(QString);
    void GetReply();
    void ReceivedNewSubscription(QString eParsed);
    void OrderPriorityChanged(int);

public slots:
    void on_MakeOrderTable(nlohmann::json);
    void on_TableCellDoubleClicked(int row, int column);
    void on_TableCellClicked(int row, int column);
    void on_SubscriptionMessage(QString eChannel, QString eMessage);

private:
    Ui::OrderWidget *ui;
    QStringList m_headerColumns;
    RedisClient *m_orderInformation = nullptr;
    QTimer *m_orderPagetimer = nullptr;
    std::vector<OrderInformation> m_orderVector;
    QInputDialog *m_priorityDialog = nullptr;
};

#endif // ORDERWIDGET_H
