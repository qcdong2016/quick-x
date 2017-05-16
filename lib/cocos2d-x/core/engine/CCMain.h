
#ifndef _QUICKX_MAIN_H_
#define _QUICKX_MAIN_H_

#include "CCDirector.h"

namespace cocos2d {

static int runApp(CCApplication* app) {
    SharedPtr<CCDirector> d(new CCDirector(app));
    return d->run();
}
    
}

#define QUICKX_APP(APP) QUICKX_MAIN(runApp(new APP()))


#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#define QUICKX_MAIN(function) \
extern "C" {\
    int SDL_main(int argc, char** argv) \
    { \
        return function; \
    } \
}

#else

#define QUICKX_MAIN(function) \
    int main(int argc, char** argv) \
    { \
        return function; \
    }

#endif





#endif
