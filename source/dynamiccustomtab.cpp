#include "dynamiccustomtab.h"
#include "redisclient.h"
#include "ui_dynamiccustomtab.h"

DynamicCustomTab::DynamicCustomTab(QWidget *parent) : QWidget(parent), ui(new Ui::DynamicCustomTab) {
    ui->setupUi(this);
    QHBoxLayout *TabLayout = new QHBoxLayout();

    for (int i = 1; i <= 5; ++i) {
        // auto *
        SkillButton = new QPushButton("Skill" + QString::number(i));
        connect(SkillButton, SIGNAL(clicked()), SLOT(SkillButton_clicked()));
        TabLayout->addWidget(SkillButton);
    }

    // connect(SkillButton, SIGNAL(clicked()), SLOT(SkillButton_clicked()));
    setLayout(TabLayout);
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
