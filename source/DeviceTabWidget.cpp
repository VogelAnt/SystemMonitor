#include "DeviceTabWidget.h"

#include <open62541.h>

DeviceTabWidget::DeviceTabWidget(QWidget *parent) : QTabWidget(parent) {
    tabBar()->setStyleSheet("font-size : 24px");
    this->setTabPosition(West);
    m_TabStyle = new TabStyle_HorizontalText();
    tabBar()->setStyle(m_TabStyle);
}

void DeviceTabWidget::on_ChangeDeviceStatus(int index, QString textColour, QString tabText) {
    this->tabBar()->setTabText(index, tabText);
    this->tabBar()->setTabTextColor(index, textColour);
}

DeviceTabWidget::~DeviceTabWidget() {
    delete m_TabStyle;
}

void DeviceTabWidget::Initialize(std::map<std::string, IDevice *> *eDeviceList) {
    m_DeviceMap = eDeviceList;
    for (auto &pair : *m_DeviceMap) { pair.second->Connect(); }

    m_Assembly = m_DeviceMap->at("Assembly");
    auto *AssemblyTab = new DeviceWidget(m_Assembly, 0, this);
    connect(AssemblyTab, &DeviceWidget::DeviceStatusChanged, this, &DeviceTabWidget::on_ChangeDeviceStatus);
    addTab(AssemblyTab, "Assembly");

    m_Labeling = m_DeviceMap->at("Labeling");
    auto *LabelingTab = new DeviceWidget(m_Labeling, 1, this);
    connect(LabelingTab, &DeviceWidget::DeviceStatusChanged, this, &DeviceTabWidget::on_ChangeDeviceStatus);
    addTab(LabelingTab, "Labeling");

    m_SuperTrak = m_DeviceMap->at("SuperTrak");
    auto *STTab = new DeviceWidget(m_SuperTrak, 2, this);
    connect(STTab, &DeviceWidget::DeviceStatusChanged, this, &DeviceTabWidget::on_ChangeDeviceStatus);
    addTab(STTab, "SuperTrak");
}

QSize TabStyle_HorizontalText::sizeFromContents(ContentsType eType, const QStyleOption *eOption, const QSize &eSize, const QWidget *eWidget) const {

    return QProxyStyle::sizeFromContents(eType, eOption, eSize, eWidget).transposed();
}

void TabStyle_HorizontalText::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const {

    if (element == CE_TabBarTabLabel) {
        const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option);
        if (tab != nullptr) {
            QStyleOptionTab opt(*tab);
            opt.shape = QTabBar::RoundedNorth;
            QProxyStyle::drawControl(element, &opt, painter, widget);
            return;
        }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}
