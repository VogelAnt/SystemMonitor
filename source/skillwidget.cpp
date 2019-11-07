#include "skillwidget.h"
#include "ui_skillwidget.h"

SkillWidget::SkillWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SkillWidget)
{
    ui->setupUi(this);
}

SkillWidget::~SkillWidget()
{
    delete ui;
}
