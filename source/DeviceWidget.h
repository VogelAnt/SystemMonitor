#ifndef MODULEWIDGET_H
#define MODULEWIDGET_H
#include "open62541.h"

#include "DeviceInformation.h"

#include <QWidget>
#include <QMainWindow>
#include <QTimer>

#include <map>

namespace Ui {
class DeviceWidget;
}

/**
 * @brief The DeviceWidget class
 * UI displaying Device information
 */
class DeviceWidget : public QMainWindow{
    Q_OBJECT
public:
    DeviceWidget(UA_Client *client,
                 std::map< char*,  char*> eMap_Device_DisplayName_NodeId,
                 std::map< char*,  char*> eMap_Skill_DisplayName_NodeId,
                 uint8_t index,
                 QWidget *parent = nullptr);
    ~DeviceWidget();
//signals:
// trigger ui (specifically tabWidget)
signals:
    /**
     * @brief UpdateDeviceInformation
     * signal communicating newly updated Device info to DeviceWidget
     */
    void UpdateDeviceInformation();

public slots:
    void on_UpdateDeviceWidget();

private:
    QTimer *m_updatetimer;
    UA_Client *m_client;
    std::map< char*, char*> m_SkillMap_Id;
    std::map< char*, char*> m_DeviceMap_Id;
    uint8_t m_DeviceNameSpace;
};

#endif // MODULEWIDGET_H
