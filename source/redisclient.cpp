#include "redisclient.h"

RedisClient::RedisClient(QObject *parent) : QObject(parent) {
    m_Timer = new QTimer(this);
    m_Redis = new Redistorium::Redis();
    connect(m_Timer, &QTimer::timeout, this, &RedisClient::update);
    m_Redis->Connect("127.0.0.1", 6379);
    m_Timer->start(5000);
}

nlohmann::json RedisClient::MES_json() {
    std::cout << "Now making MES JSON" << std::endl;
    nlohmann::json MES_json;
    auto module_arr = nlohmann::json::array();
    for (auto &module : ModuleList) {
        nlohmann::json module_json;
        module_json["ModuleName"] = module;
        auto skill_arr = nlohmann::json::array();
        for (auto &skill : SkillList) {
            // replace "OK" with actual status variable, should be a random string
            nlohmann::json j_2{{"SkillName", skill}, {"SkillStatus", "OK"}};
            skill_arr.push_back(j_2);
        }
        module_json["skills"] = skill_arr;
        module_arr.push_back(module_json);
    }
    MES_json["MES_data"] = module_arr;
    return MES_json;
}

nlohmann::json RedisClient::OrderPage_json() {
    std::cout << "Now making CLP JSON" << std::endl;
    nlohmann::json OrderPage_json;
    auto order_arr = nlohmann::json::array();
    int i = 1;
    for (auto &order : OrderList) {
        nlohmann::json order_json{{"Order#", i}, {"Priority", "low/high/medium"}};
        order_arr.push_back(order_json);
        ++i;
    }
    // check if the two are equivalent, then try key approach
    OrderPage_json["OrderPage_data"] = order_arr; // = array
    return OrderPage_json;
}

/**
 * @brief RedisClient::stringify_json
 * converts JSON object to usable string for redis server
 * @param object_to_be_stringified
 * the nlohman::json object
 * @return
 */
QString RedisClient::stringify_json(nlohmann::json object_to_be_stringified) {
    std::cout << "IN STRINGIFY" << std::endl;
    std::string tempstdstr = object_to_be_stringified.dump();
    QString tempqstring = tempstdstr.c_str();

    tempqstring.replace("\"", "\\\"");

    tempqstring = QString("\"" + tempqstring + "\"");
    std::cout << " ###THIS IS WHAT YOUR JSON LOOKS LIKE NOW \n " << tempqstring.toStdString() << std::endl;
    return tempqstring;
}

// void RedisClient::on_BulkStringReceived(const QString &eString)
//{
//    std::cout <<"now in Bulk string heres what we received"<< eString.toStdString() <<std::endl;
//    QString temp = eString;
//    temp.replace("\\\"", "\"");
//    nlohmann::json parsed = nlohmann::json::parse(temp.toStdString());
//    if(parsed["MES_data"] != nullptr)
//    {
//        emit sendMESdata(parsed);
//    }
//    else
//    {
//        emit sendOrderPageData(parsed);
//    }
//    // std::cout<<"####### PARSED:"<<parsed[0]<<std::endl;
//    // std::cout<<"####### PARSED:"<<parsed["data"][0]["name"]<<std::endl;
//    // this is not necessary, just call the GUI
//}

/*!
 * \brief RedisClient::json_check
 * Checks the type of information received (MES/Order/invalid)
 * \param parsed
 */

RedisClient::~RedisClient() {
    if (m_Timer != nullptr) delete m_Timer;
}

void RedisClient::SendCommand(const QString &eCommand) {
    //    m_Redis->Set(eCommand, );
    std::cout << "Test" << std::endl;
}

void RedisClient::update() {
    qDebug() << "Test";
}

void RedisClient::GetReply() {
    // // Make a string vector here
    //    redisReply *reply = static_cast<redisReply *>(redisCommand(m_Context, "GET key"));
    //  //redisReply *reply = static_cast<redisReply *>(redisCommand(m_Context, "HGETALL Order#1"));
    //  QString temp = reply->str;
    //  emit ReplyReceived(temp);
    //  delete reply;
    //    std::cout << test[ModuleList[0]].get<std::string>() << std::endl;
}
