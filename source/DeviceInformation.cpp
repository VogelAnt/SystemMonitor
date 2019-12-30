#include "DeviceInformation.h"

#include <open62541.h>

DeviceInformation::DeviceInformation(
    UA_Client *client,
    std::map<char *, char *> eMap_Device_DisplayName_NodeId, // the display name and node id of each skill
    std::map<char *, char *> eMap_Skill_DisplayName_NodeId, // the display name and node id of each skill
    uint8_t index) {
    m_UaClient = client;
    SkillMap_Id = eMap_Skill_DisplayName_NodeId;
    DeviceMap_Id = eMap_Device_DisplayName_NodeId;
    DeviceNameSpace = index;
    m_skillVector = new std::vector<std::string>();
    for (auto &pair : SkillMap_Id) { m_skillVector->push_back(pair.second); }
}

void DeviceInformation::on_UpdateDeviceInformation() {
    // TODO: redundancy, call a function from here ! pass into the function a boolean, sth that denotes we are either using a Device or Skill Update
    for (auto &pair : DeviceMap_Id) {
        UA_NodeId nodeId = UA_NODEID_STRING(DeviceNameSpace, pair.second);
        UA_Variant value;
        UA_Variant_init(&value);
        UA_Client_readValueAttribute(m_UaClient, nodeId, &value);

        if (value.type->typeName == UA_TYPES[UA_TYPES_INT32].typeName) {
            std::string temp = std::to_string(*((UA_Int32 *)value.data));
            UpdateUiDeviceState(temp, pair);
        } else if (value.type->typeName == UA_TYPES[UA_TYPES_STRING].typeName) {
            std::string temp = ((char const *)(((UA_String *)value.data)->data));
            UpdateUiDeviceState(temp, pair);
        }
    }
    // read skill states from server
    for (auto &pair : SkillMap_Id) {
        UA_NodeId nodeId = UA_NODEID_STRING(DeviceNameSpace, pair.second);
        UA_Variant value;
        UA_Variant_init(&value);
        UA_Client_readValueAttribute(m_UaClient, nodeId, &value);

        if (value.type->typeName == UA_TYPES[UA_TYPES_INT32].typeName) {
            std::string temp = std::to_string(*((UA_Int32 *)value.data));
            UpdateUiSkillState(temp, pair);
        } else if (value.type->typeName == UA_TYPES[UA_TYPES_STRING].typeName) {
            std::string temp = ((char const *)(((UA_String *)value.data)->data));
            UpdateUiSkillState(temp, pair);
        }
    }
}

void DeviceInformation::on_AbortDeviceManually() {
    // TODO: I need a UA_String for nodeId
    std::map<char *, char *>::iterator it = DeviceMap_Id.begin();
    QString transitionString = "";
    QString nodeIdtransitionState = it->second;
    transitionString = ".stateTransition.abort";
    int dotPosition = nodeIdtransitionState.lastIndexOf(QChar('.'));
    transitionString = nodeIdtransitionState.left(dotPosition) + transitionString;
    std::string transString = transitionString.toStdString();
    UA_NodeId nodeId = UA_NODEID_STRING(DeviceNameSpace, const_cast<char *>(transString.c_str()));
    UA_Variant value;
    UA_Variant_init(&value);
    UA_Client_writeValueAttribute(m_UaClient, nodeId, &value);
}

void DeviceInformation::on_TriggerSkillStateManually(std::string y, int z) {
    // TODO: I need a UA_String for nodeId
    for (auto &pair : DeviceMap_Id) {
        UA_NodeId nodeId = UA_NODEID_STRING(DeviceNameSpace, pair.second);
        UA_Variant value;
        UA_Variant_init(&value);
        UA_Client_writeValueAttribute(m_UaClient, nodeId, &value);
    }
}

DeviceInformation::~DeviceInformation() {
    delete m_UaClient;
}
