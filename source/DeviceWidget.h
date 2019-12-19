#ifndef DYNAMICCUSTOMTAB_H
#define DYNAMICCUSTOMTAB_H
#include "DeviceInformation.h"
#include "open62541.h"

#include <QButtonGroup>
#include <QDebug>
#include <QInputDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QStyleOptionTab>
#include <QStylePainter>
#include <QVBoxLayout>
#include <QWidget>

#include <iostream>
#include <memory>

namespace Ui {
class DeviceWidget;
}

class DeviceWidget : public QMainWindow {
    Q_OBJECT
public:
    //
    explicit DeviceWidget(
        UA_Client *eUaClient,
        std::map<char *, char *> eMap_Device_DisplayName_NodeId,
        std::map<char *, char *> eMap_DisplayName_NodeId,
        uint8_t index,
        int tabIndex,
        QWidget *parent = nullptr);
    virtual ~DeviceWidget();

    void MakeButtonLayout();
signals:
    void ChangeDeviceStatus(int index, QString textColour, QString tabText);

public slots:
    void on_UpdateDeviceUI(std::string nodevalue, std::pair<char *, char *> pair);
    void on_UpdateSkillsUI(std::string nodevalue, std::pair<char *, char *> pair);
    void on_AbortButtonClicked();
    void on_SkillButtonClicked();

private:
    Ui::DeviceWidget *ui;
    QPushButton *SkillButton = nullptr;
    UA_Client *m_UaClient = nullptr;
    QTimer *m_timer = nullptr;
    std::map<char *, char *> SkillMap_Id;
    QPushButton *m_abortButton = nullptr;
    std::map<char *, char *> DeviceMap_Id;
    std::map<char *, QPushButton *> SkillMap_Button;
    uint8_t DeviceNameSpace;
    int tabIndex;
    DeviceInformation *m_deviceinfo = nullptr;
    QVBoxLayout *m_buttonLayout = nullptr;
    QWidget *m_central = nullptr;
    QMessageBox *m_abortMessage = nullptr;
};

#endif // DYNAMICCUSTOMTAB_H
