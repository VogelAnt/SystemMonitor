#ifndef MODULESTATETABWIDGET_H
#define MODULESTATETABWIDGET_H

#include <QTabWidget>

namespace Ui {
class ModuleStateTabWidget;
}

class ModuleStateTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit ModuleStateTabWidget(QWidget *parent = nullptr);
    ~ModuleStateTabWidget();

private:
    Ui::ModuleStateTabWidget *ui;
};

#endif // MODULESTATETABWIDGET_H
