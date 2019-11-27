#ifndef DEVICEINFO_H
#define DEVICEINFO_H
#include "open62541.h"

#include <QTimer>

#include <map>
#include <string>

/**
 * @brief The DeviceInfo class
 * Polls the OPCUA server of the respective device for information on Device and Skill states.
 * Sends Device and Skill information to DeviceWidget
 */
class DeviceInformation{
public:
    DeviceInformation(UA_Client *eUaClient,
               std::map< char*,  char*> eMap_Device_DisplayName_NodeId,
               std::map< char*,  char*> eMap_Skill_DisplayName_NodeId,
               uint8_t index);

public slots:
    /**
     * @brief on_UpdateDeviceInformation
     * Request info about Device state and Skill states
     */
    void on_UpdateDeviceInformation();

signals:
    /**
     * @brief UpdateUiDeviceState
     * Update the TabWidget in MainWindow
     * @param nodevalue
     * @param pair
     */
    void UpdateUiDeviceState(std::string nodevalue, std::pair<char*, char*>pair);
    /**
     * @brief UpdateUiSkillState
     * Update Widget inside of TabWidget
     * @param nodevalue
     * @param pair
     */
    void UpdateUiSkillState(std::string nodevalue, std::pair<char *, char *>pair);


private:
    UA_Client *m_UaClient;
    std::map< char*, char*> m_SkillMap_Id;
    std::map< char*, char*> DeviceMap_Id;
    uint8_t DeviceNameSpace;
};

#endif // DEVICEINFO_H
