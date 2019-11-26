#ifndef MODULEWIDGET_H
#define MODULEWIDGET_H

#include "open62541.h"

#include <QWidget>

namespace Ui {
class DeviceWidget;
}

// TODO: shouldn't this class be called DeviceManager ? or Deviceinformation ?
class DeviceWidget : public QWidget{
    Q_OBJECT

public:
    DeviceWidget(UA_Client * eUaClient,
                 std::map< char*,  char*> eMap_Device_DisplayName_NodeId, // the display name and node id of each skill
                 std::map< char*,  char*> eMap_Skill_DisplayName_NodeId, // the display name and node id of each skill
                 uint8_t index);
    ~DeviceWidget();

//signals:
// trigger ui (specifically tabWidget)

//public slots: Request Update, put the branch thing i there, freom there trigger signals to make the UI in SkillWidget
public slots:
    void on_SkillStateUpdate();
private:
    UA_Client *m_UaClient;
    std::map< char*, char*> SkillMap_Id;
    std::map< char*, char*> DeviceMap_Id;
    uint8_t DeviceNameSpace;
};

#endif // MODULEWIDGET_H
