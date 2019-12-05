#include "DeviceTabWidget.h"

DeviceTabWidget::DeviceTabWidget(QWidget *parent) : QTabWidget(parent){
    /** DEVICE CLIENTS
*/
//     save function (load file for addresses) so you don't have to enter all manually again
//     AssemblyIP 192.168.0.5:4840
        UA_Client* uaClientAssembly = UA_Client_new(UA_ClientConfig_default);
        UA_Client_connect(uaClientAssembly, "opc.tcp://localhost:4840");

        // SuperTrakIP 192.168.0.5:4840
        UA_Client* uaClientST = UA_Client_new(UA_ClientConfig_default);
        UA_Client_connect(uaClientST, "opc.tcp://localhost:4840");

        // LabelingIP 192.168.0.102:4840
        UA_Client* uaClientLabeling = UA_Client_new(UA_ClientConfig_default);
        UA_Client_connect(uaClientLabeling, "opc.tcp://localhost:4840");

//        UA_Client* uaClientImageRecognition = UA_Client_new(UA_ClientConfig_default);
//        UA_Client_connect(uaClientImageRecognition, "opc.tcp://localhost:4840");
//        UA_Client* uaClientOutfeed = UA_Client_new(UA_ClientConfig_default);
//        UA_Client_connect(uaClientOutfeed, "opc.tcp://localhost:4840");
//        UA_Client* uaClientSealing = UA_Client_new(UA_ClientConfig_default);
//        UA_Client_connect(uaClientSealing, "opc.tcp://localhost:4840");
//        UA_Client* uaClientSeedSupply = UA_Client_new(UA_ClientConfig_default);
//        UA_Client_connect(uaClientSeedSupply, "opc.tcp://192.168.0.180:4840");

        /** Module STL maps */
        // you give the modulewidget the ModuleMap as well as the Skill Map, create the DeviceWidget within the Module itself
        std::map<char*, char*> DisplayName_NodeId_Assembly;
        DisplayName_NodeId_Assembly["Assembly"]  = "::AsGlobalPV:gAssemblyModule.state.stateMachine.operationalState";
        std::map<char*, char*> DisplayName_NodeId_AssemblySkills;
        //    DisplayName_NodeId_AssemblySkills["Go Home"]  = "::AsGlobalPV:gAssemblyModule.skill.goHome.state.stateMachine.operationalState";
        DisplayName_NodeId_AssemblySkills["Provide Cup"]  = "::AsGlobalPV:gAssemblyModule.skill.provideCup.state.stateMachine.operationalState";
        DisplayName_NodeId_AssemblySkills["Provide Pellet"]  = "::AsGlobalPV:gAssemblyModule.skill.providePellet.state.stateMachine.operationalState";

        std::map<char*, char*> DisplayName_NodeId_ST;
        DisplayName_NodeId_ST["SuperTrak"]  = "::AsGlobalPV:gSuperTrak.state.stateMachine.operationalState";
        std::map<char*, char*> DisplayName_NodeId_STSkills;
        //    DisplayName_NodeId_STSkills["Outfeed"]  = "::AsGlobalPV:gSuperTrak.skill.Outfeed.state.stateMachine.operationalState";
        DisplayName_NodeId_STSkills["checkParking"]  = "::AsGlobalPV:gSuperTrak.skill.checkParking.state.stateMachine.operationalState";
        DisplayName_NodeId_STSkills["moveShuttle1"]  = "::AsGlobalPV:gSuperTrak.skill.moveShuttle[1].state.stateMachine.operationalState";
        DisplayName_NodeId_STSkills["moveShuttle2"]  = "::AsGlobalPV:gSuperTrak.skill.moveShuttle[2].state.stateMachine.operationalState";
        DisplayName_NodeId_STSkills["moveShuttle3"]  = "::AsGlobalPV:gSuperTrak.skill.moveShuttle[3].state.stateMachine.operationalState";
        DisplayName_NodeId_STSkills["moveShuttle4"]  = "::AsGlobalPV:gSuperTrak.skill.moveShuttle[4].state.stateMachine.operationalState";
        DisplayName_NodeId_STSkills["moveShuttle5"]  = "::AsGlobalPV:gSuperTrak.skill.moveShuttle[5].state.stateMachine.operationalState";
        DisplayName_NodeId_STSkills["moveShuttle6"]  = "::AsGlobalPV:gSuperTrak.skill.moveShuttle[6].state.stateMachine.operationalState";

        std::map<char*, char*> DisplayName_NodeId_Labeling;
        DisplayName_NodeId_Labeling["Labeling"]  = "gLabelingModule.state.stateMachine.operationalState";
        std::map<char*, char*> DisplayName_NodeId_LabelingSkills;
        DisplayName_NodeId_LabelingSkills["Label"]  = "gLabelingModule.skill.label.state.stateMachine.operationalState";

        // Human Assembly
//        std::map<char*, char*> DisplayName_NodeId_HumanAssembly;
//        DisplayName_NodeId_ST["Human Assembly"]  = "mi5.Human-Assembly.state.stateMachine.operationalState";
//        std::map<char*, char*> DisplayName_NodeId_HumanAssemblySkills;
//        DisplayName_NodeId_HumanAssemblySkills["assemble"]  = "mi5.Human-Assembly.skill.assemble.state.stateMachine.operationalState";

        // image recognition
//        std::map<char*, char*> DisplayName_NodeId_ImageRecognition;
//        DisplayName_NodeId_ST["ImageRecognition"]  = "ImageRecognitionModule.state.stateMachine.operationalState";
//        std::map<char*, char*> DisplayName_NodeId_ImageRecognitionSkills;
//        DisplayName_NodeId_ImageRecognitionSkills["recognize"]  = "ImageRecognitionModule.skill.recognize.state.stateMachine.operationalState";
//        DisplayName_NodeId_ImageRecognitionSkills["findSeed"]  = "ImageRecognitionModule.skill.findSeed.state.stateMachine.operationalState";

        // Outfeed
//        std::map<char*, char*> DisplayName_NodeId_Outfeed;
//        DisplayName_NodeId_ST["Outfeed"]  = "NODE_ID_UNKNOWN.state.stateMachine.operationalState";
//        std::map<char*, char*> DisplayName_NodeId_OutfeedSkills;
//        DisplayName_NodeId_OutfeedSkills["takeout"]  = "NODE_ID_UNKNOWN.skill.takeout.state.stateMachine.operationalState";

        // Sealing
//        std::map<char*, char*> DisplayName_NodeId_Sealing;
//        DisplayName_NodeId_ST["Sealing"]  = "StOpcCom:opcComSealingModule.state.stateMachine.operationalState";
//        std::map<char*, char*> DisplayName_NodeId_SealingSkills;
//        DisplayName_NodeId_SealingSkills["sealing"]  = "StOpcCom:opcComSealingModule.skill.sealing.state.stateMachine.operationalState";

//     Seed supply
//        std::map<char*, char*> DisplayName_NodeId_SeedSupply;
//        DisplayName_NodeId_SeedSupply["SeedSupply"]  = "::AsGlobalPV:opcComMitsubishi.state.stateMachine.operationalState";
//        std::map<char*, char*> DisplayName_NodeId_SeedSupplySkills;
//        DisplayName_NodeId_SeedSupplySkills["moveToSTHomePosition"]  = "::AsGlobalPV:opcComMitsubishi.skill.moveToSTHomePosition.state.stateMachine.operationalState";
//        DisplayName_NodeId_SeedSupplySkills["provideItemFromSTToWelding"]  = "::AsGlobalPV:opcComMitsubishi.skill.provideItemFromSTToWelding.state.stateMachine.operationalState";
//        DisplayName_NodeId_SeedSupplySkills["provideItemFromWeldingToST"]  = "::AsGlobalPV:opcComMitsubishi.skill.provideItemFromWeldingToST.state.stateMachine.operationalState";
//        DisplayName_NodeId_SeedSupplySkills["provideItemToST"]  = "::AsGlobalPV:opcComMitsubishi.skill.provideItemToST.state.stateMachine.operationalState";
//        DisplayName_NodeId_SeedSupplySkills["provideItemToWelding"]  = "::AsGlobalPV:opcComMitsubishi.skill.provideItemToWelding.state.stateMachine.operationalState";
//        DisplayName_NodeId_SeedSupplySkills["releaseItemToST"]  = "::AsGlobalPV:opcComMitsubishi.skill.releaseItemToST.state.stateMachine.operationalState";


        DeviceWidget *AssemblyTab = new DeviceWidget(uaClientAssembly, DisplayName_NodeId_Assembly, DisplayName_NodeId_AssemblySkills, 6, 0 , this);
        DeviceWidget *STTab = new DeviceWidget(uaClientST, DisplayName_NodeId_ST,DisplayName_NodeId_STSkills, 6, 1, this);
        DeviceWidget *LabelingTab = new DeviceWidget(uaClientLabeling, DisplayName_NodeId_Labeling,DisplayName_NodeId_LabelingSkills, 2,2, this);
//        DeviceWidget *HumanAssemblyTab = new DeviceWidget(uaClientHumanAssembly, DisplayName_NodeId_HumanAssemblySkills, 6, this);
//        DeviceWidget *ImageRecognitionTab = new DeviceWidget(uaClientImageRecognition, DisplayName_NodeId_ImageRecognitionSkills, 6, this);
//        DeviceWidget *OutfeedTab = new DeviceWidget(uaClientOutfeed, DisplayName_NodeId_OutfeedSkills, 6, this);
//        DeviceWidget *SealingTab = new DeviceWidget(uaClientSealing, DisplayName_NodeId_SealingSkills, 6, this);
//        DeviceWidget *SeedSupplyTab = new DeviceWidget(uaClientSeedSupply, DisplayName_NodeId_SeedSupply, DisplayName_NodeId_SeedSupplySkills, 6, 3, this);

        connect(AssemblyTab, &DeviceWidget::ChangeDeviceStatus, this, &DeviceTabWidget::on_ChangeDeviceStatus);
        connect(STTab, &DeviceWidget::ChangeDeviceStatus, this, &DeviceTabWidget::on_ChangeDeviceStatus);
        connect(LabelingTab, &DeviceWidget::ChangeDeviceStatus, this, &DeviceTabWidget::on_ChangeDeviceStatus);
    //    connect(SeedSupplyTab, &DeviceWidget::ChangeDeviceStatus, this, &MainWindow::on_ChangeDeviceStatus);
        tabBar()->setStyleSheet("font-size : 24px");
        addTab(AssemblyTab,"Assembly");
        addTab(STTab,"SuperTrak");
        addTab(LabelingTab, "Labeling");
//        addTab(SeedSupplyTab, "SeedSupply");
//        addTab(HumanAssemblyTab, "HumanAssembly");
//        addTab(ImageRecognitionTab, "ImageRecognition");
//        addTab(OutfeedTab, "Outfeed");
//        addTab(SealingTab, "Sealing");
//        addTab(SeedSupplyTab, "Seed Supply");
        this->setTabPosition(West);
        m_TabStyle = new TabStyle_HorizontalText();
        tabBar()->setStyle(m_TabStyle);
}

void DeviceTabWidget::on_ChangeDeviceStatus(int index, QString textColour, QString tabText){
    this->tabBar()->setTabText(index, tabText);
    this->tabBar()->setTabTextColor(index, textColour);
}

DeviceTabWidget::~DeviceTabWidget(){

}

QSize TabStyle_HorizontalText::sizeFromContents( //
    ContentsType eType,
    const QStyleOption *eOption,
    const QSize &eSize,
    const QWidget *eWidget) const {

    return QProxyStyle::sizeFromContents(eType, eOption, eSize, eWidget).transposed();
}

void TabStyle_HorizontalText::drawControl( //
    ControlElement element,
    const QStyleOption *option,
    QPainter *painter,
    const QWidget *widget) const {

    if (element == CE_TabBarTabLabel) {
        const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option);
        if (tab != nullptr) {
            QStyleOptionTab opt(*tab);
            opt.shape = QTabBar::RoundedNorth;
            QProxyStyle::drawControl(element, &opt, painter, widget);
            return;
        }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

