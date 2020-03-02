#include "OpcuaDevice.h"

OpcuaDevice::OpcuaDevice(const QString &eName, const uint8_t &eNamespace, const QString &eNodeId, QObject *parent) : IDevice(parent) {
    m_Name = eName;
    m_NodeId = eNodeId;
    m_Namespace = eNamespace;
}

OpcuaDevice::~OpcuaDevice() {
    m_Client.Disconnect();
    m_SkillMap.clear();
}
