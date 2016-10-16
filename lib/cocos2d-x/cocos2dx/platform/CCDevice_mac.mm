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

NS_CC_END
