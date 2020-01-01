#ifndef SMG_SMGCOMMON_H
#define SMG_SMGCOMMON_H
#include <string>

#define smg_FileLineMessage(message) std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " + message

#endif // SMG_SMGCOMMON_H
