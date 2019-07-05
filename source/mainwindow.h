#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dynamiccustomtab.h"
#include "ordertable.h"
#include "redisclient.h"
#include "tabstyle_horizontaltext.h"

#include <QHBoxLayout>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QStringList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void parse_OrderPage_data();

signals:
    void SendCommand(QString);
    void SendMES_Data(nlohmann::json);
    void GetReply();

public slots:
    void on_sendMES_data(nlohmann::json);
    void on_sendOrderPageData(nlohmann::json);

private slots:
    void on_SendCommand_clicked();
    void on_GetReply_clicked();

    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::MainWindow *ui = nullptr;
    QStringList headerColumns;
    RedisClient *m_RedisClient = nullptr;
    QMessageBox *prioritybox = nullptr;
    TabStyle_HorizontalText *m_TabStyle = nullptr;
};

#endif // MAINWINDOW_H
