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

#include <iostream>

namespace Ui {
class SkillListWidget;
}

class SkillListWidget : public QMainWindow {
    Q_OBJECT
public:
    //
    explicit SkillListWidget(UA_Client * eUaClient,
                             std::map< char*,  char*> eMap_Device_DisplayName_NodeId,
                             std::map< char*,  char*> eMap_DisplayName_NodeId, // the display name and node id of each skill
                             uint8_t index,
                             QWidget *parent = nullptr);
    virtual ~SkillListWidget();

signals:
    //signals:
    // trigger ui (specifically tabWidget)


public slots:
    void on_UpdateDeviceUI(std::string nodevalue, std::pair<char *, char *>pair);
    void on_UpdateSkillsUI(std::string nodevalue, std::pair<char *, char *>pair);

private:
    Ui::SkillListWidget *ui;
    // TODO:  implement this in cpp
    QPushButton *SkillButton;
    UA_Client *m_UaClient;
    QTimer *timer;
    std::map< char*,  char*> SkillMap_Id;
    std::map< char*, char*> DeviceMap_Id;
    std::map< char*,  QPushButton*> SkillMap_Button;
    uint8_t DeviceNameSpace;
};

class Testinfo : public SkillListWidget{
    explicit Testinfo();
    ~Testinfo();
};

#endif // DYNAMICCUSTOMTAB_H
