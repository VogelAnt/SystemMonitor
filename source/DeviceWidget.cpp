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
        return "lightred";
    default:
        return "grey";
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
    ui->setupUi(this);
    m_timer = new QTimer();
    m_tabIndex = eTabIndex;

    m_Device = eDevice;
    MakeButtonLayout();
    connect(m_timer, &QTimer::timeout, this, &DeviceWidget::UpdateDeviceInfo);
    connect(m_abortButton, &QPushButton::clicked, this, &DeviceWidget::on_AbortButtonClicked);
    m_timer->start(1000);
}

void DeviceWidget::MakeButtonLayout() {
    // move this stuff into a separate function
    m_central = new QWidget(this);
    m_buttonLayout = new QVBoxLayout(m_central);
    m_abortButton = new QPushButton("ABORT DEVICE", this);
    m_abortButton->setStyleSheet("font-size : 24px");
    m_buttonLayout->addWidget(m_abortButton);
    int i = 0;
    for (auto &pair : m_Device->SkillMap()) {
        auto skill = pair.second;
        SkillButton = new QPushButton(skill->Name, this);
        SkillMap_Button[skill->Name] = SkillButton;
        m_buttonLayout->addWidget(SkillButton);
        SkillMap_Button[skill->Name]->setStyleSheet("font-size: 24px");
        connect(SkillMap_Button[skill->Name], &QPushButton::clicked, this, &DeviceWidget::on_SkillButtonClicked);
        ++i;
    }
    m_central->setLayout(m_buttonLayout);
    setCentralWidget(m_central);
}

void DeviceWidget::on_AbortButtonClicked() {
    int actionValue = QMessageBox::critical(
        this,
        "ABORTING DEVICE...",
        "The selected device will be aborted, do you really want to proceed?",
        QMessageBox::Abort | QMessageBox::Cancel,
        QMessageBox::Cancel);

    switch (actionValue) {
    case QMessageBox::Abort:
        qDebug() << "NOW ABORTING";
        m_Device->TriggerAbort();
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
    delete m_buttonLayout;
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
        QString temp_buttoncolour = QString("background-color : ") + StateToColorString(skillState).c_str() + "; font-size: 24px";
        button->setStyleSheet(temp_buttoncolour);
    }
}
