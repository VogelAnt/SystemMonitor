#ifndef OPCUACLIENT_H
#define OPCUACLIENT_H
#include <mockserver/Module.h>
#include <mockserver/System.h>
#include <open62541.h>

#include <QObject>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iterator>

class OpcuaClient : public QObject {
    Q_OBJECT

public:
    OpcuaClient();
    void ReadModuleState();

signals:
    /**
     * @brief sendModuleState
     *
     * @param nodevalues
     * State value as (Name, Value) pair
     */
    void sendModuleState(std::map<std::string, std::string> nodevalues);

public:
    smart4i::System *smart4iSystem;
    UA_Client *client;
    smart4i::Module *SuperTrak;
    smart4i::Module *Assembly;
    smart4i::Module *Labeling;
};

#endif // OPCUACLIENT_H
