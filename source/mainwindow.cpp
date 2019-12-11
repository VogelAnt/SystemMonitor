#include "mainwindow.h"
#include "DeviceWidget.h"
#include "orderinformation.h"
#include "redisclient.h"
#include "ui_mainwindow.h"

using Redistorium::Redis;
using Redistorium::Reply::ReplyElement;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::MainWindow) {
    m_ui->setupUi(this);
    m_central = new QWidget(this);
    m_layout = new QHBoxLayout(m_central);
    m_orderTable = new OrderTableWidget(this);
    m_deviceTab = new DeviceTabWidget(this);
    m_layout->addWidget(m_orderTable);
    m_layout->addWidget(m_deviceTab);
    m_central->setLayout(m_layout);
    setCentralWidget(m_central);
}

MainWindow::~MainWindow() {
    delete m_ui;
    delete m_layout;
    delete m_deviceTab;
    delete m_central;
}

// Create
