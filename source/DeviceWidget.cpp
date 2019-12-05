#include "DeviceWidget.h"
#include "ui_SkillListWidget.h"

static std::map<int, QString> sMap_String_OPCUAState{
    {0, "Idle"},
    {2, "Running"},
    {12, "Complete"},
    {17, "Aborted"},
    {15, "Stopped"},
    {1, "Starting"},
    {4, "Paused"},
    {11, "Completing"},
    {18, "Clearing"},
    {13, "Resetting"},
    {14, "Stopping"},
    {16, "Aborting"},
    {19, "Blocked"}
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
    {"Aborting", "lightred"},
    {"Paused", "pink"},
    {"Blocked", "cyan"}
};

DeviceWidget::DeviceWidget(UA_Client* client,
                                 std::map< char*,  char*> eMap_Device_DisplayName_NodeId,
                                 std::map< char*,  char*> eMap_Skill_DisplayName_NodeId,
                                 uint8_t index, int tabWindex,
                                 QWidget *parent) : QMainWindow(parent), ui(new Ui::DeviceWidget) {
    ui->setupUi(this);
    m_timer = new QTimer();
    m_UaClient = client;
    tabIndex = tabWindex;
    DeviceMap_Id = eMap_Device_DisplayName_NodeId;
    SkillMap_Id = eMap_Skill_DisplayName_NodeId;
    DeviceNameSpace = index;
    m_deviceinfo = new DeviceInformation(client, eMap_Device_DisplayName_NodeId, eMap_Skill_DisplayName_NodeId, index);
    m_central = new QWidget(this);
    m_buttonLayout = new QVBoxLayout(m_central);

    for (auto &pair: SkillMap_Id){
        SkillButton = new QPushButton(pair.first, this);
        SkillMap_Button[pair.first] = SkillButton;
        m_buttonLayout->addWidget(SkillButton);
        SkillMap_Button[pair.first]->setStyleSheet("font-size: 24px");
    }

    m_central->setLayout(m_buttonLayout);
    setCentralWidget(m_central);
    connect(m_timer, &QTimer::timeout, m_deviceinfo, &DeviceInformation::on_UpdateDeviceInformation);
    connect(m_deviceinfo, &DeviceInformation::UpdateUiDeviceState, this, &DeviceWidget::on_UpdateDeviceUI);
    connect(m_deviceinfo, &DeviceInformation::UpdateUiSkillState, this, &DeviceWidget::on_UpdateSkillsUI);
    m_timer->start(1000);
}

void DeviceWidget::on_UpdateSkillsUI(std::string nodevalue, std::pair<char *, char *>pair){
    int n = std::stoi(nodevalue);
    QString n_value = sMap_String_OPCUAState.find(n)->second;
    QString node_value = " : " + sMap_String_OPCUAState.find(n)->second;
    SkillMap_Button[pair.first]->setText(pair.first + node_value);
    QString backgroundColor = sMap_State_Colour.find(n_value)->second;
    QString temp_buttoncolour = "background-color : " + backgroundColor + "; font-size: 24px";
    SkillMap_Button[pair.first]->setStyleSheet(temp_buttoncolour);
}

void DeviceWidget::on_UpdateDeviceUI(std::string nodevalue, std::pair<char *, char *>pair){
    int n = std::stoi(nodevalue);
    QString n_value = sMap_String_OPCUAState.find(n)->second;
    QString node_value = " : " + sMap_String_OPCUAState.find(n)->second;
    QString tmp = sMap_State_Colour.find(n_value)->second;
    QString tabText = pair.first + node_value;
    ChangeDeviceStatus(tabIndex, tmp, tabText);
}

DeviceWidget::~DeviceWidget(){
    delete ui;
    delete SkillButton;
    delete m_timer;
    delete m_deviceinfo;
    delete m_buttonLayout;
    delete m_central;
}
