#include "mainwindow.h"
#include "ui_mainwindow.h"

using Redistorium::Redis;
using Redistorium::Reply::ReplyElement;
// TODO: showmaximized()?
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::MainWindow) {
    m_ui->setupUi(this);
    m_central = new QWidget(this);
    m_layout = new QHBoxLayout(m_central);
    m_orderTable = new OrderTableWidget(m_central);
    m_orderTable->setMaximumSize(299, 900);
    this->setMaximumSize(500, 350);
    m_orderTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_deviceTab = new DeviceTabWidget(m_central);
    InitializeMenubar();
    InitializeDevices();
    m_deviceTab->Initialize(&m_DeviceMap);
    m_layout->addWidget(m_orderTable);
    m_layout->addWidget(m_deviceTab);
    m_orderTable->setHidden(1);
    m_central->setLayout(m_layout);
    setCentralWidget(m_central);
}

MainWindow::~MainWindow() {
    delete m_ui;
    delete m_central;
}

void MainWindow::InitializeMenubar() {
    m_viewOrdertable = new QAction("&Order Table", this);
    m_viewOrdertable->setCheckable(true);
    m_viewOrdertable->setChecked(false);
    m_viewDeviceTab = new QAction("&Device Tab", this);
    m_viewDeviceTab->setCheckable(true);
    m_viewDeviceTab->setChecked(true);
    m_viewBothwidgets = new QAction("&Show both", this);
    m_viewBothwidgets->setCheckable(true);
    m_viewBothwidgets->setChecked(false);
    m_widgetSelectionmenu = menuBar()->addMenu("&View");
    m_widgetSelectionmenu->addAction(m_viewOrdertable);
    m_widgetSelectionmenu->addAction(m_viewDeviceTab);
    m_widgetSelectionmenu->addAction(m_viewBothwidgets);
    connect(m_viewOrdertable, &QAction::triggered, this, &MainWindow::on_ToggleOrdertable);
    connect(m_viewDeviceTab, &QAction::triggered, this, &MainWindow::on_ToggleDeviceTab);
    connect(m_viewBothwidgets, &QAction::triggered, this, &MainWindow::on_ToggleBoth);
}

void MainWindow::on_ToggleBoth() {
    m_viewBothwidgets->setChecked(true);
    m_viewDeviceTab->setChecked(true);
    m_viewOrdertable->setChecked(true);
    m_orderTable->setHidden(false);
    m_deviceTab->setHidden(false);
}

void MainWindow::on_ToggleDeviceTab() {
    m_viewBothwidgets->setChecked(false);
    m_viewDeviceTab->setChecked(true);
    m_viewOrdertable->setChecked(false);
    m_orderTable->setHidden(true);
    m_deviceTab->setHidden(false);
}

void MainWindow::on_ToggleOrdertable() {
    m_viewBothwidgets->setChecked(false);
    m_viewDeviceTab->setChecked(false);
    m_viewOrdertable->setChecked(true);
    m_orderTable->setHidden(false);
    m_deviceTab->setHidden(true);
}

void MainWindow::InitializeDevices() {
    bool monitor_assembly = true;
    bool monitor_supertrak = true;
    bool monitor_labeling = true;
    bool monitor_image_recognition = true;
    bool monitor_sealing = true;
    bool monitor_seed_supply = false;
    bool monitor_outfeed = true;

    if (monitor_assembly) {
        IDevice *assembly = new OpcuaDevice("Assembly", 6, "::AsGlobalPV:gAssemblyModule", this);
        assembly->SetUrl("opc.tcp://localhost:4840"); // 192.168.0.5:4840
        assembly->AddSkill("provideCup");
        assembly->AddSkill("providePellet");
        m_DeviceMap["Assembly"] = assembly;
    }

    if (monitor_image_recognition) {
        IDevice *image_recognition = new OpcuaDevice("ImageRecognition", 2, "ImageRecognitionModule", this);
        image_recognition->SetUrl("opc.tcp://localhost:4840");
        image_recognition->AddSkill("findSeed");
        //        image_recognition->AddSkill("recognize");
        m_DeviceMap["ImageRecognition"] = image_recognition;
    }

    if (monitor_labeling) {
        IDevice *labeling = new OpcuaDevice("Labeling", 2, "gLabelingModule", this);
        labeling->SetUrl("opc.tcp://localhost:4840"); // 192.168.0.102:4840
        labeling->AddSkill("label");
        m_DeviceMap["Labeling"] = labeling;
    }

    if (monitor_outfeed) {
        IDevice *outfeed = new OpcuaDevice("Outfeed", 2, "PLACEHOLDER_OUTFEED_NODE_ID", this);
        outfeed->SetUrl("opc.tcp://localhost:4840");
        outfeed->AddSkill("PLACEHOLDER1");
        outfeed->AddSkill("PLACEHOLDER2");
        outfeed->AddSkill("PLACEHOLDER3");
        m_DeviceMap["Outfeed"] = outfeed;
    }

    if (monitor_sealing) {
        IDevice *sealing = new OpcuaDevice("Sealing", 2, "StOpcCom:opcComSealingModule", this);
        sealing->SetUrl("opc.tcp://localhost:4840");
        sealing->AddSkill("sealing");
        m_DeviceMap["Sealing"] = sealing;
    }

    if (monitor_seed_supply) {
        IDevice *seed_supply = new OpcuaDevice("SeedSupply", 2, "::AsGlobalPV:opcComMitsubishi", this);
        seed_supply->SetUrl("opc.tcp://192.168.0.180:4840");
        seed_supply->AddSkill("moveToSTHomePosition");
        seed_supply->AddSkill("provideItemFromSTToWelding");
        seed_supply->AddSkill("provideItemFromWeldingToST");
        seed_supply->AddSkill("provideItemToST");
        seed_supply->AddSkill("provideItemToWelding");
        seed_supply->AddSkill("releaseItemToST");

        m_DeviceMap["SeedSupply"] = seed_supply;
    }

    if (monitor_supertrak) {
        IDevice *superTrak = new OpcuaDevice("SuperTrak", 6, "::AsGlobalPV:gSuperTrak", this);
        superTrak->SetUrl("opc.tcp://localhost:4840"); // 192.168.0.5:4840
        superTrak->AddSkill("checkParking");
        for (int shuttle = 1; shuttle <= 6; ++shuttle) { //
            superTrak->AddSkill(QString("moveShuttle[") + std::to_string(shuttle).c_str() + "]");
        }
        m_DeviceMap["SuperTrak"] = superTrak;
    }
}
