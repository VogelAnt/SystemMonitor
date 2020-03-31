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
#include <QStatusBar>
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

private slots:
    /**
     * @brief ToggleStatusBar
     * Toggle visibility of the DeviceTab widget by checking or unchecking
     */
    void on_ToggleDeviceTab();
    /**
     * @brief on_ToggleOrdertable
     * Toggle visibility of the OrderTable widget by checking or unchecking
     */
    void on_ToggleOrdertable();
    /**
     * @brief on_ToggleBoth
     * Toggle visibility of both the DeviceTab widget as well as the OrderTable widget
     */
    void on_ToggleBoth();

private:
    /**
     * @brief InitializeDevices
     * Instantiates new Devices and sets respective Skills
     */
    void InitializeDevices();
    /**
     * @brief InitializeMenubar
     * Instantiates menu with predefined actions for switching view of widgets
     */
    void InitializeMenubar();
    /**
     * @brief m_viewOrdertable
     * Action for displaying the order table
     */
    QAction *m_viewOrdertable = nullptr;
    /**
     * @brief m_viewDeviceTab
     * Action for displaying the device tab
     */
    QAction *m_viewDeviceTab = nullptr;
    /**
     * @brief m_viewBothwidgets
     * Action for displaying both widgets at the same time
     */
    QAction *m_viewBothwidgets = nullptr;
    /**
     * @brief m_widgetSelectionmenu
     * menu for selection of widget to be viewed
     */
    QMenu *m_widgetSelectionmenu = nullptr;
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
