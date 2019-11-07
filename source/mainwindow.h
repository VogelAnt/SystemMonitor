#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "OpcuaClient.h"
#include "dynamiccustomtab.h"
#include "orderinformation.h"
#include "redisclient.h"
#include "skillwidget.h"
#include "tabstyle_horizontaltext.h"

#include <QHBoxLayout>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QTimer>

#include <random>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void test();

signals:
    void SendCommand(QString);
    void SendMES_Data(nlohmann::json);
    void GetReply();
    void ReceivedNewSubscription(std::optional<QString> eParsed);

public slots:
    void on_MakeOrderTable(nlohmann::json);

private slots:
    void on_tableWidget_cellDoubleClicked(int row, int column);
    void on_SubscriptionMessage(QString eChannel, QString eMessage);
    void on_ChangeColour();
    void on_DisplaySkillWidget();

private:
    Ui::MainWindow *ui = nullptr;
    QStringList headerColumns;
    RedisClient *m_RedisClient = nullptr;
    QMessageBox *prioritybox = nullptr;
    TabStyle_HorizontalText *m_TabStyle = nullptr;
    OrderInformation *order = nullptr;
    OpcuaClient *m_OpcuaClient = nullptr;
    int m_OrderNumber = 0;
};

#endif // MAINWINDOW_H
