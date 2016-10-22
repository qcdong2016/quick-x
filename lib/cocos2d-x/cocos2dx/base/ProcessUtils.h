#ifndef _PROCESSUTIL_H_
#define _PROCESSUTIL_H_

#include "CCPlatformMacros.h"
#include <string>
#include <map>

NS_CC_BEGIN

const std::map<std::string, std::string>& parseArguments(const std::string& cmdLine, bool skipFirst = true);
const std::map<std::string, std::string>& parseArguments(const wchar_t* cmdLine);
const std::map<std::string, std::string>& parseArguments(const char* cmdLine);
const std::map<std::string, std::string>& getArguments();
void setArgument(const std::string& k, const std::string& v);
void setArgument(const std::string& k, int v);
void setArgument(const std::string& k, float v);
std::string getArgument(const std::string& k);
int getIntArgument(const std::string& k, int def = 0);
float getFloatArgument(const std::string& k, float def = 0.0f);

bool hasArguments(const std::string& k);

std::string getArgumentsString();

void openConsoleWindow();

NS_CC_END


#endif // !_PROCESSUTIL_H_
