#include "dynamiccustomtab.h"
#include "redisclient.h"
#include "ui_dynamiccustomtab.h"

//void TabBar::paintEvent(QPaintEvent *){
//    std::cout << "Now in Paint Event" <<std::endl;
//    QStylePainter painter(this);
//    QStyleOptionTab opt;
//    for(int i = 0;i < count();i++){
//        initStyleOption(&opt,i);
//        if(mColors.contains(opt.text)){
//            opt.palette.setColor(QPalette::Button, mColors[opt.text]);
//        }
//        painter.drawControl(QStyle::CE_TabBarTabShape, opt);
//        painter.drawControl(QStyle::CE_TabBarTabLabel,opt);
//    }
//}

DynamicCustomTab::DynamicCustomTab(QWidget *parent) : QTabWidget(parent), ui(new Ui::DynamicCustomTab) {
    ui->setupUi(this);
    QHBoxLayout *TabLayout = new QHBoxLayout();
    QTabWidget *testWidget = new QTabWidget();

    QHash<QString, QColor> hash;
    hash["test"] = QColor("yellow");
    setTabBar(new TabBar(hash));
    // add the Skill Widget in here (labels )
//    for (int i = 1; i <= 5; ++i) {
//        QTabWidget *contentWidget = new QTabWidget();
//        TabLayout->addWidget(testWidget);
//        QString content = "Skill#" + QString::number(i);
//        testWidget->addTab(contentWidget, content);
//        setLayout(TabLayout);
//    }
}

DynamicCustomTab::~DynamicCustomTab() {
    delete ui;
}

void DynamicCustomTab::SkillButton_clicked() {
    QMessageBox::information(this, "Skill Status Report", "Err/Warn/OK");
    std::cout << "NOW IN SKILL BUTTON CLICKED \n";
}

void DynamicCustomTab::on_sendModuleState(std::map<std::string, std::string> ModuleSkillMap)
{
    qDebug() << "Now in on_SendModuleState() !";
}

void DynamicCustomTab::on_SendMESData() {
    std::cout << "onsendMESDATA" << std::endl;
}

