#ifndef DYNAMICCUSTOMTAB_H
#define DYNAMICCUSTOMTAB_H
#include "open62541.h"
#include "DeviceInformation.h"

#include <QVBoxLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QWidget>
#include <QStyleOptionTab>
#include <QStylePainter>
#include <QButtonGroup>
#include <QDebug>

#include <iostream>

namespace Ui {
class DeviceWidget;
}

class DeviceWidget : public QMainWindow {
    Q_OBJECT
public:
    //
    explicit DeviceWidget(UA_Client * eUaClient,
                             std::map< char*,  char*> eMap_Device_DisplayName_NodeId,
                             std::map< char*,  char*> eMap_DisplayName_NodeId,
                             uint8_t index,
                             int tabIndex,
                             QWidget *parent = nullptr);
    virtual ~DeviceWidget();

signals:
    void ChangeDeviceStatus(int index, QString textColour, QString tabText);

public slots:
    void on_UpdateDeviceUI(std::string nodevalue, std::pair<char *, char *>pair);
    void on_UpdateSkillsUI(std::string nodevalue, std::pair<char *, char *>pair);

private:
    Ui::DeviceWidget *ui;
    // TODO:  implement this in cpp
    QPushButton *SkillButton;
    UA_Client *m_UaClient;
    QTimer *timer;
    std::map< char*,  char*> SkillMap_Id;
    std::map< char*, char*> DeviceMap_Id;
    std::map< char*,  QPushButton*> SkillMap_Button;
    uint8_t DeviceNameSpace;
    int tabIndex;
};

#endif // DYNAMICCUSTOMTAB_H
