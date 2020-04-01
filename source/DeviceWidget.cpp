#include "DeviceWidget.h"
#include "ui_DeviceWidget.h"

#include <QTimer>

QStringList sDevice_Triggers{"abort", "clear", "reset", "start", "stop"};

std::string StateToColorString(PackMLState eState) {
    switch (eState) {
    case PackMLState::Idle:
        return "lightgreen";
    case PackMLState::Running:
        return "green";
    case PackMLState::Complete:
        return "lightblue";
    case PackMLState::Aborted:
        return "red";
    case PackMLState::Stopped:
        return "darkgrey";
    case PackMLState::Starting:
        return "starting";
    case PackMLState::Completing:
        return "completing";
    case PackMLState::Clearing:
        return "clearing";
    case PackMLState::Resetting:
        return "resetting";
    case PackMLState::Stopping:
        return "stopping";
    case PackMLState::Aborting:
        return "lightred";
    default:
        return "grey";
    }
}

PackMLStateTransition StringToStateTransition(QString eString) {
    if (eString == "abort") {
        return PackMLStateTransition::Abort;
    } else if (eString == "clear") {
        return PackMLStateTransition::Clear;
    } else if (eString == "reset") {
        return PackMLStateTransition::Reset;
    } else if (eString == "start") {
        return PackMLStateTransition::Start;
    } else if (eString == "stop") {
        return PackMLStateTransition::Stop;
    }
}

std::string StateToString(PackMLState eState) {
    switch (eState) {
    case PackMLState::Idle:
        return "idle";
    case PackMLState::Running:
        return "running";
    case PackMLState::Complete:
        return "complete";
    case PackMLState::Aborted:
        return "aborted";
    case PackMLState::Stopped:
        return "stopped";
    case PackMLState::Starting:
        return "starting";
    case PackMLState::Completing:
        return "completing";
    case PackMLState::Clearing:
        return "clearing";
    case PackMLState::Resetting:
        return "resetting";
    case PackMLState::Stopping:
        return "stopping";
    case PackMLState::Aborting:
        return "aborting";
    default:
        throw std::invalid_argument(std::to_string((uint8_t)eState));
    }
}

DeviceWidget::DeviceWidget(IDevice *eDevice, int eTabIndex, QWidget *parent) : QMainWindow(parent), ui(new Ui::DeviceWidget) {
    //    ui->setupUi(this);
    m_timer = new QTimer();
    m_tabIndex = eTabIndex;
    m_Device = eDevice;
    //    qDebug() << "Device Name:" << m_Device->Name();
    //    qDebug() << "Node ID: " << m_Device->NodeId();
    //    qDebug() << "Namespace: " << m_Device->NameSpace();
    //    qDebug() << "state: " << m_Device->GetDeviceState();

    MakeButtonLayout();
    connect(m_timer, &QTimer::timeout, this, &DeviceWidget::UpdateDeviceInfo);
    connect(m_abortButton, &QPushButton::clicked, this, &DeviceWidget::on_AbortButtonClicked);
    m_timer->start(1000);
}

void DeviceWidget::MakeButtonLayout() {
    m_central = new QWidget(this);
    m_buttonLayout = new QVBoxLayout(m_central);
    m_abortButton = new QPushButton("Trigger Device State", m_central);
    m_abortButton->setStyleSheet("font-size : 18px");
    m_buttonLayout->addWidget(m_abortButton);
    int i = 0;
    for (auto &pair : m_Device->SkillMap()) {
        auto skill = pair.second;
        SkillButton = new QPushButton(skill->Name, m_central);
        SkillMap_Button[skill->Name] = SkillButton;
        m_buttonLayout->addWidget(SkillButton);
        SkillMap_Button[skill->Name]->setStyleSheet("font-size: 18px");
        connect(SkillMap_Button[skill->Name], &QPushButton::clicked, this, &DeviceWidget::on_SkillButtonClicked);
        ++i;
    }
    m_buttonLayout->insertStretch(-1, 1);
    m_central->setLayout(m_buttonLayout);
    setCentralWidget(m_central);
}

// TODO: This should be called "on_TriggerDeviceState"
// TODO: copy procedure from on_SkillButtonClicked
void DeviceWidget::on_AbortButtonClicked() {
    // TODO: make these members
    QString deviceName = m_Device->Name();
    QString nameSpace = m_Device->NodeId();
    //    QString triggerString = nameSpace + ".state.stateMachine.stateTransition.";
    QString triggerString = nameSpace;

    qDebug() << "trigger string:" << triggerString;
    bool ok;
    QString selection = QInputDialog::getItem(this, triggerString, "Trigger Skill State of " + triggerString, sDevice_Triggers, 0, false, &ok);
    if (ok && !(selection.isEmpty())) {
        //        triggerString += selection;
        qDebug() << "triggerstring : " << triggerString;
        // TODO: build the Message Box here
        m_Device->TriggerSkillStateTransition(triggerString, StringToStateTransition(selection));
    } else {
        qDebug() << "selection process for triggering device cancelled";
    }

    //    int actionValue = QMessageBox::critical(
    //        this,
    //        "ABORTING DEVICE...",
    //        "The selected device will be aborted, do you really want to proceed?",
    //        QMessageBox::Abort | QMessageBox::Cancel,
    //        QMessageBox::Cancel);
    //    QString transitionString = ".state.stateMachine.stateTransition.abort";
    //    QString nodeIdtransitionstate = m_Device->NodeId();
    //    qDebug() << "m_device->NodeId()" << nodeIdtransitionstate;
    //    int dotPosition = nodeIdtransitionstate.lastIndexOf(QChar('.'));
    //    transitionString = nodeIdtransitionstate.left(dotPosition) + transitionString;
    //    qDebug() << transitionString;
    //    switch (actionValue) {
    //    case QMessageBox::Abort:
    //        qDebug() << "NOW ABORTING";
    //        m_Device->TriggerSkillStateTransition(m_Device->NodeId(), PackMLStateTransition::Abort);
    //        break;
    //    case QMessageBox::Cancel:
    //        break;
    //    default:
    //        break;
    //    }
}

// namespace.skill.skillName.state.stateMachine.stateTransition. --> add rest in switch
void DeviceWidget::on_SkillButtonClicked() {
    QString nodeId = m_Device->NodeId() + ".skill.";
    qDebug() << "nodeId.skill." << nodeId;
    QString skillNodeid = qobject_cast<QPushButton *>(sender())->text();
    int colonPosition = skillNodeid.lastIndexOf(QChar(':'));
    skillNodeid = skillNodeid.left(colonPosition);
    qDebug() << "skillClicked : " << skillNodeid;
    QString transitionString = "state.stateMachine.stateTransition.";
    bool ok;
    QString selection = QInputDialog::getItem(this, skillNodeid, "Trigger Skill State of " + skillNodeid, sDevice_Triggers, 0, false, &ok);
    if (ok && !(selection.isEmpty())) {
        // this part here is SkillNodeId
        skillNodeid = nodeId + skillNodeid;
        qDebug() << "node ID + SKill clicked/ skillnodeId: " << skillNodeid;
        QString selectionString = skillNodeid + "." + transitionString;
        qDebug() << "selectionString + transitionString : " << selectionString;
        selectionString = selectionString + selection;
        qDebug() << "selectionString + selection: " << selectionString;
        // skillClicked is the nodId string
        m_Device->TriggerSkillStateTransition(skillNodeid, StringToStateTransition(selection));
    } else {
        qDebug() << "Selection of skill state trigger cancelled";
    }
}

DeviceWidget::~DeviceWidget() {
    delete ui;
    delete SkillButton;
    delete m_timer;
    delete m_central;
}

void DeviceWidget::UpdateDeviceInfo() {
    auto state = m_Device->GetDeviceState();
    std::string nodeId = m_Device->Name().toStdString();
    nodeId += ": " + StateToString(state);
    emit DeviceStatusChanged(m_tabIndex, StateToColorString(state).c_str(), nodeId.c_str());
    for (auto &pair : m_Device->SkillMap()) {
        auto &skill = pair.second;
        auto skillState = m_Device->GetSkillState(skill->Name);

        std::string buttonText = skill->Name.toStdString();
        std::string skillStateString = StateToString(skillState);
        buttonText += ": " + skillStateString;
        auto button = SkillMap_Button[skill->Name];
        button->setText(buttonText.c_str());
        QString temp_buttoncolour = QString("background-color : ") + StateToColorString(skillState).c_str() + "; font-size: 18px";
        button->setStyleSheet(temp_buttoncolour);
    }
}
