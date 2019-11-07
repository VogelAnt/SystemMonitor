#include "redisclient.h"

RedisClient::RedisClient(QObject *parent) : QObject(parent) {
    m_Redis = new Redistorium::Redis();
    m_Redis->Connect("localhost", 6379);
    connect(m_Redis, &Redistorium::Redis::SubscriptionMessage, this, &RedisClient::SubscriptionMessage);
    connect(m_Redis, &Redistorium::Redis::Connected, []() { qDebug() << "Connected"; });

    connect(m_Redis, &Redistorium::Redis::Disconnected, []() { qDebug() << "Disconnected"; });
}

// for testing purposes only
nlohmann::json RedisClient::make_json_MES() {
    // std::cout << "Now making MES JSON" << std::endl;
    nlohmann::json MES_json;
    auto module_arr = nlohmann::json::array();
    for (auto &module : ModuleList) {
        nlohmann::json module_json;
        module_json["ModuleName"] = module;
        auto skill_arr = nlohmann::json::array();
        for (auto &skill : SkillList) {
            nlohmann::json j_2{{"SkillName", skill}, {"SkillStatus", "OK"}};
            skill_arr.push_back(j_2);
        }
        module_json["skills"] = skill_arr;
        module_arr.push_back(module_json);
    }
    MES_json["data_MES"] = module_arr;
    // std::cout << MES_json;
    return MES_json;
}

nlohmann::json RedisClient::make_json_orderpage() {
    nlohmann::json OrderPage_json;
    auto order_arr = nlohmann::json::array();
    int r = rand();
    // problem is here
    // create a random number,
    std::string rand = std::to_string(r);
    nlohmann::json order_json{{"orderID", rand}, {"priority", "0"}, {"firstName", "John"}, {"lastName", "Kimble"}};
    order_arr.push_back(order_json);
    OrderPage_json["DataOrderPage"] = order_arr;
    return OrderPage_json;
}

QString RedisClient::stringify_json(nlohmann::json object_to_be_stringified) {
    std::string tempstdstr = object_to_be_stringified.dump();
    QString tempqstring = tempstdstr.c_str();
    tempqstring.replace("\"", "\\\"");
    tempqstring = QString("\"" + tempqstring + "\"");
    return tempqstring;
}

RedisClient::~RedisClient() {
    if (m_Timer != nullptr) delete m_Timer;
}

void RedisClient::SendCommand(const QString &eCommand) {
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

// this should be a part of the redisclient, not mainwindow
// void RedisClient::on_SubscriptionMessage(QString eChannel, QString eMessage)
//{
//    qDebug()<<"Received Message from subscribed channel "<<eChannel<<": \n"<<eMessage;
//    std::optional<QString> systemMonitor_received = eMessage;
//    if(systemMonitor_received.has_value()){
//        QString temp = systemMonitor_received.value();
//        temp.replace("\\\"", "\"");
//        nlohmann::json parsed = nlohmann::json::parse(temp.toStdString());
//    }
//}

QString RedisClient::on_ReadFromJsonString(std::optional<QString> eJsonString) {
    try {
        QString temp = eJsonString.value();
        temp = temp.replace("\\\"", "\"");
        nlohmann::json parsed = nlohmann::json::parse(temp.toStdString());
        qDebug() << "parsed[DataOrderPage]" << QString(std::string(parsed["DataOrderPage"][0].dump()).c_str());
        if (parsed["DataOrderPage"] != nullptr) {
            // emit send parsed to BuildOrderOverviewTable, initialize instance of OrderList there
            emit ParsedJson(parsed);

            // emit add to List function
            return "";
        }
    } catch (std::exception e) { qDebug() << e.what(); }
}
