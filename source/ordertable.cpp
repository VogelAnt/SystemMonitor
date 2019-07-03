#include "ordertable.h"
#include "ui_ordertable.h"

OrderTable::OrderTable(QWidget *parent) : QWidget(parent), ui(new Ui::OrderTable) {
    ui->setupUi(this);
    tableWidget = new QTableWidget();
    tableWidget->setRowCount(10);
    tableWidget->setColumnCount(2);

    /**
      set Headers of table
      **/
    tableWidgetHeader << "Order#"
                      << "Priority";
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setItem(0, 1, new QTableWidgetItem("Test"));
}

OrderTable::~OrderTable() {
    delete ui;
}
