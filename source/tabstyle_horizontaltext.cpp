#include "tabstyle_horizontaltext.h"

QSize TabStyle_HorizontalText::sizeFromContents( //
    ContentsType eType,
    const QStyleOption *eOption,
    const QSize &eSize,
    const QWidget *eWidget) const {

    return QProxyStyle::sizeFromContents(eType, eOption, eSize, eWidget).transposed();
}

void TabStyle_HorizontalText::drawControl( //
    ControlElement element,
    const QStyleOption *option,
    QPainter *painter,
    const QWidget *widget) const {

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

MyCustomTab::MyCustomTab(QWidget *parent) : QTabWidget(parent){

}

MyCustomTab::~MyCustomTab(){

}
