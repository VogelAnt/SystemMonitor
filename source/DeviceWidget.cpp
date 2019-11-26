#include "DeviceWidget.h"

#include <QWidget>

DeviceWidget::DeviceWidget(UA_Client* client,
                           std::map< char*,  char*> eMap_Device_DisplayName_NodeId,
                           std::map< char*,  char*> eMap_Skill_DisplayName_NodeId,
                           uint8_t index) {
    m_UaClient = client;
    DeviceMap_Id = eMap_Skill_DisplayName_NodeId;
    SkillMap_Id = eMap_Skill_DisplayName_NodeId;
    DeviceNameSpace = index;
    // make a slot here that reacts to the widgets being updated
    // poll for the Module state here

}

void DeviceWidget::on_SkillStateUpdate(){

}

DeviceWidget::~DeviceWidget(){

}
