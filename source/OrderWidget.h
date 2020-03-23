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
/**
 * @brief The OrderTableWidget class is a QTableWidget that displays parsed order
 * information as a table
 */
class OrderTableWidget : public QTableWidget {
    Q_OBJECT

public:
    /**
     * @brief OrderTableWidget
     * instantiates redis client and timer for polling of redis server
     * sets up header configuration and connects parsing related functions
     * between redis client and the table
     * @param parent
     * MainWindow
     */
    explicit OrderTableWidget(QWidget *parent = nullptr);

    ~OrderTableWidget();

signals:
    /**
     * @brief ReceivedNewSubscription
     * @param eParsed
     */
    void ReceivedNewSubscription(QString eParsed);

    /**
     * @brief OrderPriorityChanged
     * Emitted whenever the priority has been changed by double clicking the priority
     * column and confirming changes
     */
    void OrderPriorityChanged(int);

public slots:
    /**
     * @brief on_MakeOrderTable
     * Fills table columns with data from parsed order
     */
    void on_MakeOrderTable(nlohmann::json);
    /**
     * @brief on_TableCellDoubleClicked
     * @param row
     * @param column
     */
    void on_TableCellDoubleClicked(int row, int column);
    /**
     * @brief on_TableCellClicked
     * @param row
     * @param column
     */
    void on_TableCellClicked(int row, int column);
    /**
     * @brief on_SubscriptionMessage
     * converts received message to QString and checks if content is empty
     * @param eChannel
     * @param eMessage
     */
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
