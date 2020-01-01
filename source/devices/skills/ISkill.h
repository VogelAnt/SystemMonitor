#ifndef SMG_DEVICES_SKILLS_ISKILL_H
#define SMG_DEVICES_SKILLS_ISKILL_H
#include <QString>

class ISkill {
public:
    ISkill(const QString &eName, const QString &eParentNodeId) : Name(eName), NodeId(eParentNodeId + ".skill." + eName) {}
    const QString Name;
    const QString NodeId;
};

#endif // SMG_DEVICES_SKILLS_ISKILL_H
