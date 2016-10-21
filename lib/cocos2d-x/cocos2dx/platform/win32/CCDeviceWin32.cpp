#include "platform/CCDevice.h"
#include "CCStdC.h"
#include "CCEGLView.h"
#include "ccUTF8.h"
#include <io.h>

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

NS_CC_END
