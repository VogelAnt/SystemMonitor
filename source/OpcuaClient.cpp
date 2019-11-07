#include "OpcuaClient.h"
#include <mockserver/Module.h>
#include <mockserver/Skill.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

OpcuaClient::OpcuaClient() {
    using smart4i::Module;
    using smart4i::Skill;

    client = UA_Client_new(UA_ClientConfig_default);
    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://localhost:4840");
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        exit(1);
    }

    // system setup
    smart4iSystem = new smart4i::System("::AsGlobalPV", 6);

    SuperTrak = new Module("gSuperTrak", smart4iSystem, 6);
    const size_t NUMBER_OF_SHUTTLES = 6;
    std::vector<Skill *> moveShuttle;
    auto checkParking = new Skill("checkParking", SuperTrak, 6);
    checkParking->AddOutputParameter("shuttleId");
    for (size_t i = 1; i <= NUMBER_OF_SHUTTLES; ++i) {
        auto shuttleI = new Skill("moveShuttle[" + std::to_string(i) + "]", SuperTrak, 6);
        shuttleI->AddInputParameter("finalTarget");
    }

    Assembly = new Module("gAssemblyModule", smart4iSystem, 6);
    auto provideCup = new Skill("provideCup", Assembly, 6);
    provideCup->AddInputParameter("cupType");
    auto providePellet = new Skill("providePellet", Assembly, 6);
    providePellet->AddInputParameter("pelletType");

    Labeling = new Module("gLabelingModule", nullptr, 2);
    Skill label("label", Labeling, 2);

    // use system setup to get the names of the nodes easily
    std::string supertrakOperationalState = SuperTrak->StateMachine.FullName + ".operationalState";
    std::cout << supertrakOperationalState << std::endl;
}

void OpcuaClient::ReadModuleState() {
    // use system setup to get the names of the nodes easily
    std::string supertrakOperationalState = SuperTrak->StateMachine.FullName + ".operationalState";
    std::cout << supertrakOperationalState << std::endl;

    UA_Variant value; /* Variants can hold scalar values and arrays of any type */
    UA_Variant_init(&value);

    UA_StatusCode retval;
    UA_NodeId nodeId;

    std::unique_ptr<char> temp(new char[1000]);
    strncpy(temp.get(), supertrakOperationalState.c_str(), supertrakOperationalState.size() + 1);
    /* NodeId of the variable holding the current time */
    nodeId = UA_NODEID_STRING(6, temp.get());
    retval = UA_Client_readValueAttribute(client, nodeId, &value);
    std::cout << "test node reading " << ((UA_String *)value.data)->data << std::endl;

    std::string assemblyOperationalState = Assembly->StateMachine.FullName + ".operationalState";
    std::cout << assemblyOperationalState << std::endl;

    temp.reset(new char[1000]);
    strncpy(temp.get(), assemblyOperationalState.c_str(), assemblyOperationalState.size() + 1);
    /* NodeId of the variable holding the current time */
    nodeId = UA_NODEID_STRING(6, temp.get());
    retval = UA_Client_readValueAttribute(client, nodeId, &value);
    std::cout << "test node reading " << ((UA_String *)value.data)->data << std::endl;

    std::string labelingOperationalState = Labeling->StateMachine.FullName + ".operationalState";
    std::cout << labelingOperationalState << std::endl;

    temp.reset(new char[1000]);
    strncpy(temp.get(), labelingOperationalState.c_str(), labelingOperationalState.size() + 1);
    /* NodeId of the variable holding the current time */
    nodeId = UA_NODEID_STRING(2, temp.get());
    retval = UA_Client_readValueAttribute(client, nodeId, &value);
    std::cout << "test node reading " << ((UA_String *)value.data)->data << std::endl;
}
