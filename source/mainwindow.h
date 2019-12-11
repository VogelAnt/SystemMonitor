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
    Ui::MainWindow *m_ui = nullptr;
    QHBoxLayout *m_layout = nullptr;
    DeviceTabWidget *m_deviceTab = nullptr;
    QWidget *m_central = nullptr;
    OrderTableWidget *m_orderTable = nullptr;
};

#endif // MAINWINDOW_H
