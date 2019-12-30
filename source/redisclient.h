#ifndef REDISCLIENT_H
#define REDISCLIENT_H

#include "orderinformation.h"
#include <QObject>
#include <QTimer>
#include <nlohmann/json.hpp>
#include <redistorium/redis.hpp>

#include <iostream>
// TODO: make m_redis a private member, rework the functions
class RedisClient : public QObject {
Q_OBJECT

public:
    RedisClient(QObject *parent = nullptr);
    virtual ~RedisClient();
    Redistorium::Redis *m_Redis = nullptr;

    /**
     * @brief RedisClient::stringify_json
     * converts JSON object to usable string for redis server
     * @param object_to_be_stringified
     * the nlohman::json object
     * @return
     */
    QString stringify_json(nlohmann::json);

    void Subscribe(QString eChannel) {
        m_Redis->SUBSCRIBE(eChannel);
    }

    void Publish(const QString &eChannel, const QString &eMessage) {
        m_Redis->PUBLISH(eChannel, eMessage);
    }

signals:
    void ReceivedJSONString(QString);
    void SubscriptionMessage(const QString &eChannel, const QString &eMessage);
    void ParsedJson(nlohmann::json);
    void ReceivedNewSubscription(std::optional<QString>);

public slots:
    void on_ReadFromJsonString(QString);
};

#endif // REDISCLIENT_H
// 44:39
