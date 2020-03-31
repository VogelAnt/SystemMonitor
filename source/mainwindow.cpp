#include "mainwindow.h"
//#include "DeviceWidget.h"
//#include "orderinformation.h"
//#include "redisclient.h"
#include "ui_mainwindow.h"

using Redistorium::Redis;
using Redistorium::Reply::ReplyElement;
// TODO: showmaximized()?
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::MainWindow) {
    m_ui->setupUi(this);
    m_central = new QWidget(this);
    m_layout = new QHBoxLayout(m_central);
    m_viewOrdertable = new QAction("&Order Table", this);
    m_viewOrdertable->setCheckable(true);
    m_viewOrdertable->setChecked(false);
    m_viewDeviceTab = new QAction("&Device Tab", this);
    m_viewDeviceTab->setCheckable(true);
    m_viewDeviceTab->setChecked(true);
    m_viewBothwidgets = new QAction("&Show both", this);
    m_viewBothwidgets->setCheckable(true);
    m_viewBothwidgets->setChecked(false);
    m_selectionView = menuBar()->addMenu("&View");
    m_selectionView->addAction(m_viewOrdertable);
    m_selectionView->addAction(m_viewDeviceTab);
    m_selectionView->addAction(m_viewBothwidgets);
    connect(m_viewOrdertable, &QAction::triggered, this, &MainWindow::ToggleStatusBar);
    connect(m_viewDeviceTab, &QAction::triggered, this, &MainWindow::ToggleStatusBar);
    connect(m_viewBothwidgets, &QAction::triggered, this, &MainWindow::ToggleStatusBar);

    QMenu *test = nullptr;
    test = menuBar()->addMenu("&View");
    test->addAction(m_viewOrdertable);
    connect(m_viewOrdertable, &QAction::triggered, this, &MainWindow::ToggleStatusBar);
    m_quit = new QAction("&Quit", this);
    m_test = menuBar()->addMenu("&File");
    m_test->addAction(m_quit);
    connect(m_quit, &QAction::triggered, this, &QApplication::quit);
    m_orderTable = new OrderTableWidget(m_central);
    m_orderTable->setMaximumSize(299, 900);
    m_orderTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_deviceTab = new DeviceTabWidget(m_central);
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

void MainWindow::ToggleStatusBar() {
    if (m_viewBothwidgets->isChecked()) {
        m_viewBothwidgets->setChecked(true);
        m_viewOrdertable->setChecked(false);
        m_viewDeviceTab->setChecked(false);
        m_orderTable->setHidden(false);
        m_deviceTab->setHidden(false);
    } else if (m_viewOrdertable->isChecked() && ~m_viewDeviceTab->isChecked()) {
        m_orderTable->setHidden(false);
        m_deviceTab->setHidden(true);
        m_viewDeviceTab->setChecked(false);
    } else if (m_viewDeviceTab->isChecked() && ~m_viewOrdertable->isChecked()) {
        m_orderTable->setHidden(true);
        m_deviceTab->setHidden(false);
        m_viewOrdertable->setChecked(false);

    } else if (~m_viewDeviceTab->isChecked() && ~m_viewOrdertable->isChecked()) {
        m_orderTable->setHidden(true);
        m_deviceTab->setHidden(false);
        m_viewDeviceTab->setChecked(true);
    }
}

void MainWindow::InitializeDevices() {
    bool monitor_assembly = true;
    bool monitor_supertrak = true;
    bool monitor_labeling = true;
    bool monitor_image_recognition = false;
    bool monitor_sealing = false;
    bool monitor_seed_supply = false;
    bool monitor_outfeed = false;

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
        image_recognition->AddSkill("recognize");
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
