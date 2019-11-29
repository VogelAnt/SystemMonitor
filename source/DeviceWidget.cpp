#include "DeviceWidget.h"
#include "ui_SkillListWidget.h"

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

DeviceWidget::DeviceWidget(UA_Client* client,
                                 std::map< char*,  char*> eMap_Device_DisplayName_NodeId,
                                 std::map< char*,  char*> eMap_Skill_DisplayName_NodeId,
                                 uint8_t index, int tabWIndex,
                                 QWidget *parent) : QMainWindow(parent), ui(new Ui::DeviceWidget) {
    ui->setupUi(this);
    timer = new QTimer();
    DeviceInformation *DeviceInfo = new DeviceInformation(client, eMap_Device_DisplayName_NodeId, eMap_Skill_DisplayName_NodeId, index);
    QVBoxLayout *ButtonLayout = new QVBoxLayout(this);
    auto central = new QWidget(this);
    m_UaClient = client;
    tabIndex = tabWIndex;
    DeviceMap_Id = eMap_Device_DisplayName_NodeId;
    SkillMap_Id = eMap_Skill_DisplayName_NodeId;
    DeviceNameSpace = index;
    for (auto &pair: SkillMap_Id){
        SkillButton = new QPushButton(pair.first, this);
        SkillMap_Button[pair.first] = SkillButton;
        ButtonLayout->addWidget(SkillButton);
        SkillMap_Button[pair.first]->setStyleSheet("font-size: 24px");
    }
    central->setLayout(ButtonLayout);
    setCentralWidget(central);

    connect(timer, &QTimer::timeout, DeviceInfo, &DeviceInformation::on_UpdateDeviceInformation);
    connect(DeviceInfo, &DeviceInformation::UpdateUiDeviceState, this, &DeviceWidget::on_UpdateDeviceUI);
    connect(DeviceInfo, &DeviceInformation::UpdateUiSkillState, this, &DeviceWidget::on_UpdateSkillsUI);

    timer->start(1000);
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

}
