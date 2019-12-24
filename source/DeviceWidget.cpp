#include "DeviceWidget.h"
#include "ui_DeviceWidget.h"

static std::map<int, QString> sMap_String_OPCUAState{{0, "Idle"},
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
                                                     {19, "Blocked"}};

static std::map<QString, QString> sMap_State_Colour{{"Idle", "lightgreen"},
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
                                                    {"Blocked", "cyan"}};

static QStringList sDevice_Triggers{"TriggerAbort", "TriggerClear", "TriggerReset", "TriggerStart", "TriggerStop"};

DeviceWidget::DeviceWidget(
    UA_Client *client,
    std::map<char *, char *> eMap_Device_DisplayName_NodeId,
    std::map<char *, char *> eMap_Skill_DisplayName_NodeId,
    uint8_t index,
    int tabWindex,
    QWidget *parent)
    : QMainWindow(parent), ui(new Ui::DeviceWidget) {
    ui->setupUi(this);
    m_timer = new QTimer();
    m_UaClient = client;
    tabIndex = tabWindex;
    DeviceMap_Id = eMap_Device_DisplayName_NodeId;
    SkillMap_Id = eMap_Skill_DisplayName_NodeId;
    DeviceNameSpace = index;
    m_deviceinfo = new DeviceInformation(client, eMap_Device_DisplayName_NodeId, eMap_Skill_DisplayName_NodeId, index);
    MakeButtonLayout();
    connect(m_timer, &QTimer::timeout, m_deviceinfo, &DeviceInformation::on_UpdateDeviceInformation);
    connect(m_abortButton, &QPushButton::clicked, this, &DeviceWidget::on_AbortButtonClicked);
    connect(m_deviceinfo, &DeviceInformation::UpdateUiDeviceState, this, &DeviceWidget::on_UpdateDeviceUI);
    connect(m_deviceinfo, &DeviceInformation::UpdateUiSkillState, this, &DeviceWidget::on_UpdateSkillsUI);
    connect(this, &DeviceWidget::TriggerDeviceStateManually, m_deviceinfo, &DeviceInformation::on_TriggerDeviceStateManually);
    connect(this, &DeviceWidget::TriggerSkillStateManually, m_deviceinfo, &DeviceInformation::on_TriggerSkillStateManually);
    m_timer->start(1000);
}

void DeviceWidget::MakeButtonLayout() {
    // move this stuff into a separate function
    m_central = new QWidget(this);
    m_buttonLayout = new QVBoxLayout(m_central);
    m_abortButton = new QPushButton("ABORT DEVICE", this);
    m_abortButton->setStyleSheet("font-size : 24px");
    m_buttonLayout->addWidget(m_abortButton);
    for (auto &pair : SkillMap_Id) {
        SkillButton = new QPushButton(pair.first, this);
        SkillMap_Button[pair.first] = SkillButton;
        m_buttonLayout->addWidget(SkillButton);
        SkillMap_Button[pair.first]->setStyleSheet("font-size: 24px");
        connect(SkillMap_Button[pair.first], &QPushButton::clicked, this, &DeviceWidget::on_SkillButtonClicked);
    }
    m_central->setLayout(m_buttonLayout);
    setCentralWidget(m_central);
}

void DeviceWidget::on_UpdateSkillsUI(std::string nodevalue, std::pair<char *, char *> pair) {
    int n = std::stoi(nodevalue);
    QString n_value = sMap_String_OPCUAState.find(n)->second;
    QString node_value = " : " + sMap_String_OPCUAState.find(n)->second;
    SkillMap_Button[pair.first]->setText(pair.first + node_value);
    QString backgroundColor = sMap_State_Colour.find(n_value)->second;
    QString temp_buttoncolour = "background-color : " + backgroundColor + "; font-size: 24px";
    SkillMap_Button[pair.first]->setStyleSheet(temp_buttoncolour);
}

void DeviceWidget::on_UpdateDeviceUI(std::string nodevalue, std::pair<char *, char *> pair) {
    int n = std::stoi(nodevalue);
    QString n_value = sMap_String_OPCUAState.find(n)->second;
    QString node_value = " : " + sMap_String_OPCUAState.find(n)->second;
    QString tmp = sMap_State_Colour.find(n_value)->second;
    QString tabText = pair.first + node_value;
    ChangeDeviceStatus(tabIndex, tmp, tabText);
}

void DeviceWidget::on_AbortButtonClicked() {
    int actionValue = QMessageBox::critical(
        this,
        "ABORTING DEVICE...",
        "The selected device will be aborted, do you really want to proceed?",
        QMessageBox::Abort | QMessageBox::Cancel,
        QMessageBox::Cancel);
    std::map<char *, char *>::iterator it = DeviceMap_Id.begin();
    QString transitionString = "";
    QString nodeIdtransitionState = it->second;
    transitionString = ".stateTransition.abort";
    int dotPosition = nodeIdtransitionState.lastIndexOf(QChar('.'));
    transitionString = nodeIdtransitionState.left(dotPosition) + transitionString;
    qDebug() << transitionString;
    switch (actionValue) {
    case QMessageBox::Abort:
        qDebug() << "NOW ABORTING";
        emit TriggerDeviceStateManually(transitionString.toStdString());
        break;
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
}

void DeviceWidget::on_SkillButtonClicked() {
    bool ok;
    int pos = m_buttonLayout->indexOf(qobject_cast<QPushButton *>(sender()));
    qDebug() << pos;
    QString trigger = qobject_cast<QPushButton *>(sender())->text();
    QString test = QInputDialog::getItem(this, trigger, "Trigger Skill State of " + trigger, sDevice_Triggers, 0, false);

    if (ok && !trigger.isEmpty()) {
        if (trigger == "abort") {

        } else if (trigger == "clear") {

        } else if (trigger == "reset") {

        } else if (trigger == "start") {

        } else if (trigger == "stop") {
        }
    }
    //    emit TriggerSkillStateManually();
}

DeviceWidget::~DeviceWidget() {
    delete ui;
    delete SkillButton;
    delete m_timer;
    delete m_deviceinfo;
    delete m_buttonLayout;
    delete m_central;
}
