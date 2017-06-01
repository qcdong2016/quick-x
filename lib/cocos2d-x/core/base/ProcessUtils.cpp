#include "ProcessUtils.h"
#include <map>
#include <vector>

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include <consoleapi.h>
#endif
#include <sstream>
#include "cocoa/ccUTF8.h"

NS_CC_BEGIN

static std::map<std::string, std::string> arguments;

static bool consoleOpened = false;

static void stringReplace(std::string &strBase, std::string strSrc, std::string strDes)
{
	std::string::size_type pos = 0;
	std::string::size_type srcLen = strSrc.size();
	std::string::size_type desLen = strDes.size();
	pos = strBase.find(strSrc, pos);
	while ((pos != std::string::npos))
	{
		strBase.replace(pos, srcLen, strDes);
		pos = strBase.find(strSrc, (pos + desLen));
	}
}

template<typename T>
static T stringTo(const std::string& s)
{
	std::stringstream ss;
	T ret;
	ss << s;
	ss >> ret;
	return ret;
}

template<typename T>
static std::string toString(T v)
{
	std::stringstream ss;
	ss << v;
	return ss.str();
}



static void defaultArg(const std::string& k, const std::string& v)
{
	if (!hasArguments(k))
		setArgument(k, v);
}

static void prepareDefaultArguments()
{
	defaultArg("zoom", "1.0");
	defaultArg("w", "960");
	defaultArg("h", "640");
}

const std::map<std::string, std::string>& parseArguments(const std::string& cmdLine, bool skipFirst)
{
	std::vector<std::string> opts;

	unsigned cmdStart = 0, cmdEnd = 0;
	bool inCmd = false;
	bool inQuote = false;

	for (unsigned i = 0; i < cmdLine.length(); ++i)
	{
		if (cmdLine[i] == '\"')
			inQuote = !inQuote;
		if (cmdLine[i] == ' ' && !inQuote)
		{
			if (inCmd)
			{
				inCmd = false;
				cmdEnd = i;
				// Do not store the first argument (executable name)
				if (!skipFirst)
					opts.push_back(cmdLine.substr(cmdStart, cmdEnd - cmdStart));
				skipFirst = false;
			}
		}
		else
		{
			if (!inCmd)
			{
				inCmd = true;
				cmdStart = i;
			}
		}
	}
	if (inCmd)
	{
		cmdEnd = cmdLine.length();
		if (!skipFirst)
			opts.push_back(cmdLine.substr(cmdStart, cmdEnd - cmdStart));
	}


	// Strip double quotes from the arguments
	for (unsigned i = 0; i < opts.size(); ++i) {
		stringReplace(opts[i], "\"", "");

		if (opts[i][0] == '-')
		{
			size_t pos = opts[i].find_first_of('=');

			if (pos != std::string::npos) {
				std::string k = opts[i].substr(1, pos-1);
				std::string v = opts[i].substr(pos+1);
				arguments[k] = v;
			}
			else {
				std::string k = opts[i].substr(1);
				arguments[k] = "";
			}
		}
	}

	prepareDefaultArguments();

	return arguments;
}

const std::map<std::string, std::string>& parseArguments(const wchar_t* cmdLine)
{
	const char* data = cc_utf16_to_utf8((const unsigned short*)cmdLine);

	if (!data)
		return parseArguments("");

	std::string s(data);
	delete[] data;

	return parseArguments(s);
}

const std::map<std::string, std::string>& parseArguments(const char* cmdLine)
{
	return parseArguments(std::string(cmdLine));
}

const std::map<std::string, std::string>& getArguments()
{
	return arguments;
}

void setArgument(const std::string& k, const std::string& v)
{
	arguments[k] = v;
}

void setArgument(const std::string& k, int v)
{
	arguments[k] = toString(v);
}

void setArgument(const std::string& k, float v)
{
	arguments[k] = toString(v);
}

std::string getArgument(const std::string& k)
{
	return arguments[k];
}

int getIntArgument(const std::string& k, int def)
{
	if (arguments.find(k) != arguments.end())
		return stringTo<int>(arguments[k]);
	return def;
}

float getFloatArgument(const std::string& k, float def)
{
	if (arguments.find(k) != arguments.end())
		return stringTo<float>(arguments[k]);
	return def;
}

bool hasArguments(const std::string& k)
{
	return arguments.find(k) != arguments.end();
}

std::string getArgumentsString()
{
	std::map<std::string, std::string>::iterator iter;
	std::string str;

	for (iter = arguments.begin(); iter != arguments.end(); iter++)
	{
		str += "-" + iter->first + "=\"" + iter->second + "\" ";
	}

	return str;
}

void openConsoleWindow()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	if (consoleOpened)
		return;

	AllocConsole();

	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);

	consoleOpened = true;
#endif
}

NS_CC_END
