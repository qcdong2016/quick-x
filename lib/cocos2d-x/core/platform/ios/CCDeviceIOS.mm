#include "CCDevice.h"
#import <UIKit/UIKit.h>
#import <UIKit/UIAlert.h>
#import <AudioToolbox/AudioServices.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>




USING_NS_CC;

@interface CCNativeIOS : NSObject < UIAlertViewDelegate >
{
    UIActivityIndicatorView *activityIndicatorView_;
    
    UIAlertView *alertView_;
}

+ (CCNativeIOS *)sharedInstance;


#pragma mark -
#pragma mark activity indicator

- (void)showActivityIndicator:(UIActivityIndicatorViewStyle)style;
- (void)hideActivityIndicator;


#pragma mark -
#pragma mark alert view

- (void)createAlertView:(NSString *)title
             andMessage:(NSString *)message
   andCancelButtonTitle:(NSString *)cancelButtonTitle;
- (NSInteger)addAlertButton:(NSString *)buttonTitle;
- (void)showAlertView;
- (void)removeAlertView;
- (void)cancelAlertView;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
- (void)showAlertViewWithObjcDelegate:(id<UIAlertViewDelegate>)delegate;
#endif


#pragma mark -
#pragma mark UIAlertView delegates

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex;

@end

#ifndef utf8cstr
#define utf8cstr(nsstr) (nsstr ? [nsstr cStringUsingEncoding:NSUTF8StringEncoding] : "")
#endif

@implementation CCNativeIOS

static CCNativeIOS *s_sharedInstance;

+ (CCNativeIOS *)sharedInstance
{
    if (!s_sharedInstance)
    {
        s_sharedInstance = [[CCNativeIOS alloc] init];
    }
    return s_sharedInstance;
}

- (void)dealloc
{
    [self hideActivityIndicator];
    [self removeAlertView];
#if CC_LUA_ENGINE_ENABLED > 0
    [self removeAlertViewLuaListener];
#endif
    [super dealloc];
    s_sharedInstance = nil;
}


#pragma mark -
#pragma mark activity indicator

- (void)showActivityIndicator:(UIActivityIndicatorViewStyle)style
{
    if (activityIndicatorView_)
    {
        CCLOG("[CCNativeIOS] ERR, showActivityIndicator() activity indicator already visible");
        return;
    }
    
    CCLOG("[CCNativeIOS] showActivityIndicator()");
    activityIndicatorView_ = [UIActivityIndicatorView  alloc];
    [activityIndicatorView_ initWithActivityIndicatorStyle:style];
    [activityIndicatorView_ autorelease];
    [activityIndicatorView_ retain];
    
    NSInteger count = [UIApplication sharedApplication].windows.count;
    UIWindow* topWindow = [[UIApplication sharedApplication].windows objectAtIndex:count - 1];
    [topWindow addSubview: activityIndicatorView_];
    activityIndicatorView_.center = topWindow.center;
    [activityIndicatorView_ startAnimating];
}

- (void)hideActivityIndicator
{
    if (!activityIndicatorView_)
    {
        CCLOG("[CCNativeIOS] ERR, hideActivityIndicator() activity indicator not visible");
        return;
    }
    
    CCLOG("[CCNativeIOS] hideActivityIndicator()");
    [activityIndicatorView_ removeFromSuperview];
    [activityIndicatorView_ release];
    activityIndicatorView_ = nil;
}


#pragma mark -
#pragma mark alert view

- (void)createAlertView:(NSString *)title
             andMessage:(NSString *)message
   andCancelButtonTitle:(NSString *)cancelButtonTitle
{
    if (alertView_)
    {
        CCLOG("[CCNativeIOS] ERR, createAlertView() alert view already exists");
        return;
    }
    
    CCLOG("[CCNativeIOS] createAlertView() title: %s, message: %s, cancelButtonTitle: %s",
          utf8cstr(title), utf8cstr(message), utf8cstr(cancelButtonTitle));
    alertView_ = [[UIAlertView alloc] initWithTitle:title
                                            message:message
                                           delegate:self
                                  cancelButtonTitle:cancelButtonTitle
                                  otherButtonTitles:nil];
}

- (NSInteger)addAlertButton:(NSString *)buttonTitle
{
    if (!alertView_)
    {
        CCLOG("[CCNativeIOS] ERR, addAlertButton() alert view not exists");
        return 0;
    }
    
    CCLOG("[CCNativeIOS] addAlertButton() buttonTitle: %s", utf8cstr(buttonTitle));
    return [alertView_ addButtonWithTitle:buttonTitle];
}

- (void)showAlertView
{
    if (!alertView_)
    {
        CCLOG("[CCNativeIOS] ERR, showAlertViewWithDelegate() alert view not exists");
        return;
    }
    
    CCLOG("[CCNativeIOS] showAlertViewWithDelegate()");
    //    alertViewDelegates_ = delegate;
    [alertView_ show];
}

- (void)removeAlertView
{
    if (!alertView_)
    {
        CCLOG("[CCNativeIOS] ERR, removeAlertView() alert view not exists");
        return;
    }
    
    CCLOG("[CCNativeIOS] removeAlertView()");
    [alertView_ release];
    alertView_ = nil;

    //    alertViewDelegates_ = nil;
}

- (void)cancelAlertView
{
    if (!alertView_)
    {
        CCLOG("[CCNativeIOS] ERR, cancelAlertView() alert view not exists");
        return;
    }
    
    CCLOG("[CCNativeIOS] cancelAlertView()");
    [alertView_ dismissWithClickedButtonIndex:0 animated:YES];
    [self removeAlertView];
}


#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
- (void)showAlertViewWithObjcDelegate:(id<UIAlertViewDelegate>)delegate
{
    if (!alertView_)
    {
        CCLOG("[CCNativeIOS] ERR, showAlertViewWithDelegate() alert view not exists");
        return;
    }
    
    CCLOG("[CCNativeIOS] showAlertViewWithObjcDelegate()");
    [alertView_ setDelegate:delegate];
    [alertView_ show];
}
#endif

#pragma mark -
#pragma mark UIAlertView delegates

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    [self removeAlertView];
}

@end

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
