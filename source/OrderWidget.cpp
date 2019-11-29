#include "OrderWidget.h"
#include "ui_OrderWidget.h"
// TODO: make this the table portion here
// TODO:
OrderWidget::OrderWidget(QWidget *parent) :
      QTableWidget(parent),
      ui(new Ui::OrderWidget)
{
    ui->setupUi(this);
}

OrderWidget::~OrderWidget()
{
    delete ui;
}
