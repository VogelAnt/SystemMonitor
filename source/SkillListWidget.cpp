#include "SkillListWidget.h"
#include "redisclient.h"
#include "ui_SkillListWidget.h"

// TODO: What if you can't connect ? What if a module or Skill doesn't exist ? You'll crash right now, this is not acceptable !
static std::map<int, QString> sMap_String_OPCUAState{
    {0, "Idle"},
    {2, "Running"},
    {12, "Complete"},
    {17, "Aborted"},
    {15, "Stopped"},
    {1, "Starting"},
    {11, "Completing"},
    {18, "Clearing"},
    {13, "Resetting"},
    {14, "Stopping"},
    {16, "Aborting"}
};

static std::map<QString, QString> sMap_State_Colour{
    {"Idle", "lightgreen"},
    {"Running", "green"},
    {"Complete", "lightblue"},
    {"Aborted", "red"},
    {"Stopped", "grey"},
    {"Starting", "grey"},
    {"Completing", "grey"},
    {"Clearing", "grey"},
    {"Resetting", "grey"},
    {"Stopping", "grey"},
    {"Aborting", "lightred"}
};

SkillListWidget::SkillListWidget(UA_Client* client,
                                 std::map< char*,  char*> eMap_DisplayName_NodeId,
                                 uint8_t index,
                                 QWidget *parent) : QMainWindow(parent), ui(new Ui::SkillListWidget) {
    ui->setupUi(this);
    auto central = new QWidget(this);
    QVBoxLayout *ButtonLayout = new QVBoxLayout(this);

    m_UaClient = client;
    SkillMap_Id = eMap_DisplayName_NodeId;
    ModuleNameSpace = index;

    for (auto &pair: SkillMap_Id){
        QPushButton *skillButton = new QPushButton(pair.first,this);
        SkillMap_Button[pair.first] = skillButton;
        ButtonLayout->addWidget(skillButton);
    }

    central->setLayout(ButtonLayout);
    setCentralWidget(central);
    timer = new QTimer();
    connect(timer, &QTimer::timeout, [&](){
        // from this timer poll to ModuleState
        for (auto &pair: SkillMap_Id) {
            UA_NodeId nodeId = UA_NODEID_STRING(ModuleNameSpace, pair.second);
            UA_Variant value;
            UA_Variant_init(&value);
            UA_Client_readValueAttribute(m_UaClient, nodeId, &value);

            std::cout<< "Value Type : " << value.type->typeName<<std::endl;
            std::cout<< "UATYPES[UA_TYPES_INT32]" << UA_TYPES[UA_TYPES_INT32].typeName << std::endl;
            if(value.type->typeName == UA_TYPES[UA_TYPES_INT32].typeName){
                qDebug() << "WE ARE NOW IN UAINT COMPARISON";
                std::string temp = std::to_string(*((UA_Int32*) value.data));
                // convert node value to int
                int n = std::stoi(temp);
                QString n_value = sMap_String_OPCUAState.find(n)->second;
                QString node_value = " : " + sMap_String_OPCUAState.find(n)->second;
                qDebug() << "node value " << node_value;
                qDebug() << QString::number(n);
                qDebug() << sMap_String_OPCUAState.find(n)->second;
                SkillMap_Button[pair.first]->setText(pair.first + node_value);

                qDebug() << "Skill : " << pair.first;
                qDebug() << "sMapStateColour Second find" << sMap_State_Colour.find(n_value)->second;
                QString tmp = sMap_State_Colour.find(n_value)->second;
                QString temp_buttoncolour = "background-color : " + tmp;
                SkillMap_Button[pair.first]->setStyleSheet(temp_buttoncolour);
            }
            else if(value.type->typeName == UA_TYPES[UA_TYPES_STRING].typeName){

                                // read the node value
                std::string temp = ((char const*)(((UA_String*) value.data)->data));

//                std::cout << "whatever" << *((UA_String *)value.data)->data << std::endl;
//                std::string temp = std::to_string(*((UA_Int32*) value.data));
//                unsigned char * node = ((UA_String *)value.data)->data;
//                std::string node_reading(reinterpret_cast< char const* >(node));
//                std::cout << node_reading << std::endl;

                // convert to
                int n = std::stoi(temp);
                QString node_value = " : " + sMap_String_OPCUAState.find(n)->second;
//                qDebug() << "node value " << node_value;
//                qDebug() << QString::number(n);
    //            node_value = sMap_String_OPCUAState[n];
//                qDebug() << sMap_String_OPCUAState.find(n)->second;
                SkillMap_Button[pair.first]->setText(pair.first + node_value);
                SkillMap_Button[pair.first]->setStyleSheet("background-color : yellow");
//                qDebug() << "Skill : " << pair.first;
            }
        }
    });
    timer->start(3000);
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

