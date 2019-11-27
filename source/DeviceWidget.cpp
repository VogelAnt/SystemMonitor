#include "DeviceWidget.h"

#include <QWidget>

DeviceWidget::DeviceWidget(UA_Client *client,
                           std::map< char*,  char*> eMap_Device_DisplayName_NodeId,
                           std::map< char*,  char*> eMap_Skill_DisplayName_NodeId,
                           uint8_t index,
                           QWidget *parent) :QMainWindow(parent) {
    DeviceInformation *DeviceInfo = new DeviceInformation(client, m_DeviceMap_Id, m_SkillMap_Id, m_DeviceNameSpace);
    m_updatetimer = new QTimer();
    m_client = client;
    m_DeviceMap_Id = eMap_Device_DisplayName_NodeId;
    m_SkillMap_Id = eMap_Skill_DisplayName_NodeId;
    m_DeviceNameSpace = index;
    connect(m_updatetimer, &QTimer::timeout, DeviceInfo, &DeviceInformation::on_UpdateDeviceInformation);


    // make a slot here that reacts to the widgets being updated
    // poll for the Module state here
}


DeviceWidget::~DeviceWidget(){

}
