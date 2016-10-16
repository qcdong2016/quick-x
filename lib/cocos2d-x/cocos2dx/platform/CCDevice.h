#ifndef __CCDEVICE_H__
#define __CCDEVICE_H__

#include "CCPlatformMacros.h"
#include <string>

NS_CC_BEGIN


/**
 * @addtogroup platform
 * @{
 */


/**
 @brief Enum the language type supported now
 */
typedef enum LanguageType
{
    kLanguageEnglish = 0,
    kLanguageChinese,
    kLanguageFrench,
    kLanguageItalian,
    kLanguageGerman,
    kLanguageSpanish,
    kLanguageDutch,
    kLanguageRussian,
    kLanguageKorean,
    kLanguageJapanese,
    kLanguageHungarian,
    kLanguagePortuguese,
    kLanguageArabic,
    kLanguageChineseTW,
} ccLanguageType;

// end of platform group
/// @}


/**
 @js NA
 @lua NA
 */
class CC_DLL CCDevice
{
private:
    CCDevice();
public:

    /**
     @brief Output Debug message.
     */
    static void log(const char * fmt, ...);
	static void logV(const char * fmt, va_list args);
	static void logS(const char * str);

    /**
     @brief Pop out a message box
     */
    static void showMessageBox(const char * pszMsg, const char * pszTitle);

    /**
     *  Gets the DPI of device
     *  @return The DPI of device.
     */
    static int getDPI();
    static std::string getWritablePath();
};

CC_DLL void CCLog(const char* fmt, ...);

NS_CC_END

#endif /* __CCDEVICE_H__ */
