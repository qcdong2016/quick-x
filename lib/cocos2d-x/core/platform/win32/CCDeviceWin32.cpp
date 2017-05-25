#include "platform/CCDevice.h"
#include "CCStdC.h"
#include "CCEGLView.h"
#include "cocoa/ccUTF8.h"
#include <io.h>
#include <WinSock2.h>
#include <Iphlpapi.h>
#include "engine/CCEngineEvents.h"
#include "engine/CCDirector.h"
NS_CC_BEGIN

int CCDevice::getDPI()
{
    static int dpi = -1;
    if (dpi == -1)
    {
        HDC hScreenDC = GetDC( NULL );
        int PixelsX = GetDeviceCaps( hScreenDC, HORZRES );
        int MMX = GetDeviceCaps( hScreenDC, HORZSIZE );
        ReleaseDC( NULL, hScreenDC );
        dpi = 254.0f*PixelsX/MMX/10;
    }
    return dpi;
}

void CCDevice::logS(const char* str)
{
    int size = 0;
    unsigned short* s = cc_utf8_to_utf16(str, &size);

    FILE* out = stdout;
    if (!_isatty(_fileno(out)))
        fprintf(out, "%s\n", str);
    else {
        HANDLE stream = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD charsWritten;
        WriteConsoleW(stream, s, size, &charsWritten, 0);
        WriteConsoleW(stream, "\n", 1, &charsWritten, 0);
    }
}

void CCDevice::showMessageBox(const char * pszMsg, const char * pszTitle)
{
    MessageBoxA(NULL, pszMsg, pszTitle, MB_OK);
}

TargetPlatform CCDevice::getTargetPlatform()
{
	return kTargetWindows;
}

ccLanguageType CCDevice::getCurrentLanguage()
{
	ccLanguageType ret = kLanguageEnglish;

	LCID localeID = GetUserDefaultLCID();
	unsigned short primaryLanguageID = localeID & 0xFF;

	switch (primaryLanguageID)
	{
	case LANG_CHINESE:
		ret = kLanguageChinese;
		break;
	case LANG_ENGLISH:
		ret = kLanguageEnglish;
		break;
	case LANG_FRENCH:
		ret = kLanguageFrench;
		break;
	case LANG_ITALIAN:
		ret = kLanguageItalian;
		break;
	case LANG_GERMAN:
		ret = kLanguageGerman;
		break;
	case LANG_SPANISH:
		ret = kLanguageSpanish;
		break;
	case LANG_DUTCH:
		ret = kLanguageDutch;
		break;
	case LANG_RUSSIAN:
		ret = kLanguageRussian;
		break;
	case LANG_KOREAN:
		ret = kLanguageKorean;
		break;
	case LANG_JAPANESE:
		ret = kLanguageJapanese;
		break;
	case LANG_HUNGARIAN:
		ret = kLanguageHungarian;
		break;
	case LANG_PORTUGUESE:
		ret = kLanguagePortuguese;
		break;
	case LANG_ARABIC:
		ret = kLanguageArabic;
		break;
	}

	return ret;
}


void CCDevice::showActivityIndicator(void)
{
}

void CCDevice::hideActivityIndicator(void)
{
}

struct AlertStruct
{
	std::string m_alertViewTitle;
	std::string m_alertViewMessage;

	std::string m_macAddress;
};

static AlertStruct alertStruct;

void CCDevice::createAlert(const char* title,
	const char* message,
	const char* cancelButtonTitle)
{
	alertStruct.m_alertViewTitle = std::string(title ? title : "");
	alertStruct.m_alertViewMessage = std::string(message ? message : "");
}

int CCDevice::addAlertButton(const char* buttonTitle)
{
	return 0;
}

void CCDevice::showAlert()
{
	/*
	wstring title(m_alertViewTitle.begin(), m_alertViewTitle.end());
	wstring message(m_alertViewMessage.begin(), m_alertViewMessage.end());
	int button = MessageBox(NULL, message.c_str(), title.c_str(), MB_OKCANCEL);
	*/
	WCHAR *wszTitleBuf;
	WCHAR *wszMessageBuf;

	int titleBufLen = MultiByteToWideChar(CP_UTF8, 0, alertStruct.m_alertViewTitle.c_str(), -1, NULL, 0);
	int messageBufLen = MultiByteToWideChar(CP_UTF8, 0, alertStruct.m_alertViewMessage.c_str(), -1, NULL, 0);

	wszTitleBuf = new WCHAR[titleBufLen + 1];
	wszMessageBuf = new WCHAR[messageBufLen + 1];

	memset(wszTitleBuf, 0, sizeof(WCHAR)*(titleBufLen + 1));

	memset(wszMessageBuf, 0, sizeof(WCHAR)*(messageBufLen + 1));
	MultiByteToWideChar(CP_UTF8, 0, alertStruct.m_alertViewTitle.c_str(), -1, wszTitleBuf, titleBufLen);
	MultiByteToWideChar(CP_UTF8, 0, alertStruct.m_alertViewMessage.c_str(), -1, wszMessageBuf, messageBufLen);

	int button = MessageBox(NULL, wszMessageBuf, wszTitleBuf, MB_OKCANCEL);

	delete[] wszTitleBuf;
	delete[] wszMessageBuf;

	AlertEvent map;

	if (button == IDOK || button == IDYES)
		map[AlertEvent::buttonIndex] = 0;
	else
		map[AlertEvent::buttonIndex] = 1;

	CCDirector::sharedDirector()->sendEvent<AlertEvent>(map);
}

void CCDevice::cancelAlert(void)
{
}

static bool getMacAddress(std::string& macstring)
{
	bool ret = false;
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
	if (pAdapterInfo == NULL)
	{
		return false;
	}

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
		if (pAdapterInfo == NULL)
		{
			return false;
		}
	}

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR)
	{
		for (PIP_ADAPTER_INFO pAdapter = pAdapterInfo; pAdapter != NULL; pAdapter = pAdapter->Next)
		{
			if (pAdapter->Type != MIB_IF_TYPE_ETHERNET)
			{
				continue;
			}

			if (pAdapter->AddressLength != 6)
			{
				continue;
			}

			char macBuf[32];
			sprintf(macBuf, "%02X-%02X-%02X-%02X-%02X-%02X",
				int(pAdapter->Address[0]),
				int(pAdapter->Address[1]),
				int(pAdapter->Address[2]),
				int(pAdapter->Address[3]),
				int(pAdapter->Address[4]),
				int(pAdapter->Address[5]));
			macstring = macBuf;
			ret = true;
			break;
		}
	}

	free(pAdapterInfo);
	return ret;
}

const std::string CCDevice::getOpenUDID(void)
{
	if (alertStruct.m_macAddress.length() <= 0)
	{
		if (!getMacAddress(alertStruct.m_macAddress))
		{
			alertStruct.m_macAddress = "udid-fixed-1234567890";
		}
	}

	return alertStruct.m_macAddress;
}

void CCDevice::openURL(const char* url)
{
	if (url == NULL || *url == '\0')
		return /*false*/;

	// Success when result code > 32
	int len = MultiByteToWideChar(CP_ACP, 0, url, -1, NULL, 0);
	wchar_t* wurl = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, url, -1, wurl, len);
	int r = (int)ShellExecute(NULL, NULL, wurl, NULL, NULL, SW_SHOWNORMAL);
	CC_SAFE_DELETE_ARRAY(wurl);
	return/* (r > 32)*/;
}

typedef struct {
	std::string title;
	std::string message;
	std::string value;
} CCNativeWin32InputBoxStruct;

const std::string CCDevice::getInputText(const char* title, const char* message, const char* defaultValue)
{
	HWND handle;// = CCEGLView::sharedOpenGLView()->getHWnd();

	CCNativeWin32InputBoxStruct inputbox;
	inputbox.title = std::string(title ? title : "INPUT TEXT");
	inputbox.message = std::string(message ? message : "INPUT TEXT, PRESS ENTER");
	inputbox.value = std::string(defaultValue ? defaultValue : "");
	SendMessage(handle, WM_CUT, 998, (LPARAM)&inputbox);
	return inputbox.value;
}

const std::string CCDevice::getDeviceName(void)
{
	return "Win32";
}

void CCDevice::vibrate()
{
}

std::string CCDevice::getResourcePath() {
	return "";
}


NS_CC_END
