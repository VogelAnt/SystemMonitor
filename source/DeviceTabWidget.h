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
    DeviceTabWidget(QWidget *parent = nullptr);

    virtual ~DeviceTabWidget();
    void Initialize(std::map<std::string, IDevice *> *eDeviceList);
public slots:
    void on_ChangeDeviceStatus(int index, QString textColour, QString tabText);

private:
    TabStyle_HorizontalText *m_TabStyle = nullptr;
    std::map<std::string, IDevice *> *m_DeviceMap;
    IDevice *m_Assembly;
    IDevice *m_ImageRecognition;
    IDevice *m_Labeling;
    IDevice *m_SuperTrak;
};

#endif // TABSTYLE_HORIZONTALTEXT_H
