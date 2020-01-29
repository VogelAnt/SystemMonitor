#ifndef OPCUADEVICE_H
#define OPCUADEVICE_H
#include "IDevice.h"

#include "adapters/OpcuaClient.h"
#include "skills/ISkill.h"

class OpcuaDevice : public IDevice {
public:
    explicit OpcuaDevice(const QString &eName, const uint8_t &eNamespace, const QString &eNodeId, QObject *parent = nullptr);
    ~OpcuaDevice() final;

    QString Name() const final {
        return m_Name;
    }

    QString NodeId() const final {
        return m_NodeId;
    }

    std::map<QString, ISkill *> SkillMap() const final {
        return m_SkillMap;
    }

    void AddSkill(QString eSkillName) final {
        auto skill = new ISkill(eSkillName, m_NodeId);
        m_SkillMap[eSkillName] = skill;
    }

    void SetUrl(QString eUrl) final {
        m_Url = eUrl;
    }

    PackMLState LastKnownState() const final {}

    void Connect() final {
        if (m_Url.isEmpty()) { throw std::invalid_argument("The URL is empty"); }
        m_Client.Connect(m_Url);
    }

    void Disconnect() final {
        m_Client.Disconnect();
    }

    PackMLState GetDeviceState() final {
        auto temp = m_Client.ReadNode(Namespace, m_NodeId + ".state.stateMachine.operationalState");
        return static_cast<PackMLState>(temp.toInt());
    }

    PackMLState GetSkillState(QString eSkillName) final {
        if (m_SkillMap.find(eSkillName) != m_SkillMap.end()) {
            auto temp = m_Client.ReadNode(Namespace, m_SkillMap[eSkillName]->NodeId + ".state.stateMachine.operationalState");
            return static_cast<PackMLState>(temp.toInt());
        }
    }

    void TriggerSkillStateTransition() final {
        //        m_Client.WriteNode();
    }

    // TODO: fill in the functionality of the trigger functions
    void TriggerAbort() final {}
    void TriggerClear() final {}
    void TriggerReset() final {}
    void TriggerStart() final {}
    void TriggerStop() final {}
    unsigned int Namespace;

private:
    QString m_Url;
    QString m_Name;
    QString m_NodeId;
    OpcuaClient m_Client;
    std::map<QString, ISkill *> m_SkillMap;
};

#endif // OPCUADEVICE_H
