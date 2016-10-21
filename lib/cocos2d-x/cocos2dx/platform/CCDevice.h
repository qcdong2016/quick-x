#ifndef __CCDEVICE_H__
#define __CCDEVICE_H__

#include "CCPlatformMacros.h"
#include "CCAlertViewDelegate.h"

#if CC_LUA_ENGINE_ENABLED > 0
#include "scripting/CCLuaEngine.h"
#endif

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


enum TargetPlatform
{
	kTargetWindows,
	kTargetLinux,
	kTargetMacOS,
	kTargetAndroid,
	kTargetIphone,
	kTargetIpad,
	kTargetBlackBerry,
	kTargetNaCl,
	kTargetEmscripten,
	kTargetTizen,
	kTargetWinRT,
	kTargetWP8
};


/**
 @js NA
 @lua NA
 */
class CC_DLL CCDevice
{
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


	static bool isIOS64bit();

	/**
	@brief Get target platform
	*/
	static TargetPlatform getTargetPlatform();

	/**
	@brief Get current language config
	@return Current language config
	*/
	static ccLanguageType getCurrentLanguage();

	/**
     *  Gets the DPI of device
     *  @return The DPI of device.
     */
    static int getDPI();
    static std::string getWritablePath();

#pragma mark -
#pragma mark activity indicator

    /** @brief Show activity indicator */
    static void showActivityIndicator(void);

    /** @brief Hide activity indicator */
    static void hideActivityIndicator(void);

#pragma mark -
#pragma mark alert view

    /** @brief Create alert view */
    static void createAlert(const char* title,
                            const char* message,
                            const char* cancelButtonTitle);

    /** @brief Add button to alert view, return button index */
    static int addAlertButton(const char* buttonTitle);

    /** @brief Show alert view */
    static void showAlert(CCAlertViewDelegate* delegate = NULL);
    /** @brief Hide and remove alert view */
    static void cancelAlert(void);

#pragma mark -
#pragma mark OpenUDID

    /** @brief Get OpenUDID value */
    static const std::string getOpenUDID(void);

#pragma mark -
#pragma mark misc

    /** @brief Open a web page in the browser; create an email; or call a phone number. */
    static void openURL(const char* url);

    /** @brief Show alert view, and get user input */
    static const std::string getInputText(const char* title, const char* message, const char* defaultValue);

#pragma mark -
#pragma mark helper

    static const std::string getDeviceName(void);
    static void vibrate();

//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    static void showAlertObjc(void *delegate);
//#endif

#if CC_LUA_ENGINE_ENABLED > 0
    static int addAlertButtonLua(const char* buttonTitle);
    static void showAlertLua(LUA_FUNCTION listener);
#endif

private:
    CCDevice();
};

CC_DLL void CCLog(const char* fmt, ...);

NS_CC_END

#endif /* __CCDEVICE_H__ */
