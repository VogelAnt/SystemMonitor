#ifndef ORDERTABLE_H
#define ORDERTABLE_H

#include "redisclient.h"
#include <QStringList>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>

namespace Ui {
class OrderTable;
}

class OrderTable : public QWidget {
    Q_OBJECT

public:
    explicit OrderTable(QWidget *parent = nullptr);
    ~OrderTable();

private:
    Ui::OrderTable *ui;
    QTableWidget *tableWidget = new QTableWidget();
    QStringList tableWidgetHeader;
};

#endif // ORDERTABLE_H
