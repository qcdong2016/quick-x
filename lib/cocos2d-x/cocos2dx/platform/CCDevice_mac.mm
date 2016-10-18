#include "platform/CCDevice.h"

#include <Cocoa/Cocoa.h>
#import "EAGLView.h"

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

std::string CCDevice::getWritablePath()
{
    // save to document folder
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    std::string strRet = [documentsDirectory UTF8String];
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

NS_CC_END
