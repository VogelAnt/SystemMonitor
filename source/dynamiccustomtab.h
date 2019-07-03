#ifndef DYNAMICCUSTOMTAB_H
#define DYNAMICCUSTOMTAB_H
#include "redisclient.h"

#include <QHBoxLayout>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QWidget>

namespace Ui {
class DynamicCustomTab;
}

class DynamicCustomTab : public QWidget {
    Q_OBJECT

public:
    explicit DynamicCustomTab(QWidget *parent = nullptr);
    virtual ~DynamicCustomTab();

public slots:
    void SkillButton_clicked();
    void on_SendMESData();

private:
    Ui::DynamicCustomTab *ui;
    QPushButton *SkillButton;
};

#endif // DYNAMICCUSTOMTAB_H
