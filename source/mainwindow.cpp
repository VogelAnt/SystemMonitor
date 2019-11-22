#include "mainwindow.h"
#include "OpcuaClient.h"
#include "SkillListWidget.h"
#include "orderinformation.h"
#include "redisclient.h"
#include "ui_mainwindow.h"

using Redistorium::Redis;
using Redistorium::Reply::ReplyElement;
// TODO: SeedSupply hat UA_String* als node identifier Typ
// TODO: SuperTrak und Assembly haben UA_int32 als Identifier

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->tabWidget->clear();
    QTimer *t_order_page = new QTimer();
    /** OPCUA CLIENTS FOR EACH MODULE
      */
    // save function (load file for addresses) so you don't have to enter all manually again
    // AssemblyIP 192.168.0.5:4840
    UA_Client* uaClientAssembly = UA_Client_new(UA_ClientConfig_default);
    UA_Client_connect(uaClientAssembly, "opc.tcp://192.168.0.5:4840");

    // SuperTrakIP 192.168.0.5:4840
    UA_Client* uaClientST = UA_Client_new(UA_ClientConfig_default);
    UA_Client_connect(uaClientST, "opc.tcp://192.168.0.5:4840");

    // LabelingIP 192.168.0.100:4840
    UA_Client* uaClientLabeling = UA_Client_new(UA_ClientConfig_default);
    UA_Client_connect(uaClientLabeling, "opc.tcp://192.168.0.100:4840");

//    UA_Client* uaClientImageRecognition = UA_Client_new(UA_ClientConfig_default);
//    UA_Client_connect(uaClientImageRecognition, "opc.tcp://localhost:4840");
//    UA_Client* uaClientOutfeed = UA_Client_new(UA_ClientConfig_default);
//    UA_Client_connect(uaClientOutfeed, "opc.tcp://localhost:4840");
//    UA_Client* uaClientSealing = UA_Client_new(UA_ClientConfig_default);
//    UA_Client_connect(uaClientSealing, "opc.tcp://localhost:4840");
//    UA_Client* uaClientSeedSupply = UA_Client_new(UA_ClientConfig_default);
//    UA_Client_connect(uaClientSeedSupply, "opc.tcp://192.168.0.180:4840");

    /** Module STL maps */
    // you give the modulewidget the ModuleMap as well as the Skill Map, create the SkillListWidget within the Module itself
    std::map<char*, char*> DisplayName_NodeId_Assembly;
    DisplayName_NodeId_Assembly["Assembly"]  = "::AsGlobalPV:gAssemblyModule.state.stateMachine.operationalState";
    std::map<char*, char*> DisplayName_NodeId_AssemblySkills;
    DisplayName_NodeId_AssemblySkills["Provide Cup"]  = "::AsGlobalPV:gAssemblyModule.skill.provideCup.state.stateMachine.operationalState";
    DisplayName_NodeId_AssemblySkills["Provide Pellet"]  = "::AsGlobalPV:gAssemblyModule.skill.providePellet.state.stateMachine.operationalState";

    std::map<char*, char*> DisplayName_NodeId_ST;
    DisplayName_NodeId_ST["SuperTrak"]  = "::AsGlobalPV:gSuperTrak.state.stateMachine.operationalState";
    std::map<char*, char*> DisplayName_NodeId_STSkills;
    DisplayName_NodeId_STSkills["checkParking"]  = "::AsGlobalPV:gSuperTrak.skill.checkParking.state.stateMachine.operationalState";
    DisplayName_NodeId_STSkills["moveShuttle1"]  = "::AsGlobalPV:gSuperTrak.skill.moveShuttle[1].state.stateMachine.operationalState";
    DisplayName_NodeId_STSkills["moveShuttle2"]  = "::AsGlobalPV:gSuperTrak.skill.moveShuttle[2].state.stateMachine.operationalState";
    DisplayName_NodeId_STSkills["moveShuttle3"]  = "::AsGlobalPV:gSuperTrak.skill.moveShuttle[3].state.stateMachine.operationalState";
    DisplayName_NodeId_STSkills["moveShuttle4"]  = "::AsGlobalPV:gSuperTrak.skill.moveShuttle[4].state.stateMachine.operationalState";
    DisplayName_NodeId_STSkills["moveShuttle5"]  = "::AsGlobalPV:gSuperTrak.skill.moveShuttle[5].state.stateMachine.operationalState";
    DisplayName_NodeId_STSkills["moveShuttle6"]  = "::AsGlobalPV:gSuperTrak.skill.moveShuttle[6].state.stateMachine.operationalState";

    std::map<char*, char*> DisplayName_NodeId_Labeling;
    DisplayName_NodeId_ST["Labeling"]  = "gLabelingModule.state.stateMachine.operationalState";
    std::map<char*, char*> DisplayName_NodeId_LabelingSkills;
    DisplayName_NodeId_LabelingSkills["Label"]  = "gLabelingModule.skill.label.state.stateMachine.operationalState";

//    // Human Assembly
//    std::map<char*, char*> DisplayName_NodeId_HumanAssembly;
//    DisplayName_NodeId_ST["Human Assembly"]  = "mi5.Human-Assembly.state.stateMachine.operationalState";
//    std::map<char*, char*> DisplayName_NodeId_HumanAssemblySkills;
//    DisplayName_NodeId_HumanAssemblySkills["assemble"]  = "mi5.Human-Assembly.skill.assemble.state.stateMachine.operationalState";

//    // image recognition
//    std::map<char*, char*> DisplayName_NodeId_ImageRecognition;
//    DisplayName_NodeId_ST["ImageRecognition"]  = "ImageRecognitionModule.state.stateMachine.operationalState";
//    std::map<char*, char*> DisplayName_NodeId_ImageRecognitionSkills;
//    DisplayName_NodeId_ImageRecognitionSkills["recognize"]  = "ImageRecognitionModule.skill.recognize.state.stateMachine.operationalState";
//    DisplayName_NodeId_ImageRecognitionSkills["findSeed"]  = "ImageRecognitionModule.skill.findSeed.state.stateMachine.operationalState";

//    // Outfeed
//    std::map<char*, char*> DisplayName_NodeId_Outfeed;
//    DisplayName_NodeId_ST["Outfeed"]  = "NODE_ID_UNKNOWN.state.stateMachine.operationalState";
//    std::map<char*, char*> DisplayName_NodeId_OutfeedSkills;
//    DisplayName_NodeId_OutfeedSkills["takeout"]  = "NODE_ID_UNKNOWN.skill.takeout.state.stateMachine.operationalState";

//    // Sealing
//    std::map<char*, char*> DisplayName_NodeId_Sealing;
//    DisplayName_NodeId_ST["Sealing"]  = "StOpcCom:opcComSealingModule.state.stateMachine.operationalState";
//    std::map<char*, char*> DisplayName_NodeId_SealingSkills;
//    DisplayName_NodeId_SealingSkills["sealing"]  = "StOpcCom:opcComSealingModule.skill.sealing.state.stateMachine.operationalState";

    // Seed supply
    std::map<char*, char*> DisplayName_NodeId_SeedSupply;
    DisplayName_NodeId_ST["Labeling"]  = "AsGlobalPV:opcComMitsubishi.state.stateMachine.operationalState";
    std::map<char*, char*> DisplayName_NodeId_SeedSupplySkills;
    DisplayName_NodeId_SeedSupplySkills["moveToSTHomePosition"]  = "AsGlobalPV:opcComMitsubishi.skill.moveToSTHomePosition.state.stateMachine.operationalState";
    DisplayName_NodeId_SeedSupplySkills["provideItemFromSTToWelding"]  = "AsGlobalPV:opcComMitsubishi.skill.provideItemFromSTToWelding.state.stateMachine.operationalState";
    DisplayName_NodeId_SeedSupplySkills["provideItemFromWeldingToST"]  = "AsGlobalPV:opcComMitsubishi.skill.provideItemFromWeldingToST.state.stateMachine.operationalState";
    DisplayName_NodeId_SeedSupplySkills["provideItemToST"]  = "AsGlobalPV:opcComMitsubishi.skill.provideItemToST.state.stateMachine.operationalState";
    DisplayName_NodeId_SeedSupplySkills["provideItemToWelding"]  = "AsGlobalPV:opcComMitsubishi.skill.provideItemToWelding.state.stateMachine.operationalState";
    DisplayName_NodeId_SeedSupplySkills["releaseItemToST"]  = "AsGlobalPV:opcComMitsubishi.skill.releaseItemToST.state.stateMachine.operationalState";

    // TODO: create a new ModuleWidgetClass, this ModuleWidgetClass communicating via OPCUA directly
    // TODO: Inside the ModuleWidget create a SkillListWidget containing from which we pass on the skills
    SkillListWidget *AssemblyTab = new SkillListWidget(uaClientAssembly, DisplayName_NodeId_AssemblySkills, 6, ui->tabWidget);
    SkillListWidget *STTab = new SkillListWidget(uaClientST, DisplayName_NodeId_STSkills, 6, ui->tabWidget);
    SkillListWidget *LabelingTab = new SkillListWidget(uaClientLabeling, DisplayName_NodeId_LabelingSkills, 2, ui->tabWidget);
//    SkillListWidget *HumanAssemblyTab = new SkillListWidget(uaClientHumanAssembly, DisplayName_NodeId_HumanAssemblySkills, 6, ui->tabWidget);
//    SkillListWidget *ImageRecognitionTab = new SkillListWidget(uaClientImageRecognition, DisplayName_NodeId_ImageRecognitionSkills, 6, ui->tabWidget);
//    SkillListWidget *OutfeedTab = new SkillListWidget(uaClientOutfeed, DisplayName_NodeId_OutfeedSkills, 6, ui->tabWidget);
//    SkillListWidget *SealingTab = new SkillListWidget(uaClientSealing, DisplayName_NodeId_SealingSkills, 6, ui->tabWidget);
//    SkillListWidget *SeedSupplyTab = new SkillListWidget(uaClientSeedSupply, DisplayName_NodeId_SeedSupplySkills, 6, ui->tabWidget);

    ui->tabWidget->addTab(AssemblyTab,"Assembly");
    ui->tabWidget->addTab(STTab,"SuperTrak");
    ui->tabWidget->addTab(LabelingTab, "Labeling");

//    ui->tabWidget->addTab(HumanAssemblyTab, "HumanAssembly");
//    ui->tabWidget->addTab(ImageRecognitionTab, "ImageRecognition");
//    ui->tabWidget->addTab(OutfeedTab, "Outfeed");
//    ui->tabWidget->addTab(SealingTab, "Sealing");
//    ui->tabWidget->addTab(SeedSupplyTab, "Seed Supply");

    m_TabStyle = new TabStyle_HorizontalText();
    ui->tabWidget->tabBar()->setStyle(m_TabStyle);

//    ModuleStateTabWidget *ModuleStateTab = new ModuleStateTabWidget();

//    m_layout = new QHBoxLayout();
//    m_layout->addWidget(ui->tableWidget);
//    m_layout->addWidget(ModuleStateTab);
//    setLayout(m_layout);
//    ui->tabWidget->clear();

    m_RedisClient = new RedisClient(this);

//    connect(m_OpcuaClient, &OpcuaClient::SendAssemblySkillState ,AssemblyTab, &SkillListWidget::on_SendAssemblySkillState);
//    connect(m_OpcuaClient, &OpcuaClient::SendLabelingSkillState ,LabelingTab, &SkillListWidget::on_SendLabelingSkillState);
//    connect(m_OpcuaClient, &OpcuaClient::SendSuperTrakSkillState ,SuperTrakTab, &SkillListWidget::on_SendSuperTrakSkillState);

//    ui->tabWidget->addTab(LabelingTab, "LabelingModule");
//    ui->tabWidget->addTab(SuperTrakTab, "SuperTrakTab");
//    for (int i = 0 ;i<3;++i) {
//        ui->tabWidget->tabBar()->setTabTextColor(i, QColor(Qt::yellow));
//    }


//     set up Table Widget move this to an outside function ?
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setSortingEnabled(false);
    headerColumns << "OrderID"
                  << "Priority"
                  << "FirstName"
                  << "LastName";
    ui->tableWidget->setHorizontalHeaderLabels(headerColumns);
    ui->tableWidget->verticalHeader()->setVisible(false);

    connect(this, &MainWindow::SendCommand, m_RedisClient, &RedisClient::SendCommand);
    connect(m_RedisClient, &RedisClient::ReceivedJSONString, m_RedisClient, &RedisClient::on_ReadFromJsonString);
    connect(m_RedisClient, &RedisClient::SubscriptionMessage, this, &MainWindow::on_SubscriptionMessage);
    connect(m_RedisClient, &RedisClient::ParsedJson, this, &MainWindow::on_MakeOrderTable);
    connect(m_RedisClient, &RedisClient::SubscriptionMessage, [](QString eChannel, QString eMessage) {
        // std::cout << "New subscription message on channel \"" << eChannel.toStdString() << "\": " << eMessage.toStdString() << std::endl;
    });
    connect(t_order_page, &QTimer::timeout, [&]() { MainWindow::MockOrderPage(); }); // only for testing purposes
    connect(this, &MainWindow::ReceivedNewSubscription, m_RedisClient, &RedisClient::on_ReadFromJsonString);
    // this is where on:send Moduel state is actually connected !!
    connect(m_OpcuaClient, &OpcuaClient::SendModuleState, this, &MainWindow::on_SendModuleState);
    t_order_page->start(3000);

    m_RedisClient->m_Redis->SUBSCRIBE("OrderPage");

    ReplyElement orderPage_data_received = m_RedisClient->m_Redis->GET("DataOrderPage");
    if (orderPage_data_received.GetBulkString().has_value()) {
        qDebug() << "This is what OrderPage_data looks like" << orderPage_data_received.GetBulkString().value();
        emit m_RedisClient->ReceivedJSONString(orderPage_data_received.GetBulkString());
    }
}

void MainWindow::on_SendModuleState(std::map<std::string, std::string> ModuleSkillMap){
    int tab_index = 0;
    std::string default_node_value = "17";
    for (std::map<std::string, std::string>::iterator it = ModuleSkillMap.begin(); it != ModuleSkillMap.end(); ++it){
        for (int i = 0; i < default_node_value.length(); ++i){
            if((it->second[i] - '0') != (default_node_value[i] - '0')){
                std::cout << "setting red" << std::endl;
                ui->tabWidget->tabBar()->setTabTextColor(tab_index, QColor(Qt::red));
            }
            else {
            std::cout << "setting green " <<std:: endl;
            ui->tabWidget->tabBar()->setTabTextColor(tab_index, QColor(Qt::green));
            }
        }
    ++tab_index;
    }
}

void MainWindow::on_MakeOrderTable(nlohmann::json eParsed) {
    ui->tableWidget->setSortingEnabled(false);
    order = new OrderInformation(
        QString::fromStdString(std::string(eParsed["DataOrderPage"][0]["orderID"])),
        QString::fromStdString(std::string(eParsed["DataOrderPage"][0]["priority"])),
        QString::fromStdString(std::string(eParsed["DataOrderPage"][0]["firstName"])),
        QString::fromStdString(std::string(eParsed["DataOrderPage"][0]["lastName"])));
    //    order->SendOrderToRedis();
    ui->tableWidget->setRowCount(eParsed["DataOrderPage"].size() + m_OrderNumber);
    ui->tableWidget->setItem(m_OrderNumber, 0, new QTableWidgetItem(order->GetOrderID()));
    ui->tableWidget->setItem(m_OrderNumber, 1, new QTableWidgetItem(order->GetOrderPriority()));
    ui->tableWidget->setItem(m_OrderNumber, 2, new QTableWidgetItem(order->GetCustomerFirstName()));
    ui->tableWidget->setItem(m_OrderNumber, 3, new QTableWidgetItem(order->GetCustomerLastName()));
    ++m_OrderNumber;
    // You can't sort while still setting...
    ui->tableWidget->horizontalHeader()->setSortIndicatorShown(true);
    ui->tableWidget->horizontalHeader()->setSortIndicator(0, Qt::DescendingOrder);
    ui->tableWidget->setSortingEnabled(true);
}

MainWindow::~MainWindow() {
    delete ui;
    delete m_RedisClient;
    delete m_TabStyle;
    delete m_OpcuaClient;
    if (prioritybox != nullptr) delete prioritybox;
}

void MainWindow::on_SubscriptionMessage(QString eChannel, QString eMessage) {
    // qDebug() << "Received Message from subscribed channel " << eChannel << ": \n" << eMessage;
    std::optional<QString> systemMonitor_received = eMessage;
    emit ReceivedNewSubscription(systemMonitor_received);
}

void MainWindow::MockOrderPage() {
    nlohmann::json OrderPage_data = m_RedisClient->make_json_orderpage();
    QString OrderPage_data_stringified = m_RedisClient->stringify_json(OrderPage_data);
    // why are these two different Keys ?
    m_RedisClient->m_Redis->SET("DataOrderPage", OrderPage_data_stringified);
    m_RedisClient->m_Redis->PUBLISH("OrderPage", OrderPage_data_stringified);
}

void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column){
    if (column != 1) {
        ui->tableWidget->item(row, column)->setFlags(ui->tableWidget->item(row,column)->flags() & ~Qt::ItemIsEditable);
    }
    else {
        dialog = new QInputDialog();
        std:: cout << " Clicked Row: " << row << "Column: " << column << std::endl;
        int n = QInputDialog::getInt(this, "Alter OrderPriority", "Confirm by entering a different value");
        ui->tableWidget->setItem(row, column, new QTableWidgetItem(QString::number(n)));
        // make the json
        nlohmann::json ChangedOrder_json;
        auto order_arr = nlohmann::json::array();
        nlohmann::json order_json{{"orderID", ui->tableWidget->item(row, 0)->text().toStdString()},
                                  {"priority", std::to_string(n)},
                                  {"firstName", ui->tableWidget->item(row, 2)->text().toStdString()},
                                  {"lastName", ui->tableWidget->item(row, 3)->text().toStdString()}};
        order_arr.push_back(order_json);
        ChangedOrder_json["DataOrderPage"] = order_arr;
        // stringify it
        std::string tempstdstr = ChangedOrder_json.dump();
        QString tempqstring = tempstdstr.c_str();
        tempqstring.replace("\"", "\\\"");
        tempqstring = QString("\"" + tempqstring + "\"");
        // make a QStringList
//        m_RedisClient->m_Redis->LPUSH()
        m_RedisClient->m_Redis->SET("AlteredDataOrderPage", tempqstring);
        m_RedisClient->m_Redis->PUBLISH("AltOrderPage", tempqstring);
    }
}

void MainWindow::on_tableWidget_cellClicked(int row, int column){
    if (column != 1) {
        ui->tableWidget->item(row, column)->setFlags(ui->tableWidget->item(row,column)->flags() & ~Qt::ItemIsEditable);
    }
}
