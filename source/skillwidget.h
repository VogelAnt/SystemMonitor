#ifndef SKILLWIDGET_H
#define SKILLWIDGET_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTimer>

#include <map>

namespace Ui {
class SkillWidget;
}

class SkillWidget : public QMainWindow{
    Q_OBJECT

public:
    explicit SkillWidget(std::map<char*, char*>eMap_DisplayName_NodeId,
        QWidget *parent = nullptr);
    ~SkillWidget();
    void CreateButton(std::string, std::pair<char*, char*>);

private:
    Ui::SkillWidget *ui;
    QPushButton *SkillButton;
    QTimer *timer;
    std::map< char*,  char*> SkillMap_Id;
    std::map< char*,  QPushButton*> SkillMap_Button;
};

#endif // SKILLWIDGET_H
