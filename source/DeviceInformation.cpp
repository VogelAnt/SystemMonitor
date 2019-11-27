#include "DeviceInformation.h"

DeviceInformation::DeviceInformation(UA_Client *client,
                       std::map< char*,  char*> eMap_Device_DisplayName_NodeId, // the display name and node id of each skill
                       std::map< char*,  char*> eMap_Skill_DisplayName_NodeId, // the display name and node id of each skill
                       uint8_t index){
    m_UaClient = client;
    SkillMap_Id = eMap_Skill_DisplayName_NodeId;
    DeviceMap_Id = eMap_Device_DisplayName_NodeId;
    DeviceNameSpace = index;
}

// TODO: hold your horses we#re not even passing the DeviceNamespace....
void DeviceInformation::on_UpdateDeviceInformation(){
    // read Device state from Server
    for(auto &pair : DeviceMap_Id){
        UA_NodeId nodeId = UA_NODEID_STRING(DeviceNameSpace, pair.second);
        UA_Variant value;
        UA_Variant_init(&value);
        UA_Client_readValueAttribute(m_UaClient, nodeId, &value);

        if(value.type->typeName == UA_TYPES[UA_TYPES_INT32].typeName){
            std::string temp = std::to_string(*((UA_Int32*) value.data));
            UpdateUiDeviceState(temp, pair);
        }
        else if(value.type->typeName == UA_TYPES[UA_TYPES_STRING].typeName){
            std::string temp = ((char const*)(((UA_String*) value.data)->data));
            UpdateUiDeviceState(temp, pair);
        }
    }
    // read skill states from server
    for(auto &pair : SkillMap_Id){
        UA_NodeId nodeId = UA_NODEID_STRING(DeviceNameSpace, pair.second);
        UA_Variant value;
        UA_Variant_init(&value);
        UA_Client_readValueAttribute(m_UaClient, nodeId, &value);

        if(value.type->typeName == UA_TYPES[UA_TYPES_INT32].typeName){
            std::string temp = std::to_string(*((UA_Int32*) value.data));
            UpdateUiSkillState(temp, pair);
        }
        else if(value.type->typeName == UA_TYPES[UA_TYPES_STRING].typeName){
            std::string temp = ((char const*)(((UA_String*) value.data)->data));
            UpdateUiSkillState(temp, pair);
        }
    }
}
// slot for being polled by UI
// if we poll from here then we need another
