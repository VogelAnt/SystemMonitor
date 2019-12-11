#include "redisclient.h"

RedisClient::RedisClient(QObject *parent) : QObject(parent) {
    m_Redis = new Redistorium::Redis();
    m_Redis->Connect("localhost", 6379);
    connect(m_Redis, &Redistorium::Redis::SubscriptionMessage, this, &RedisClient::SubscriptionMessage);
    connect(m_Redis, &Redistorium::Redis::Connected, []() { qDebug() << "Connected"; });
    connect(m_Redis, &Redistorium::Redis::Disconnected, []() { qDebug() << "Disconnected"; });
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

void RedisClient::on_ReadFromJsonString(QString eJsonString) {
    std::cout << "reading json string" << std::endl;
    QString temp = eJsonString;
    temp = temp.replace("\\\"", "\"");
    temp.remove(0, 1);
    temp.remove(temp.length() - 1, 1);
    nlohmann::json parsed = nlohmann::json::parse(temp.toStdString());
    // qDebug() << "parsed[DataOrderPage]" << QString(std::string(parsed["DataOrderPage"][0].dump()).c_str());
    if (parsed["id"] != nullptr) { emit ParsedJson(parsed); }
}
