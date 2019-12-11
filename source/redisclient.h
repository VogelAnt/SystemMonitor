#ifndef REDISCLIENT_H
#define REDISCLIENT_H

#include "orderinformation.h"
#include <QObject>
#include <QTimer>
#include <nlohmann/json.hpp>
#include <redistorium/redis.hpp>

#include <iostream>

// Needed for MES Mock
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

    void Subscribe(QString eChannel) {
        m_Redis->SUBSCRIBE(eChannel);
    }

    void Publish(const QString &eChannel, const QString &eMessage) {
        m_Redis->PUBLISH(eChannel, eMessage);
    }

    void LPOP(const QString &list) {
        m_Redis->LPOP(list);
    }

    void LPUSH(const QString &list, const QStringList &listElement) {
        m_Redis->LPUSH(list, listElement);
    }

signals:
    void ReceivedJSONString(QString);
    void SubscriptionMessage(const QString &eChannel, const QString &eMessage);
    void ParsedJson(nlohmann::json);
    void ReceivedNewSubscription(std::optional<QString>);

public slots:
    void SendCommand(const QString &command); // doesn't do anything ????
    void GetReply();
    void on_ReadFromJsonString(QString);

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
