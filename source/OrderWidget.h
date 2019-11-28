#ifndef ORDERWIDGET_H
#define ORDERWIDGET_H

#include <QWidget>
#include <QTableWidget>

namespace Ui {
class OrderWidget;
}

class OrderWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit OrderWidget(QWidget *parent = nullptr);
    ~OrderWidget();

private:
    Ui::OrderWidget *ui;
};

#endif // ORDERWIDGET_H
