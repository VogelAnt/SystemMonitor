#ifndef DYNAMICCUSTOMTAB_H
#define DYNAMICCUSTOMTAB_H
#include "redisclient.h"

#include <QHBoxLayout>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QWidget>
#include <QStyleOptionTab>
#include <QStylePainter>
#include <QTabWidget>

namespace Ui {
class DynamicCustomTab;
}
/**
 * @brief The TabBar class
 *
 */
class TabBar: public QTabBar
{
public:
    TabBar(const QHash<QString, QColor> &colors, QWidget *parent=nullptr) : QTabBar(parent)
    {
        mColors = colors;
    }

protected:
    /**
     * @brief paintEvent
     * overrides the QTabbar paint event so we can colour Tabs individually
     */
    void paintEvent(QPaintEvent */*event*/){
        std::cout << "Now in Paint Event" <<std::endl;
        QStylePainter painter(this);
        QStyleOptionTab opt;
        for(int i = 0;i < count();i++){
            initStyleOption(&opt,i);
            if(mColors.contains(opt.text)){
                opt.palette.setColor(QPalette::Button, mColors[opt.text]);
            }
            painter.drawControl(QStyle::CE_TabBarTabShape, opt);
            painter.drawControl(QStyle::CE_TabBarTabLabel,opt);
        }
    }

private:
    QHash<QString, QColor> mColors;
};

class DynamicCustomTab : public QTabWidget {
    Q_OBJECT

public:
    explicit DynamicCustomTab(QWidget *parent = nullptr);
    virtual ~DynamicCustomTab();

public slots:
    //functions for testing
    void SkillButton_clicked();
    void on_SendMESData();
    /**
     * @brief on_sendModuleState
     * receives ModuleState information
     */
    void on_sendModuleState(std::map<std::string, std::string>);

private:
    Ui::DynamicCustomTab *ui;
    QPushButton *SkillButton;
};


#endif // DYNAMICCUSTOMTAB_H
