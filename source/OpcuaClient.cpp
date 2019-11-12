//#include "OpcuaClient.h"
//#include <mockserver/Module.h>
//#include <mockserver/Skill.h>


//// TODO: Add Address of Server as QString
//OpcuaClient::OpcuaClient() {
//    using smart4i::Module;
//    using smart4i::Skill;

//    client = UA_Client_new(UA_ClientConfig_default);
//    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://192.168.100.25");
//    if (retval != UA_STATUSCODE_GOOD) {
//        UA_Client_delete(client);
//        exit(1);
//    }

//    // system setup
//    smart4iSystem = new smart4i::System("::AsGlobalPV", 6);

//    SuperTrak = new Module("gSuperTrak", smart4iSystem, 6);
//    const size_t NUMBER_OF_SHUTTLES = 6;
//    std::vector<Skill *> moveShuttle;
//    auto checkParking = new Skill("checkParking", SuperTrak, 6);
//    checkParking->AddOutputParameter("shuttleId");
//    for (size_t i = 1; i <= NUMBER_OF_SHUTTLES; ++i) {
//        auto shuttleI = new Skill("moveShuttle[" + std::to_string(i) + "]", SuperTrak, 6);
//        shuttleI->AddInputParameter("finalTarget");
//    }

//    Assembly = new Module("gAssemblyModule", smart4iSystem, 6);
//    auto provideCup = new Skill("provideCup", Assembly, 6);
//    provideCup->AddInputParameter("cupType");
//    auto providePellet = new Skill("providePellet", Assembly, 6);
//    providePellet->AddInputParameter("pelletType");

//    Labeling = new Module("gLabelingModule", nullptr, 2);
//    Skill label("label", Labeling, 2);

//    // TODO: Remove Double Entries ???
//    // use system setup to get the names of the nodes easily
//    std::string supertrakOperationalState = SuperTrak->StateMachine.FullName + ".operationalState";
//    std::cout << supertrakOperationalState << std::endl;
//}

////void OpcuaClient::ReadModuleState() {
////    std::map<std::string, std::string> ModuleValuePair;

////    // use system setup to get the names of the nodes easily
////    std::string supertrakOperationalState = SuperTrak->StateMachine.FullName + ".operationalState";
////    std::cout << supertrakOperationalState << std::endl;

////    UA_Variant value; /* Variants can hold scalar values and arrays of any type */
////    UA_Variant_init(&value);

////    UA_StatusCode retval;
////    UA_NodeId nodeId;

////    // Supertrak
////    std::unique_ptr<char> temp(new char[1000]);
////    strncpy(temp.get(), supertrakOperationalState.c_str(), supertrakOperationalState.size() + 1);
////    /* NodeId of the variable holding the current time */
////    nodeId = UA_NODEID_STRING(6, temp.get());
////    retval = UA_Client_readValueAttribute(client, nodeId, &value);
////    std::cout << "SuperTRAK node reading " << ((UA_String *)value.data)->data << std::endl;
////    unsigned char * st_node = ((UA_String *)value.data)->data;
////    std::string st( reinterpret_cast< char const* >(st_node));
////    std::cout << st << std::endl;
////    ModuleValuePair.insert(std::make_pair("SuperTRAK", st));

////    // Assembly
////    std::string assemblyOperationalState = Assembly->StateMachine.FullName + ".operationalState";
////    std::cout << assemblyOperationalState << std::endl;
////    temp.reset(new char[1000]);
////    strncpy(temp.get(), assemblyOperationalState.c_str(), assemblyOperationalState.size() + 1);
////    /* NodeId of the variable holding the current time */
////    nodeId = UA_NODEID_STRING(6, temp.get());
////    retval = UA_Client_readValueAttribute(client, nodeId, &value);
////    std::cout << "Assembly node reading " << ((UA_String *)value.data)->data << std::endl;
////    unsigned char * ass_node = ((UA_String *)value.data)->data;
////    std::string ass( reinterpret_cast< char const* >(ass_node) );
////    std::cout << ass << std::endl;
////    ModuleValuePair.insert(std::make_pair("Assembly Module", ass));

////    // labeling
////    std::string labelingOperationalState = Labeling->StateMachine.FullName + ".operationalState";
////    std::cout << labelingOperationalState << std::endl;
////    temp.reset(new char[1000]);
////    strncpy(temp.get(), labelingOperationalState.c_str(), labelingOperationalState.size() + 1);
////    /* NodeId of the variable holding the current time */
////    nodeId = UA_NODEID_STRING(2, temp.get());
////    retval = UA_Client_readValueAttribute(client, nodeId, &value);
////    std::cout << "Labeling node reading " << ((UA_String *)value.data)->data << std::endl;
////    unsigned char * lab_node = ((UA_String *)value.data)->data;
////    std::string lab( reinterpret_cast< char const* >(lab_node) ) ;
////    std::cout << lab << std::endl;
////    ModuleValuePair.insert(std::make_pair("Labeling Module", lab));

////    // trigger
////    SendModuleState(ModuleValuePair);
////}


////void OpcuaClient::ReadSkillStateAssembly(){
////    UA_Variant value; /* Variants can hold scalar values and arrays of any type */
////    UA_Variant_init(&value);
////    UA_StatusCode retval;
////    UA_NodeId nodeId;

////    std::cout << "NOW IN READSKILLSTATEASSEMBLY" <<std::endl;

////    std::map<std::string, std::string> SkillStatePair;

////    // (OperationalState of Skills provided as a QStringList, nodeid)
////    std::string provideCupOperationalState = Assembly->SkillList.at(0)->StateMachine.FullName + ".operationalState";
////    std::cout << provideCupOperationalState << std::endl;
////    std::unique_ptr<char> temp(new char[1000]);
////    strncpy(temp.get(), provideCupOperationalState.c_str(), provideCupOperationalState.size() + 1);
////    /* NodeId of the variable holding the current time */
////    nodeId = UA_NODEID_STRING(6, temp.get());
////    retval = UA_Client_readValueAttribute(client, nodeId, &value);
////    std::cout << "ProvideCup-Skill reading " << ((UA_String *)value.data)->data << std::endl;
////    unsigned char * pc_node = ((UA_String *)value.data)->data;
////    std::string pc( reinterpret_cast< char const* >(pc_node));
////    std::cout << pc << std::endl;
////    // this part should be a function - cut down amount of space quite a bit
////    //this will be outside of the function
////    SkillStatePair.insert(std::make_pair("Provide Cup", pc));

////    std::string providePelletOperationalState = Assembly->SkillList.at(1)->StateMachine.FullName + ".operationalState";
////    std::cout << providePelletOperationalState << std::endl;
////    temp.reset(new char[1000]);
////    strncpy(temp.get(), providePelletOperationalState.c_str(), providePelletOperationalState.size() + 1);
////    /* NodeId of the variable holding the current time */
////    nodeId = UA_NODEID_STRING(6, temp.get());
////    retval = UA_Client_readValueAttribute(client, nodeId, &value);
////    std::cout << "ProvidePellet-Skill reading " << ((UA_String *)value.data)->data << std::endl;
////    unsigned char * pp_node = ((UA_String *)value.data)->data;
////    std::string pp( reinterpret_cast< char const* >(pp_node) ) ;
////    std::cout << pp << std::endl;
////    SkillStatePair.insert(std::make_pair("Provide Pellet", pp));
////    SendAssemblySkillState(SkillStatePair);
////}



////void OpcuaClient::ReadSkillStateLabeling(){
//////    UA_Variant value; /* Variants can hold scalar values and arrays of any type */
//////    UA_Variant_init(&value);
//////    UA_StatusCode retval;
//////    UA_NodeId nodeId;

//////    std::map<std::string, std::string> SkillStatePair;

//////    // Labeling
//////    std::string labelOperationalState = Labeling->SkillList.at(0)->StateMachine.FullName + ".operationalState";
//////    std::cout << labelOperationalState << std::endl;
//////    std::unique_ptr<char> temp(new char[1000]);
//////    strncpy(temp.get(), labelOperationalState.c_str(), labelOperationalState.size() + 1);
//////    /* NodeId of the variable holding the current time */
//////    nodeId = UA_NODEID_STRING(2, temp.get());
//////    retval = UA_Client_readValueAttribute(client, nodeId, &value);
//////    std::cout << "Label-Skill reading " << ((UA_String *)value.data)->data << std::endl;
//////    unsigned char * lab_node = ((UA_String *)value.data)->data;
//////    std::string lab( reinterpret_cast< char const* >(lab_node));
//////    std::cout <<lab << std::endl;
//////    SkillStatePair.insert(std::make_pair("Label", lab));
//////    // sendSkillState
//////    SendSkillState(SkillStatePair);
////    std::cout << "ReadLabeling would have been called by now" << std::endl;
////}

////void OpcuaClient::ReadSkillStateSuperTrak(){
////    std::map<std::string, std::string> SkillStatePair;
////    // put this in function
////    UA_Variant value; /* Variants can hold scalar values and arrays of any type */
////    UA_Variant_init(&value);
////    UA_StatusCode retval;
////    UA_NodeId nodeId;

////    std::string checkParkingOperationalState = SuperTrak->SkillList.at(0)->StateMachine.FullName + ".operationalState";
////    std::cout << checkParkingOperationalState << std::endl;
////    std::unique_ptr<char> temp(new char[1000]);
////    strncpy(temp.get(), checkParkingOperationalState.c_str(), checkParkingOperationalState.size() + 1);
////    /* NodeId of the variable holding the current time */
////    nodeId = UA_NODEID_STRING(6, temp.get());
////    retval = UA_Client_readValueAttribute(client, nodeId, &value);
////    std::cout << "Checkparking-Skill reading " << ((UA_String *)value.data)->data << std::endl;
////    unsigned char * cp_node = ((UA_String *)value.data)->data;
////    std::string cp( reinterpret_cast< char const* >(cp_node));
////    std::cout << cp << std::endl;
////    SkillStatePair.insert(std::make_pair("Check Parking", cp));

////    std::string moveShuttle1OperationalState = SuperTrak->SkillList.at(1)->StateMachine.FullName + ".operationalState";
////    std::cout << moveShuttle1OperationalState << std::endl;
////    temp.reset(new char[1000]);
////    strncpy(temp.get(), moveShuttle1OperationalState.c_str(), moveShuttle1OperationalState.size() + 1);
////    /* NodeId of the variable holding the current time */
////    nodeId = UA_NODEID_STRING(6, temp.get());
////    retval = UA_Client_readValueAttribute(client, nodeId, &value);
////    std::cout << "moveShuttle1-Skill reading " << ((UA_String *)value.data)->data << std::endl;
////    unsigned char * ms1_node = ((UA_String *)value.data)->data;
////    std::string ms1( reinterpret_cast< char const* >(ms1_node) ) ;
////    std::cout << ms1 << std::endl;
////    SkillStatePair.insert(std::make_pair("Move Shuttle 1", ms1));

////    std::string moveShuttle2OperationalState = SuperTrak->SkillList.at(2)->StateMachine.FullName + ".operationalState";
////    std::cout << moveShuttle2OperationalState << std::endl;
////    temp.reset(new char[1000]);
////    strncpy(temp.get(), moveShuttle2OperationalState.c_str(), moveShuttle2OperationalState.size() + 1);
////    /* NodeId of the variable holding the current time */
////    nodeId = UA_NODEID_STRING(6, temp.get());
////    retval = UA_Client_readValueAttribute(client, nodeId, &value);
////    std::cout << "moveShuttle2-Skill reading " << ((UA_String *)value.data)->data << std::endl;
////    unsigned char * ms2_node = ((UA_String *)value.data)->data;
////    std::string ms2( reinterpret_cast< char const* >(ms2_node) ) ;
////    std::cout << ms2 << std::endl;
////    SkillStatePair.insert(std::make_pair("Move Shuttle 2", ms2));

////    std::string moveShuttle3OperationalState = SuperTrak->SkillList.at(3)->StateMachine.FullName + ".operationalState";
////    std::cout << moveShuttle3OperationalState << std::endl;
////    temp.reset(new char[1000]);
////    strncpy(temp.get(), moveShuttle3OperationalState.c_str(), moveShuttle3OperationalState.size() + 1);
////    /* NodeId of the variable holding the current time */
////    nodeId = UA_NODEID_STRING(6, temp.get());
////    retval = UA_Client_readValueAttribute(client, nodeId, &value);
////    std::cout << "moveShuttle3-Skill reading " << ((UA_String *)value.data)->data << std::endl;
////    unsigned char * ms3_node = ((UA_String *)value.data)->data;
////    std::string ms3( reinterpret_cast< char const* >(ms3_node) ) ;
////    std::cout << ms3 << std::endl;
////    SkillStatePair.insert(std::make_pair("Move Shuttle 3", ms3));

////    std::string moveShuttle4OperationalState = SuperTrak->SkillList.at(4)->StateMachine.FullName + ".operationalState";
////    std::cout << moveShuttle4OperationalState << std::endl;
////    temp.reset(new char[1000]);
////    strncpy(temp.get(), moveShuttle4OperationalState.c_str(), moveShuttle4OperationalState.size() + 1);
////    /* NodeId of the variable holding the current time */
////    nodeId = UA_NODEID_STRING(6, temp.get());
////    retval = UA_Client_readValueAttribute(client, nodeId, &value);
////    std::cout << "moveShuttle4-Skill reading " << ((UA_String *)value.data)->data << std::endl;
////    unsigned char * ms4_node = ((UA_String *)value.data)->data;
////    std::string ms4( reinterpret_cast< char const* >(ms4_node) ) ;
////    std::cout << ms4 << std::endl;
////    SkillStatePair.insert(std::make_pair("Move Shuttle 4", ms4));

////    std::string moveShuttle5OperationalState = SuperTrak->SkillList.at(5)->StateMachine.FullName + ".operationalState";
////    std::cout << moveShuttle5OperationalState << std::endl;
////    temp.reset(new char[1000]);
////    strncpy(temp.get(), moveShuttle5OperationalState.c_str(), moveShuttle5OperationalState.size() + 1);
////    /* NodeId of the variable holding the current time */
////    nodeId = UA_NODEID_STRING(6, temp.get());
////    retval = UA_Client_readValueAttribute(client, nodeId, &value);
////    std::cout << "moveShuttle5-Skill reading " << ((UA_String *)value.data)->data << std::endl;
////    unsigned char * ms5_node = ((UA_String *)value.data)->data;
////    std::string ms5( reinterpret_cast< char const* >(ms5_node) ) ;
////    std::cout << ms5 << std::endl;
////    SkillStatePair.insert(std::make_pair("Move Shuttle 5", ms5));

////    std::string moveShuttle6OperationalState = SuperTrak->SkillList.at(6)->StateMachine.FullName + ".operationalState";
////    std::cout << moveShuttle6OperationalState << std::endl;
////    temp.reset(new char[1000]);
////    strncpy(temp.get(), moveShuttle6OperationalState.c_str(), moveShuttle6OperationalState.size() + 1);
////    /* NodeId of the variable holding the current time */
////    nodeId = UA_NODEID_STRING(6, temp.get());
////    retval = UA_Client_readValueAttribute(client, nodeId, &value);
////    std::cout << "moveShuttle6-Skill reading " << ((UA_String *)value.data)->data << std::endl;
////    unsigned char * ms6_node = ((UA_String *)value.data)->data;
////    std::string ms6( reinterpret_cast< char const* >(ms6_node) ) ;
////    std::cout << ms6 << std::endl;
////    SkillStatePair.insert(std::make_pair("Move Shuttle 6", ms6));
////    SendSkillState(SkillStatePair);
////}
