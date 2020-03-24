/**
 * @file DeviceTabWidget.h
 * @brief This file contains the declarations of the TabStyle_HorizontalText class
 * and the DeviceTabWidget class
 */

#ifndef TABSTYLE_HORIZONTALTEXT_H
#define TABSTYLE_HORIZONTALTEXT_H
#include "DeviceWidget.h"

#include "devices/OpcuaDevice.h"

#include <QProxyStyle>
#include <QStyleOption>
#include <QTabWidget>

#include <memory>

/**
 * @brief The TabStyle_HorizontalText class is used as a custom tab style for a tab widget
 * with its tabPosition set to either East or West.
 * By default, the text will be vertical. This is not ideal in this program since a quick overview is needed
 * and there is plenty of room for the horizontal text.
 */
class TabStyle_HorizontalText : public QProxyStyle {
public:
    /**
     * @brief sizeFromContents overrides QProxyStyle::sizeFromContents and transposes the size to make it horizontal.
     * All parameters are simply those needed for QProxyStyle::sizeFromContents.
     * @param eType
     * @param eOption
     * @param eSize
     * @param eWidget
     * @return
     */
    QSize sizeFromContents(ContentsType eType, const QStyleOption *eOption, const QSize &eSize, const QWidget *eWidget) const override;

    /**
     * @brief drawControl overrides QProxyStyle::drawControl to draw the text horizontally.
     * @param eElement
     * @param eOption
     * @param ePainter
     * @param eWidget
     */
    void drawControl(ControlElement eElement, const QStyleOption *eOption, QPainter *ePainter, const QWidget *eWidget) const override;
};

/**
 * @brief The DeviceTabWidget class is a customized QTabWidget in which each
 * individual tab represents a device with its current state information gained
 * from the corresponding OPCUA server
 */
class DeviceTabWidget : public QTabWidget {
    Q_OBJECT
public:
    /**
     * @brief DeviceTabWidget
     * set tab style
     * @param parent
     */
    DeviceTabWidget(QWidget *parent = nullptr);

    /**
     * @brief ~DeviceTabWidget
     * destroy custom tab style
     */
    virtual ~DeviceTabWidget();

    /**
     * @brief Initialize
     * Create map containing string name of device to technical opcua name
     * @param eDeviceList
     * A map from easily readable device name to technical name used in opcua
     */
    void Initialize(std::map<std::string, IDevice *> *eDeviceList);
public slots:
    /**
     * @brief on_ChangeDeviceStatus
     * Slot changing the text colour of the tab depending on status updated
     * @param index
     * @param textColour
     * @param tabText
     */
    void on_ChangeDeviceStatus(int index, QString textColour, QString tabText);

private:
    /**
     * @brief m_TabStyle
     * Instantiation of horizontal text style for tab text
     */
    TabStyle_HorizontalText *m_TabStyle = nullptr;
    /**
     * @brief m_DeviceMap
     * map from device name as string to
     */
    std::map<std::string, IDevice *> *m_DeviceMap;
    IDevice *m_Assembly;
    IDevice *m_ImageRecognition;
    IDevice *m_Labeling;
    IDevice *m_SuperTrak;
};

#endif // TABSTYLE_HORIZONTALTEXT_H
