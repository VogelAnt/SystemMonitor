#ifndef SMG_DEVICEWIDGET_H
#define SMG_DEVICEWIDGET_H

#include "devices/IDevice.h"

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
    explicit DeviceWidget(IDevice *eDevice, int tabIndex, QWidget *parent = nullptr);
    ~DeviceWidget() final;
    void MakeButtonLayout();

signals:
    void DeviceStatusChanged(int index, QString textColour, QString tabText);
    void TriggerSkillStateTransition(QString);
    void AbortDeviceManually(QString);

public slots:
    void UpdateDeviceInfo();
    void on_AbortButtonClicked();
    void on_SkillButtonClicked();

private:
    Ui::DeviceWidget *ui;
    QPushButton *SkillButton = nullptr;
    QTimer *m_timer = nullptr;
    QPushButton *m_abortButton = nullptr;
    std::map<QString, QPushButton *> SkillMap_Button;
    int m_tabIndex;

    IDevice *m_Device = nullptr;
    QVBoxLayout *m_buttonLayout = nullptr;
    QWidget *m_central = nullptr;
    QMessageBox *m_abortMessage = nullptr;
};

#endif // SMG_DEVICEWIDGET_H
