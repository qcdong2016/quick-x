#include "CCDevice.h"
#import <UIKit/UIKit.h>
#import <UIKit/UIAlert.h>

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

void CCDevice::ShowMessageBox(const char * pszMsg, const char * pszTitle)
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

void CCDevice::LogS(const char * str)
{
    printf("%s\n", str);
    fflush(stdout);
}

NS_CC_END
