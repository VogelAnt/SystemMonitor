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

class OpcuaClient : public QObject {
    Q_OBJECT
public:
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

private:
    void MaintainConnection();
    void InitClient();

    UA_Client *m_Client = nullptr;
    std::mutex m_ClientLock;
    std::unique_ptr<QTimer> m_Timer_MaintainConnection;
    std::atomic<bool> m_MaintenanceInProgress = false;
};

#endif // SMG_ADAPTERS_OPCUACLIENT_H
