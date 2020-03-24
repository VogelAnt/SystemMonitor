/**
 * @file OpcuaClient.h
 * @brief This file contains the OpcuaClient class
 */
#ifndef SMG_ADAPTERS_OPCUACLIENT_H
#define SMG_ADAPTERS_OPCUACLIENT_H
#include <QObject>

#include "smgCommon.h"

#include <open62541.h>

#include <QThread>
#include <QTimer>

#include <iostream>
#include <memory>
#include <mutex>
#include <string>
/**
 * @brief The OpcuaClient class is an open62541 based OPCUA client with the ability
 * to read and write to the corresponding server node
 */
class OpcuaClient : public QObject {
    Q_OBJECT
public:
    /**
     * @brief OpcuaClient
     * @param parent
     */
    explicit OpcuaClient(QObject *parent = nullptr);
    OpcuaClient(const OpcuaClient &a) = delete;
    OpcuaClient &operator=(const OpcuaClient &a) = delete;

    OpcuaClient(const OpcuaClient &&a) = delete;
    OpcuaClient &operator=(OpcuaClient &&a) = delete;
    ~OpcuaClient() final;

    UA_StatusCode Connect(const QString &eUrl);
    void Disconnect();
    inline UA_ClientState State() {
        UA_ClientState a = UA_Client_getState(m_Client);
        return a;
    }

    QString ReadNode(uint8_t eNamespace, const QString &eNodeIdString);

    template <typename t> void WriteNode(uint8_t eNameSpace, const QString &eNodeIdString, t value) {
        std::unique_ptr<char> temp(new char[eNodeIdString.size() + 1]);
        strncpy(temp.get(), eNodeIdString.toStdString().c_str(), eNodeIdString.size() + 1);
        UA_NodeId nodeId = UA_NODEID_STRING(eNameSpace, temp.get());

        // TODO: assign value to v before writing it, otherwise undefined behavior (server probably will crash).
        UA_Variant v;

        UA_Variant_setScalar(&v, &value, &UA_TYPES[UA_TYPES_BOOLEAN]);

        UA_Client_writeValueAttribute(m_Client, nodeId, &v);
    }

private:
    void MaintainConnection();
    void InitClient();

    UA_Client *m_Client = nullptr;
    std::mutex m_ClientLock;
    std::unique_ptr<QTimer> m_Timer_MaintainConnection;
    std::atomic<bool> m_MaintenanceInProgress = false;
};

#endif // SMG_ADAPTERS_OPCUACLIENT_H
