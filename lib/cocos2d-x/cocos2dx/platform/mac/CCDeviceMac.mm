#include "platform/CCDevice.h"

#include <Cocoa/Cocoa.h>
#include <sys/sysctl.h>
#import "EAGLView.h"
#import "CCNativeMac.h"
#import "openudid/OpenUDIDMac.h"

NS_CC_BEGIN

int CCDevice::getDPI()
{
//TODO:
    return 160;
}

void CCDevice::showMessageBox(const char *pszMsg, const char *pszTitle)
{
    NSString * title = (pszTitle) ? [NSString stringWithUTF8String : pszTitle] : nil;
    NSString * msg = (pszMsg) ? [NSString stringWithUTF8String : pszMsg] : nil;

    NSAlert *alert = [[[NSAlert alloc] init] autorelease];
    [alert addButtonWithTitle:@"OK"];
    [alert setMessageText:msg];
    [alert setInformativeText:title];
    [alert setAlertStyle:NSWarningAlertStyle];

    NSWindow *window = [[EAGLView sharedEGLView] window];
    [alert beginSheetModalForWindow:window
                      modalDelegate:[window delegate]
                     didEndSelector:nil
                        contextInfo:nil];
}

// @return /Users/{WHOAMI}/Library/Application Support/{bundleID}
std::string CCDevice::getWritablePath()
{
    // https://developer.apple.com/library/ios/documentation/FileManagement/Conceptual/FileSystemProgrammingGuide/ManagingFIlesandDirectories/ManagingFIlesandDirectories.html

    NSString* bundleID = [[NSBundle mainBundle] bundleIdentifier];
    NSFileManager*fm = [NSFileManager defaultManager];
    NSURL*    dirPath = nil;

    // Find the application support directory in the home directory.
    NSArray* appSupportDir = [fm URLsForDirectory:NSApplicationSupportDirectory
                                        inDomains:NSUserDomainMask];
    if ([appSupportDir count] > 0)
    {
        // Append the bundle ID to the URL for the
        // Application Support directory
        dirPath = [[appSupportDir objectAtIndex:0] URLByAppendingPathComponent:bundleID];

        // If the directory does not exist, this method creates it.
        // This method is only available in OS X v10.7 and iOS 5.0 or later.
        NSError*    theError = nil;
        if (![fm createDirectoryAtURL:dirPath withIntermediateDirectories:YES
                           attributes:nil error:&theError])
        {
            // TODO - Handle the error.
            return "";
        }
    }

    std::string strRet = [dirPath fileSystemRepresentation];
    strRet.append("/");
    return strRet;
}

void CCDevice::logS(const char * str)
{
    printf("%s\n", str);
    fflush(stdout);
}

TargetPlatform CCDevice::getTargetPlatform()
{
    return kTargetMacOS;
}

ccLanguageType CCDevice::getCurrentLanguage()
{
    // get the current language and country config
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
    NSString *currentLanguage = [languages objectAtIndex:0];

    // get the current language code.(such as English is "en", Chinese is "zh" and so on)
    NSDictionary* temp = [NSLocale componentsFromLocaleIdentifier:currentLanguage];
    NSString * languageCode = [temp objectForKey:NSLocaleLanguageCode];

    ccLanguageType ret = kLanguageEnglish;
    if ([languageCode isEqualToString:@"zh"])
    {
        NSString * scriptCode = [temp objectForKey:NSLocaleScriptCode];
        if ([scriptCode isEqualToString:@"Hant"])
            ret = kLanguageChineseTW;
        else
            ret = kLanguageChinese;
    }
    else if ([languageCode isEqualToString:@"en"])
    {
        ret = kLanguageEnglish;
    }
    else if ([languageCode isEqualToString:@"fr"]){
        ret = kLanguageFrench;
    }
    else if ([languageCode isEqualToString:@"it"]){
        ret = kLanguageItalian;
    }
    else if ([languageCode isEqualToString:@"de"]){
        ret = kLanguageGerman;
    }
    else if ([languageCode isEqualToString:@"es"]){
        ret = kLanguageSpanish;
    }
    else if ([languageCode isEqualToString:@"nl"]){
        ret = kLanguageDutch;
    }
    else if ([languageCode isEqualToString:@"ru"]){
        ret = kLanguageRussian;
    }
    else if ([languageCode isEqualToString:@"ko"]){
        ret = kLanguageKorean;
    }
    else if ([languageCode isEqualToString:@"ja"]){
        ret = kLanguageJapanese;
    }
    else if ([languageCode isEqualToString:@"hu"]){
        ret = kLanguageHungarian;
    }
    else if ([languageCode isEqualToString:@"pt"])
    {
        ret = kLanguagePortuguese;
    }
    else if ([languageCode isEqualToString:@"ar"])
    {
        ret = kLanguageArabic;
    }

    return ret;
}

#pragma mark -
#pragma mark activity indicator

void CCDevice::showActivityIndicator(void)
{
    [[CCNativeMac sharedInstance] showActivityIndicator];
}

void CCDevice::hideActivityIndicator(void)
{
    [[CCNativeMac sharedInstance] hideActivityIndicator];
}

#pragma mark -
#pragma mark alert view

void CCDevice::createAlert(const char* title,
                           const char* message,
                           const char* cancelButtonTitle)
{
    NSString *title_ = [NSString stringWithUTF8String:title ? title : ""];
    NSString *message_ = [NSString stringWithUTF8String:message ? message : ""];
    NSString *cancelButtonTitle_ = cancelButtonTitle ? [NSString stringWithUTF8String:cancelButtonTitle] : nil;
    [[CCNativeMac sharedInstance] createAlertView:title_
                                         andMessage:message_
                               andCancelButtonTitle:cancelButtonTitle_];
}

int CCDevice::addAlertButton(const char* buttonTitle)
{
    NSString *buttonTitle_ = [NSString stringWithUTF8String:buttonTitle ? buttonTitle : "Button"];
    return (int)[[CCNativeMac sharedInstance] addAlertButton:buttonTitle_];
}

#if CC_LUA_ENGINE_ENABLED > 0
int CCDevice::addAlertButtonLua(const char* buttonTitle)
{
    return addAlertButton(buttonTitle) + 1;
}
#endif

void CCDevice::showAlert(CCAlertViewDelegate* delegate)
{
    [[CCNativeMac sharedInstance] showAlertViewWithDelegate:delegate];
}

#if CC_LUA_ENGINE_ENABLED > 0
void CCDevice::showAlertLua(LUA_FUNCTION listener)
{
    [[CCNativeMac sharedInstance] showAlertViewWithLuaListener:listener];
}
#endif

void CCDevice::cancelAlert(void)
{
    [[CCNativeMac sharedInstance] cancelAlertView];
}


#pragma mark -
#pragma mark misc

void CCDevice::openURL(const char* url)
{
    if (!url) return;
    NSURL *nsurl = [NSURL URLWithString:[NSString stringWithCString:url encoding:NSUTF8StringEncoding]];
    [[NSWorkspace sharedWorkspace] openURL:nsurl];
}

const std::string CCDevice::getInputText(const char* title, const char* message, const char* defaultValue)
{
    NSString *title_ = [NSString stringWithUTF8String:title ? title : ""];
    NSString *message_ = [NSString stringWithUTF8String:message ? message : ""];
    NSString *defaultValue_ = [NSString stringWithUTF8String:defaultValue ? defaultValue : ""];
    NSString *input = [[CCNativeMac sharedInstance] getInputText:title_
                                                           message:message_
                                                      defaultValue:defaultValue_];
    return std::string([input cStringUsingEncoding:NSUTF8StringEncoding]);
}


#pragma mark -
#pragma mark OpenUDID

const std::string CCDevice::getOpenUDID(void)
{
    return std::string([[OpenUDIDMac value] cStringUsingEncoding:NSUTF8StringEncoding]);
}

const std::string CCDevice::getDeviceName(void)
{
    size_t len = 0;
    sysctlbyname("hw.model", NULL, &len, NULL, 0);
    if (len)
    {
        char* model = static_cast<char*>(malloc(len * sizeof(char)));
        sysctlbyname("hw.model", model, &len, NULL, 0);
        std::string modelStr(model);
        free(model);
        return modelStr;
    }

    CCLOG("CCDevice::getDeviceName() not support on this platform.");
    return std::string("");
}

void CCDevice::vibrate()
{
    CCLOG("CCDevice::vibrate() not support on this platform.");
}

NS_CC_END
