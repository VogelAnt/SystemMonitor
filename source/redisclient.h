#ifndef REDISCLIENT_H
#define REDISCLIENT_H
#include <nlohmann/json.hpp>
#include <redistorium/redis.hpp>

#include <QObject>
#include <QTimer>

#include <iostream>

// TODO: clean up and put in a common.h file
static std::vector<std::string> ModuleList{"SeedModule", //
                                           "BufferModule",
                                           "SealingModule",
                                           "OutputModule",
                                           "TransportModule",
                                           "TransferModule"};

static std::vector<std::string> SkillList{"Skill1", //
                                          "Skill2",
                                          "Skill3",
                                          "Skill4",
                                          "Skill5"};

static std::vector<std::string> StateList{"ok", //
                                          "warning",
                                          "error"};

static std::vector<std::string> OrderList{"Order#1",
                                          "Order#2",
                                          "Order#3"
                                          "Order#4",
                                          "Order#5",
                                          "Order#6",
                                          "Order#7",
                                          "Order#8"};

static std::vector<std::string> PriorityList{"Low_Priority", //
                                             "Medium_Priority",
                                             "High_Priority"};

typedef struct {
    std::string moduleName;
    std::string skill;
    std::string status;
} MES_test;

// TODO: add format_json, orderpage_json, MES_json method
class RedisClient : public QObject {
    Q_OBJECT

public:
    RedisClient(QObject *parent = nullptr);
    virtual ~RedisClient();

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
     * creates JSON for MES
     * @return
     */
    nlohmann::json MES_json();

    /**
     * @brief RedisClient::OrderPage_json
     * creates JSON for Orders
     * @return
     */
    nlohmann::json OrderPage_json();

    // TODO: change this so its not public
    Redistorium::Redis *m_Redis = nullptr;

signals:
    void sendMESdata(nlohmann::json);
    void sendOrderPageData(nlohmann::json);

public slots:
    void SendCommand(const QString &command);
    void GetReply();

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
