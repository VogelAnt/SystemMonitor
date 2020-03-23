#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "DeviceTabWidget.h"
#include "OrderWidget.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QMainWindow>
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
    Ui::MainWindow *m_ui = nullptr;
    QHBoxLayout *m_layout = nullptr;

    std::map<std::string, IDevice *> m_DeviceMap;
    //    std::unique_ptr<DeviceTabWidget> m_deviceTab = nullptr;
    DeviceTabWidget *m_deviceTab = nullptr;
    QWidget *m_central = nullptr;
    OrderTableWidget *m_orderTable = nullptr;
};

#endif // MAINWINDOW_H
