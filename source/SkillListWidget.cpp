#include "SkillListWidget.h"
#include "redisclient.h"
#include "ui_SkillListWidget.h"
#include <iostream>

SkillListWidget::SkillListWidget(UA_Client* client,
                                 std::map< char*,  char*> eMap_DisplayName_NodeId,
                                 uint8_t index,
                                 QWidget *parent) : QWidget(parent), ui(new Ui::SkillListWidget) {

    ui->setupUi(this);
    m_UaClient = client;
    SkillMap_Id = eMap_DisplayName_NodeId;
    ModuleNameSpace = index;

    for (auto &pair: SkillMap_Id){
        QPushButton *skillButton = new QPushButton(pair.first,this);
        SkillMap_Button[pair.first] = skillButton;
    }

    timer = new QTimer();
    connect(timer, &QTimer::timeout, [&](){
        for (auto &pair: SkillMap_Id) {
            UA_NodeId nodeId = UA_NODEID_STRING(ModuleNameSpace, pair.second);
            UA_Variant value;
            UA_Variant_init(&value);
            UA_Client_readValueAttribute(m_UaClient, nodeId, &value);

            std::string c = ((char const*)(((UA_String*) value.data)->data));
            SkillMap_Button[pair.first]->setText(c.c_str());
        }
    });
    timer->start(1000);
}

void SkillListWidget::on_SendAssemblySkillState(std::map<std::string, std::string>SkillStateMap ){
    std::cout << "NOW IN ON_SEND _ASSEMBLY_ SKILLSTATE" << std::endl;
    int Buttonindex = 0;
    std::string default_node_value = "17";
    for (std::map<std::string, std::string>::iterator it = SkillStateMap.begin(); it != SkillStateMap.end(); ++it){
        std::cout << "We got Skill "<< it->first << " with value " << it->second << std::endl;
        for (int i = 0; i < default_node_value.length(); ++i){
            if((it->second[i] - '0') != (default_node_value[i] - '0')){
//                std::cout << "setting Button to red" << std::endl;
            }
            else {
//            std::cout << "setting Button to green " <<std:: endl;
            }
        }
    }

}

void SkillListWidget::on_SendSuperTrakSkillState(std::map<std::string, std::string>SkillStateMap ){
    std::cout << "NOW IN ON_SENDSKILL _SUPERTRAK_ STATE" << std::endl;
    int Buttonindex = 0;
    std::string default_node_value = "17";
    for (std::map<std::string, std::string>::iterator it = SkillStateMap.begin(); it != SkillStateMap.end(); ++it){
        std::cout << "We got Skill "<< it->first << " with value " << it->second << std::endl;

        for (int i = 0; i < default_node_value.length(); ++i){
            if((it->second[i] - '0') != (default_node_value[i] - '0')){
//                std::cout << "setting Button to red" << std::endl;
//                ui->tabWidget->tabBar()->setTabTextColor(tab_index, QColor(Qt::red));
            }
            else {
//            std::cout << "setting Button to green " <<std:: endl;
//            ui->tabWidget->tabBar()->setTabTextColor(tab_index, QColor(Qt::green));
            }
        }
    }

}
void SkillListWidget::on_SendLabelingSkillState(std::map<std::string, std::string>SkillStateMap ){
    std::cout << "NOW IN ON_SENDSKILL _LABELING_STATE" << std::endl;
    int Buttonindex = 0;
    std::string default_node_value = "17";
    for (std::map<std::string, std::string>::iterator it = SkillStateMap.begin(); it != SkillStateMap.end(); ++it){
        std::cout << "We got Skill "<< it->first << " with value " << it->second << std::endl;

        for (int i = 0; i < default_node_value.length(); ++i){
            if((it->second[i] - '0') != (default_node_value[i] - '0')){
//                std::cout << "setting Button to red" << std::endl;
//                ui->tabWidget->tabBar()->setTabTextColor(tab_index, QColor(Qt::red));
            }
            else {
//            std::cout << "setting Button to green " <<std:: endl;
//            ui->tabWidget->tabBar()->setTabTextColor(tab_index, QColor(Qt::green));
            }    }

    }

}


void SkillListWidget::on_SendSkillState(std::map<std::string, std::string>SkillStateMap){
    std::cout << "NOW IN ON_SENDSKILLSTATE" << std::endl;
    int Buttonindex = 0;
    std::string default_node_value = "17";
    for (std::map<std::string, std::string>::iterator it = SkillStateMap.begin(); it != SkillStateMap.end(); ++it){
        std::cout << "We got Skill "<< it->first << " with value " << it->second << std::endl;

        for (int i = 0; i < default_node_value.length(); ++i){
            if((it->second[i] - '0') != (default_node_value[i] - '0')){
//                std::cout << "setting Button to red" << std::endl;
//                ui->tabWidget->tabBar()->setTabTextColor(tab_index, QColor(Qt::red));
            }
            else {
//            std::cout << "setting Button to green " <<std:: endl;
//            ui->tabWidget->tabBar()->setTabTextColor(tab_index, QColor(Qt::green));
            }    }

    }
}

SkillListWidget::~SkillListWidget() {
    delete ui;
}

void SkillListWidget::SkillButton_clicked() {
    QMessageBox::information(this, "Skill Status Report", "Err/Warn/OK");
    std::cout << "NOW IN SKILL BUTTON CLICKED \n";
}

void SkillListWidget::on_SendMESData() {
    std::cout << "onsendMESDATA" << std::endl;
}

// Buttons, and that should perhaps open up a new menu ?
//

