#ifndef DYNAMICCUSTOMTAB_H
#define DYNAMICCUSTOMTAB_H
#include "redisclient.h"
#include "open62541.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QWidget>
#include <QStyleOptionTab>
#include <QStylePainter>
#include <QTabWidget>
#include <QButtonGroup>

#include <iostream>

enum class NodeIdentifierType{
    UAString,
    UAInt
};

namespace Ui {
class SkillListWidget;
}

class SkillListWidget : public QMainWindow {
    Q_OBJECT
public:
    //
    explicit SkillListWidget(UA_Client * eUaClient,
                             std::map< char*,  char*> eMap_DisplayName_NodeId, // the display name and node id of each skill
                             uint8_t index,
                             QWidget *parent = nullptr);
    virtual ~SkillListWidget();

    void CreateButton(std::string, std::pair<char*, char*>);
    QButtonGroup *SkillButtonGroup = nullptr;

public slots:
    void on_SendSkillState(std::map<std::string, std::string> nodevalues);
    void on_SendAssemblySkillState(std::map<std::string, std::string> nodevalues);
    void on_SendSuperTrakSkillState(std::map<std::string, std::string> nodevalues);
    void on_SendLabelingSkillState(std::map<std::string, std::string> nodevalues);

    //functions for testing
    void SkillButton_clicked();
    void on_SendMESData();

    // signals: timeout --> RequestUpdate;
signals:
    void SkillStateUpdate();
    // slot: on_SkillUpdateReceived do sth with the buttons
    /**
     * @brief on_sendModuleState
     * receives ModuleState information
     */
private:
    Ui::SkillListWidget *ui;
    QPushButton *SkillButton;
    UA_Client *m_UaClient;
    QTimer *timer;
    std::map< char*,  char*> SkillMap_Id;
    std::map< char*,  QPushButton*> SkillMap_Button;
    uint8_t DeviceNameSpace;
};
#endif // DYNAMICCUSTOMTAB_H
