#include "CCDevice.h"
#import <UIKit/UIKit.h>
#import <UIKit/UIAlert.h>
#import <AudioToolbox/AudioServices.h>
#import "CCNativeIOS.h"
#import "openudid/OpenUDIDIOS.h"


NS_CC_BEGIN

int CCDevice::getDPI()
{
    static int dpi = -1;

    if (dpi == -1)
    {
        float scale = 1.0f;

        if ([[UIScreen mainScreen] respondsToSelector:@selector(scale)]) {
            scale = [[UIScreen mainScreen] scale];
        }

        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            dpi = 132 * scale;
        } else if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
            dpi = 163 * scale;
        } else {
            dpi = 160 * scale;
        }
    }
    return dpi;
}

void CCDevice::showMessageBox(const char * pszMsg, const char * pszTitle)
{
    NSString * title = (pszTitle) ? [NSString stringWithUTF8String : pszTitle] : nil;
    NSString * msg = (pszMsg) ? [NSString stringWithUTF8String : pszMsg] : nil;
    UIAlertView * messageBox = [[UIAlertView alloc] initWithTitle: title
                                                          message: msg
                                                         delegate: nil
                                                cancelButtonTitle: @"OK"
                                                otherButtonTitles: nil];
    [messageBox autorelease];
    [messageBox show];
}

std::string CCDevice::getWritablePath()
{
    // save to document folder
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
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
    else if ([languageCode isEqualToString:@"pt"]){
        ret = kLanguagePortuguese;
    }
    else if ([languageCode isEqualToString:@"ar"]){
        ret = kLanguageArabic;
    }

    return ret;
}

TargetPlatform CCDevice::getTargetPlatform()
{
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) // idiom for iOS <= 3.2, otherwise: [UIDevice userInterfaceIdiom] is faster.
    {
        return kTargetIpad;
    }
    else
    {
        return kTargetIphone;
    }
}

#pragma mark -
#pragma mark activity indicator

void CCDevice::showActivityIndicator(void)
{
    [[CCNativeIOS sharedInstance] showActivityIndicator:UIActivityIndicatorViewStyleWhiteLarge];
}

void CCDevice::hideActivityIndicator(void)
{
    [[CCNativeIOS sharedInstance] hideActivityIndicator];
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
    [[CCNativeIOS sharedInstance] createAlertView:title_
                                       andMessage:message_
                             andCancelButtonTitle:cancelButtonTitle_];
}

int CCDevice::addAlertButton(const char* buttonTitle)
{
    NSString *buttonTitle_ = [NSString stringWithUTF8String:buttonTitle ? buttonTitle : "Button"];
    return [[CCNativeIOS sharedInstance] addAlertButton:buttonTitle_];
}

void CCDevice::showAlert()
{
    [[CCNativeIOS sharedInstance] showAlertView];
}

void CCDevice::cancelAlert(void)
{
    [[CCNativeIOS sharedInstance] cancelAlertView];
}

void CCDevice::openURL(const char* url)
{
    if (!url) return;
    NSURL *nsurl = [NSURL URLWithString:[NSString stringWithCString:url encoding:NSUTF8StringEncoding]];
    [[UIApplication sharedApplication] openURL:nsurl];
}

const std::string CCDevice::getInputText(const char* title, const char* message, const char* defaultValue)
{
    CCLOG("CCDevice::getInputText() - not support this platform.");
    return std::string("");
}

#pragma mark -
#pragma mark OpenUDID

const std::string CCDevice::getOpenUDID(void)
{
    return std::string([[OpenUDIDIOS value] cStringUsingEncoding:NSUTF8StringEncoding]);
}

const std::string CCDevice::getDeviceName(void)
{
    UIDevice *device = [UIDevice currentDevice];
    return [[device name] cStringUsingEncoding:NSUTF8StringEncoding];
}

void CCDevice::vibrate()
{
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}

std::string CCDevice::getResourcePath()
{
    return [[NSBundle mainBundle] resourcePath].UTF8String;
}


NS_CC_END
