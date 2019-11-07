#ifndef REDISCLIENT_H
#define REDISCLIENT_H
#include "orderinformation.h"
#include <QObject>
#include <QTimer>
#include <nlohmann/json.hpp>
#include <redistorium/redis.hpp>

#include <iostream>

static std::vector<std::string> ModuleList{"SeedModule", "BufferModule", "SealingModule", "OutputModule", "TransportModule", "TransferModule"};

static std::vector<std::string> SkillList{"Skill1", "Skill2", "Skill3", "Skill4", "Skill5"};

typedef struct {
    std::string moduleName;
    std::string skill;
    std::string status;
} MES_test;

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

    /**
     * @brief MES_json
     * creates mocc JSON from MES
     * @return
     */
    nlohmann::json make_json_MES();

    /**
     * @brief RedisClient::OrderPage_json
     * creates JSON for Orders
     * @return
     */
    nlohmann::json make_json_orderpage();

    void Subscribe(QString eChannel) {
        m_Redis->SUBSCRIBE(eChannel);
    }

    void Publish(const QString &eChannel, const QString &eMessage) {
        m_Redis->PUBLISH(eChannel, eMessage);
    }

signals:
    void sendMESdata(nlohmann::json);
    void ReceivedJSONString(std::optional<QString>);
    void SubscriptionMessage(const QString &eChannel, const QString &eMessage);
    void ParsedJson(nlohmann::json);
    void ReceivedNewSubscription(std::optional<QString>);

public slots:
    void SendCommand(const QString &command); // doesn't do anything ????
    void GetReply();
    QString on_ReadFromJsonString(std::optional<QString>);

private slots:
    /**
     * @brief update
     * simulates timed OrderPage & MES
     */
    void update();

private:
    QTimer *m_Timer = nullptr;
};

#endif // REDISCLIENT_H
// 44:39
