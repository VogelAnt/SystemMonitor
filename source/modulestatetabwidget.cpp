#include "modulestatetabwidget.h"
#include "ui_modulestatetabwidget.h"

ModuleStateTabWidget::ModuleStateTabWidget(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::ModuleStateTabWidget)
{
    ui->setupUi(this);
}

ModuleStateTabWidget::~ModuleStateTabWidget()
{
    delete ui;
}
