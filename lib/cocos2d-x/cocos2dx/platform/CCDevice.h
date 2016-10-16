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
    static void Log(const char * pszFormat, ...) CC_FORMAT_PRINTF(1, 2);
    
    /**
     * lua can not deal with ...
     */
    static void LuaLog(const char * pszFormat);
    
    /**
     @brief Pop out a message box
     */
    static void MessageBox(const char * pszMsg, const char * pszTitle);

    /**
     *  Gets the DPI of device
     *  @return The DPI of device.
     */
    static int getDPI();
    static std::string getWritablePath();
};


NS_CC_END


CC_DLL void CCLog(const char* pszFormat, ...);

#endif /* __CCDEVICE_H__ */
