#ifndef OPCUACLIENT_H
#define OPCUACLIENT_H
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
     * Send (Name, State) pair of respective Module
     */
    void SendModuleState(std::map<std::string, std::string> nodevalues);

    /**
     * @brief sendSkillStateAssembly
     * Send (Name, State) pair of respective Skills
     * @param nodevalues
     */

public:
};

#endif // OPCUACLIENT_H
