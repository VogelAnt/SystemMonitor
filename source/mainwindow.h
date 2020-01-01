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

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void InitializeDevices();
    Ui::MainWindow *m_ui = nullptr;
    QHBoxLayout *m_layout = nullptr;

    std::map<std::string, IDevice *> m_DeviceMap;
    std::unique_ptr<DeviceTabWidget> m_deviceTab = nullptr;
    QWidget *m_central = nullptr;
    OrderTableWidget *m_orderTable = nullptr;
};

#endif // MAINWINDOW_H
