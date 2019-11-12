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

static QStringList AssemblySkillList{"Provide Cup", "Provide Pellet"};
static QStringList SuperTrakSkillList{"Check Parking", "Move Shuttle 1",
                                     "Move Shuttle 2",
                                     "Move Shuttle 3",
                                     "Move Shuttle 4",
                                     "Move Shuttle 5",
                                     "Move Shuttle 6"};
static QStringList LabelingSkillList{"Label"};

namespace Ui {
class SkillListWidget;
}

class SkillListWidget : public QWidget {
    Q_OBJECT

public:
    //
    explicit SkillListWidget(UA_Client * eUaClient,
                             std::map< char*,  char*> eMap_DisplayName_NodeId, // the display name and node id of each skill
                             uint8_t index,
                             QWidget *parent = nullptr);
    virtual ~SkillListWidget();
    QButtonGroup *SkillButtonGroup = nullptr;

public slots:
    void on_SendSkillState(std::map<std::string, std::string> nodevalues);
    void on_SendAssemblySkillState(std::map<std::string, std::string> nodevalues);
    void on_SendSuperTrakSkillState(std::map<std::string, std::string> nodevalues);
    void on_SendLabelingSkillState(std::map<std::string, std::string> nodevalues);

    //functions for testing
    void SkillButton_clicked();
    void on_SendMESData();
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
    uint8_t ModuleNameSpace;
};


#endif // DYNAMICCUSTOMTAB_H
