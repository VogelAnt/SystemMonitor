#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "OrderWidget.h"
#include "DeviceTabWidget.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QString>

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

    void MockOrderPage();
    void MockorderPageList();

private:
    Ui::MainWindow *m_ui = nullptr;
    QHBoxLayout *m_layout = nullptr;
    DeviceTabWidget *m_deviceTab = nullptr;
    QWidget *m_central = nullptr;
    OrderTableWidget *m_orderTable = nullptr;
};

#endif // MAINWINDOW_H
