/**
 * @file mainwindow.h
 * @brief This file contains the declaration of the mainwindow class
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "DeviceTabWidget.h"
#include "OrderWidget.h"

#include <QAction>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

#include <iostream>
#include <string>

namespace Ui {
class MainWindow;
}
/**
 * @brief The MainWindow class is the core of the user interface consisting of
 * a central widget in which either a customized QTabWidget, QTableWidget
 * or both can be displayed
 *
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    /**
     * @brief MainWindow
     * Calls function InitiallizeDevices and sets up ui
     * @param parent is nullpointer
     */
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /**
     * @brief InitializeDevices
     * Instantiates new Devices and sets respective Skills
     */
    void InitializeDevices();
    /**
     * @brief m_quit
     */
    QAction *m_quit = nullptr;
    /**
     * @brief m_test
     */
    QMenu *m_test = nullptr;
    /**
     * @brief m_ui
     * the ui of the main window
     */
    Ui::MainWindow *m_ui = nullptr;
    /**
     * @brief m_layout
     * the horizontal layout of the main window
     */
    QHBoxLayout *m_layout = nullptr;
    /**
     * @brief m_DeviceMap
     */
    std::map<std::string, IDevice *> m_DeviceMap;
    /**
     * @brief m_deviceTab
     */
    DeviceTabWidget *m_deviceTab = nullptr;
    /**
     * @brief m_central
     * central widget of main window
     */
    QWidget *m_central = nullptr;
    /**
     * @brief m_orderTable
     * QTableWidget containing order information
     */
    OrderTableWidget *m_orderTable = nullptr;
};

#endif // MAINWINDOW_H
