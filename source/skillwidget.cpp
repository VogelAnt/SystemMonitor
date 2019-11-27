#include "skillwidget.h"
#include "ui_skillwidget.h"

static std::map<int, QString> sMap_String_OPCUAState{
    {0, "Idle"},
    {2, "Running"},
    {12, "Complete"},
    {17, "Aborted"},
    {15, "Stopped"},
    {1, "Starting"},
    {11, "Completing"},
    {18, "Clearing"},
    {13, "Resetting"},
    {14, "Stopping"},
    {16, "Aborting"}
};

static std::map<QString, QString> sMap_State_Colour{
    {"Idle", "lightgreen"},
    {"Running", "green"},
    {"Complete", "lightblue"},
    {"Aborted", "red"},
    {"Stopped", "grey"},
    {"Starting", "grey"},
    {"Completing", "grey"},
    {"Clearing", "grey"},
    {"Resetting", "grey"},
    {"Stopping", "grey"},
    {"Aborting", "lightred"}
};

SkillWidget::SkillWidget(std::map<char*, char*>eMap_DisplayName_NodeId, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SkillWidget){
    ui->setupUi(this);
    // so if I were to put Module Widget in here, then we make the connect here
    auto central = new QWidget(this);
    QVBoxLayout *ButtonLayout = new QVBoxLayout(this);
    timer = new QTimer();
    SkillMap_Id = eMap_DisplayName_NodeId;

    for (auto &pair: SkillMap_Id){
        QPushButton *skillButton = new QPushButton(pair.first,this);
        SkillMap_Button[pair.first] = skillButton;
        ButtonLayout->addWidget(skillButton);
        SkillMap_Button[pair.first]->setStyleSheet("font-size: 24px");
    }
    central->setLayout(ButtonLayout);
    setCentralWidget(central);
}

SkillWidget::~SkillWidget(){
    delete ui;
}
