

#include "OpcuaClient.h"

#include "smgCommon.h"

#include <iostream>
#include <memory>
#include <string>

using namespace std;
void StateCallback(UA_Client * /*unused*/, UA_ClientState eState);
QString VariantToString(const UA_Variant &eValue);
QString local_url;
UA_ClientState local_state;

OpcuaClient::OpcuaClient(QObject *parent) : QObject(parent) {
    InitClient();
}

OpcuaClient::~OpcuaClient() {
    m_Timer_MaintainConnection.reset();
    UA_Client_disconnect(m_Client);
    UA_Client_delete(m_Client);
}

UA_StatusCode OpcuaClient::Connect(const QString &eUrl) {
    local_url = eUrl;
    lock_guard guard(m_ClientLock);
    UA_StatusCode status = UA_Client_connect(m_Client, eUrl.toStdString().c_str());
    if (status == UA_STATUSCODE_GOOD) {
        m_Timer_MaintainConnection = std::make_unique<QTimer>();
        connect(m_Timer_MaintainConnection.get(), &QTimer::timeout, this, &OpcuaClient::MaintainConnection);
        m_Timer_MaintainConnection->start(1000); // check connection every second
        return status;
    }
    throw runtime_error(smg_FileLineMessage(": Could not connect to " + eUrl.toStdString() + "."));
}

void OpcuaClient::Disconnect() {
    lock_guard guard(m_ClientLock);
    UA_StatusCode status = UA_Client_disconnect(m_Client);
    if (status != UA_STATUSCODE_GOOD) {
        cerr << "Some error occured while disconnecting from " + local_url.toStdString() + "." << endl;
        UA_Client_delete(m_Client);
        InitClient();
    }
    if (m_Timer_MaintainConnection != nullptr) {
        disconnect(m_Timer_MaintainConnection.get());
        m_Timer_MaintainConnection.reset();
    }
    local_url = "";
}

QString OpcuaClient::ReadNode(uint8_t eNamespace, const QString &eNodeIdString) {
    // create node id structure to read from
    unique_ptr<char> temp(new char[eNodeIdString.size() + 1]);
    strncpy(temp.get(), eNodeIdString.toStdString().c_str(), eNodeIdString.size() + 1);
    UA_NodeId nodeId = UA_NODEID_STRING(eNamespace, temp.get());

    // create structure to read into
    UA_Variant value;
    UA_Variant_init(&value);
    UA_Client_readValueAttribute(m_Client, nodeId, &value);

    return VariantToString(value);
}

template <typename t> void OpcuaClient::WriteNode(uint8_t eNamespace, const QString &eNodeIdString, t value) {
    unique_ptr<char> temp(new char[eNodeIdString.size() + 1]);
    strncpy(temp.get(), eNodeIdString.toStdString().c_str(), eNodeIdString.size() + 1);
    UA_NodeId nodeId = UA_NODEID_STRING(eNamespace, temp.get());

    // TODO: assign value to v before writing it, otherwise undefined behavior (server probably will crash).
    UA_Variant v = value;
    UA_Variant_init(&v);

    UA_Client_writeValueAttribute(m_Client, nodeId, &v);
}

void OpcuaClient::InitClient() {
    m_Client = UA_Client_new();
    auto config = UA_Client_getConfig(m_Client);
    UA_ClientConfig_setDefault(config);
    config->stateCallback = StateCallback;
}

void OpcuaClient::MaintainConnection() {
    if (m_MaintenanceInProgress) { return; }

    m_MaintenanceInProgress = true;
    auto const state = State();
    if (UA_CLIENTSTATE_DISCONNECTED == state) {
        smg_FileLineMessage("Disconnection detected.");
        if (!local_url.isEmpty()) {
            try {
                Connect(local_url);
            } catch (exception &e) { cout << e.what() << endl; }
        }
    }
    m_MaintenanceInProgress = false;
}

void StateCallback(UA_Client * /*unused*/, UA_ClientState eState) {
    local_state = eState;
    cout << eState << endl;
    switch (eState) {
    case UA_CLIENTSTATE_CONNECTED:
        cout << "Connected to " + local_url.toStdString() + "." << endl;
        break;
    case UA_CLIENTSTATE_DISCONNECTED:
        cout << "Disconnected from " + local_url.toStdString() + "." << endl;
        break;
    default:
        break;
    }
}

QString VariantToString(const UA_Variant &eValue) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
    if (eValue.type->typeName == UA_TYPES[UA_TYPES_INT32].typeName) { //
        return QString::number(*(UA_Int32 *)eValue.data);
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
    if (eValue.type->typeName == UA_TYPES[UA_TYPES_STRING].typeName) { return ((char const *)(((UA_String *)eValue.data)->data)); }
}
