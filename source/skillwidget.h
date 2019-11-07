#ifndef SKILLWIDGET_H
#define SKILLWIDGET_H

#include <QWidget>

namespace Ui {
class SkillWidget;
}

class SkillWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SkillWidget(QWidget *parent = nullptr);
    ~SkillWidget();

private:
    Ui::SkillWidget *ui;
};

#endif // SKILLWIDGET_H
