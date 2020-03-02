#ifndef SMG_DEVICES_IDEVICE_H
#define SMG_DEVICES_IDEVICE_H

#include "skills/ISkill.h"

#include <QObject>

enum class PackMLState {
    // stable states: devices are not supposed to change from this state unless triggered by MES or maintenance/errors.
    Idle = 0,
    Running = 2,
    Complete = 12,
    Aborted = 17,
    Stopped = 15,

    // transitional states: devices are supposed to change to stable state shortly.
    Starting = 1,
    Completing = 11,
    Clearing = 18,
    Resetting = 13,
    Stopping = 14,
    Aborting = 16,
};

enum class PackMLStateTransition { Abort, Clear, Reset, Start, Stop };

class IDevice : public QObject {
    Q_OBJECT
public:
    explicit IDevice(QObject *parent = nullptr) : QObject(parent) {}
    ~IDevice() override = default;

    virtual QString Name() const = 0;

    virtual QString NodeId() const = 0;

    virtual unsigned int NameSpace() const = 0;

    virtual std::map<QString, ISkill *> SkillMap() const = 0;
    virtual void AddSkill(QString eSkillName) = 0;
    virtual void SetUrl(QString) = 0;
    virtual PackMLState LastKnownState() const = 0;

    virtual void Connect() = 0;
    virtual void Disconnect() = 0;

    virtual PackMLState GetDeviceState() = 0;
    virtual PackMLState GetSkillState(QString eSkillName) = 0;

    virtual void TriggerSkillStateTransition(QString) = 0;

    // TODO: remove ?
    virtual void TriggerAbort() = 0;
    virtual void TriggerClear() = 0;
    virtual void TriggerReset() = 0;
    virtual void TriggerStart() = 0;
    virtual void TriggerStop() = 0;

    unsigned int Namespace;
};

#endif // SMG_DEVICES_IDEVICE_H
