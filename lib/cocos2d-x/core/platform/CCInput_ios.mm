/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "CCInput.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface AccelerometerDispatcher : NSObject<UIAccelerometerDelegate>
{
}


+ (id) sharedAccelerometerDispather;
- (void) enable : (bool) e;
- (id) init;
- (void) setAccelerometerInterval:(float)interval;

@end

@implementation AccelerometerDispatcher

static AccelerometerDispatcher* s_pAccelerometerDispatcher;


+ (id) sharedAccelerometerDispather
{
    if (s_pAccelerometerDispatcher == nil) {
        s_pAccelerometerDispatcher = [[self alloc] init];
    }
    
    return s_pAccelerometerDispatcher;
}

- (void) enable : (bool) e
{
    if (e) {
        [[UIAccelerometer sharedAccelerometer] setDelegate:self];
    } else {
        [[UIAccelerometer sharedAccelerometer] setDelegate:nil];
    }
    
}

- (id) init
{
    return self;
}

- (void) dealloc
{
    s_pAccelerometerDispatcher = 0;
    [super dealloc];
}

-(void) setAccelerometerInterval:(float)interval
{
    [[UIAccelerometer sharedAccelerometer] setUpdateInterval:interval];
}

- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration
{
    double x = acceleration.x;
    double y = acceleration.y;
    double z = acceleration.z;
    double timeStamp = acceleration.timestamp;
    double x_ = x;
    
    switch ([[UIApplication sharedApplication] statusBarOrientation])
    {
        case UIInterfaceOrientationLandscapeRight:
            x = -y;
            y = x_;
            break;
            
        case UIInterfaceOrientationLandscapeLeft:
            x = y;
            y = -x_;
            break;
            
        case UIInterfaceOrientationPortraitUpsideDown:
            x = -y;
            y = -x_;
            break;
            
        case UIInterfaceOrientationPortrait:
            break;
    }
    
    cocos2d::SubSystem::get<cocos2d::Input>()->onAcceleration(x, y, z, timeStamp);
}

@end


NS_CC_BEGIN
    
void Input::setAccelerationEnable(bool enable)
{
    [[AccelerometerDispatcher sharedAccelerometerDispather] enable:enable];
}

void Input::setAccelerationInterval(float interval)
{
    [[AccelerometerDispatcher sharedAccelerometerDispather] setAccelerometerInterval:interval];
}

NS_CC_END

