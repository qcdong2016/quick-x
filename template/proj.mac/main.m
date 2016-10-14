
#import <Cocoa/Cocoa.h>
#import "AppController.h"

int main(int argc, char *argv[])
{
    NSAutoreleasePool* p = [[NSAutoreleasePool alloc] init];
    
    NSApplication* app = [[NSApplication sharedApplication] autorelease];
    
    [app setDelegate: [[AppController alloc] autorelease]];
    [app run];
    
    [p release];
    
    return EXIT_SUCCESS;
}
