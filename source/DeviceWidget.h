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
/**
 * @brief The DeviceWidget class is a QMainWindow with a central Widget
 * containing buttons representing individual skill state as well as a
 * "Trigger Device State" button.
 * individual Skills as well as device state transitions can be triggered by
 * pressing the corresponding button
 */
class DeviceWidget : public QMainWindow {
    Q_OBJECT
public:
    /**
     * @brief DeviceWidget
     * Create UI for buttons in layout,start timer for polling servers
     * @param eDevice
     * @param tabIndex
     * @param parent
     */
    explicit DeviceWidget(IDevice *eDevice, int tabIndex, QWidget *parent = nullptr);
    ~DeviceWidget() final;
    /**
     * @brief MakeButtonLayout
     * Create new Widget with vertical layout, add buttons for triggering
     * skills and device status changes
     *
     */
    void MakeButtonLayout();

signals:
    /**
     * @brief DeviceStatusChanged
     * Updates device information in Tab
     * Emitted whenever new information has been polled
     * @param index
     * @param textColour
     * @param tabText
     */
    void DeviceStatusChanged(int index, QString textColour, QString tabText);
    /**
     * @brief TriggerDeviceState
     *
     */
    void TriggerDeviceState(QString);

public slots:
    void UpdateDeviceInfo();
    void on_AbortButtonClicked();
    /**
     * @brief on_SkillButtonClicked
     * Prompts a Q
     */
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
