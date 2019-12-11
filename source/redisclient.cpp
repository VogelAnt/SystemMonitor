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

RedisClient::~RedisClient() {
    if (m_Redis != nullptr) { delete m_Redis; }
}
