#ifndef OPCUACLIENT_H
#define OPCUACLIENT_H
#include <mockserver/Module.h>
#include <mockserver/System.h>
#include <open62541.h>

#include <QObject>

class OpcuaClient : public QObject {
    Q_OBJECT
public:
    OpcuaClient();

    void ReadModuleState();

    smart4i::System *smart4iSystem;
    UA_Client *client;
    smart4i::Module *SuperTrak;
    smart4i::Module *Assembly;
    smart4i::Module *Labeling;
};

#endif // OPCUACLIENT_H
