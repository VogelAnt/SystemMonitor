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

signals:
//    void SendCommand(QString);
//    void SendMES_Data(nlohmann::json);
//    void GetReply();
//    void ReceivedNewSubscription(std::optional<QString> eParsed);
//    void OrderPriorityChanged(int);

public slots:
//    void on_MakeOrderTable(nlohmann::json);

//private slots:
//    void on_SubscriptionMessage(QString eChannel, QString eMessage);

private:
    Ui::MainWindow *m_ui = nullptr;
    QHBoxLayout *m_layout = nullptr;
    DeviceTabWidget *m_deviceTab = nullptr;
    QWidget *m_central = nullptr;
    OrderWidget *m_orderTable = nullptr;
};

#endif // MAINWINDOW_H
