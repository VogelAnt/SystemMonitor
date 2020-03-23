#ifndef REDISCLIENT_H
#define REDISCLIENT_H

#include "orderinformation.h"
#include <QObject>
#include <QTimer>
#include <nlohmann/json.hpp>
#include <redistorium/redis.hpp>

#include <iostream>
/**
 * @brief The RedisClient class utilizes a redis client based on the QT friendly
 * framework redistorium. It can subscribe, publish and perform the operation LGET
 */
class RedisClient : public QObject {
    Q_OBJECT

public:
    RedisClient(QObject *parent = nullptr);
    virtual ~RedisClient();
    Redistorium::Redis *m_Redis = nullptr;

    /**
     * @brief stringify_json
     * converts JSON object to usable string for redis server
     * @param object_to_be_stringified
     * the nlohman::json object
     * @return
     */
    QString stringify_json(nlohmann::json);

    /**
     * @brief Subscribe
     * Subscribes to specified channel
     * @param eChannel
     */
    void Subscribe(QString eChannel) {
        m_Redis->SUBSCRIBE(eChannel);
    }
    /**
     * @brief Publish
     * Publiushes to specified channel
     * @param eChannel
     * @param eMessage
     */
    void Publish(const QString &eChannel, const QString &eMessage) {
        m_Redis->PUBLISH(eChannel, eMessage);
    }

signals:
    /**
     * @brief ReceivedJSONString
     * Emitted upon initial get and subscription
     */
    void ReceivedJSONString(QString);
    /**
     * @brief SubscriptionMessage
     * Called to pass new messages from subscribed channel
     * @param eChannel
     * @param eMessage
     */
    void SubscriptionMessage(const QString &eChannel, const QString &eMessage);
    /**
     * @brief ParsedJson
     * emitted when data contained in parsed json message is not empty
     * passes data in parsed json message to OrderWidget class
     */
    void ParsedJson(nlohmann::json);
    /**
     * @brief ReceivedNewSubscription
     *
     */

public slots:
    /**
     * @brief on_ReadFromJsonString
     * received JSON in form of QString is parsed into nlohmann json format
     */
    void on_ReadFromJsonString(QString);
};

#endif // REDISCLIENT_H
// 44:39
