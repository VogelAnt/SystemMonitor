#include "dynamiccustomtab.h"
#include "redisclient.h"
#include "ui_dynamiccustomtab.h"

DynamicCustomTab::DynamicCustomTab(QWidget *parent) : QWidget(parent), ui(new Ui::DynamicCustomTab) {
    ui->setupUi(this);
    QHBoxLayout *TabLayout = new QHBoxLayout();
    QTabWidget *testWidget = new QTabWidget();

    for (int i = 1; i <= 5; ++i) {
        QTabWidget *contentWidget = new QTabWidget();
        TabLayout->addWidget(testWidget);
        QString content = "Skill#" + QString::number(i);
        testWidget->addTab(contentWidget, content);
        setLayout(TabLayout);
    }
}

DynamicCustomTab::~DynamicCustomTab() {
    delete ui;
}

void DynamicCustomTab::SkillButton_clicked() {
    QMessageBox::information(this, "Skill Status Report", "Err/Warn/OK");
    std::cout << "NOW IN SKILL BUTTON CLICKED \n";
}

void DynamicCustomTab::on_SendMESData() {
    std::cout << "onsendMESDATA" << std::endl;
}
