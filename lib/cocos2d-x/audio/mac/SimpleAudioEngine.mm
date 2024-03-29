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

#include "SimpleAudioEngine.h"
#include "SimpleAudioEngine_objc.h"
#include "engine/CCFileSystem.h"

USING_NS_CC;

static void static_end()
{
    [SimpleAudioEngineOC  end];
}

static void static_preloadBackgroundMusic(const char* pszFilePath)
{
    [[SimpleAudioEngineOC sharedEngine] preloadBackgroundMusic: [NSString stringWithUTF8String: pszFilePath]];
}

static void static_playBackgroundMusic(const char* pszFilePath, bool bLoop)
{
    [[SimpleAudioEngineOC sharedEngine] playBackgroundMusic: [NSString stringWithUTF8String: pszFilePath] loop: bLoop];
}

static void static_stopBackgroundMusic()
{
    [[SimpleAudioEngineOC sharedEngine] stopBackgroundMusic];
}

static void static_pauseBackgroundMusic()
{
     [[SimpleAudioEngineOC sharedEngine] pauseBackgroundMusic];
}

static void static_resumeBackgroundMusic()
{
    [[SimpleAudioEngineOC sharedEngine] resumeBackgroundMusic];
} 

static void static_rewindBackgroundMusic()
{
    [[SimpleAudioEngineOC sharedEngine] rewindBackgroundMusic];
}

static bool static_willPlayBackgroundMusic()
{
    return [[SimpleAudioEngineOC sharedEngine] willPlayBackgroundMusic];
}

static bool static_isBackgroundMusicPlaying()
{
    return [[SimpleAudioEngineOC sharedEngine] isBackgroundMusicPlaying];
}

static float static_getBackgroundMusicVolume()
{
    return [[SimpleAudioEngineOC sharedEngine] backgroundMusicVolume];
}

static void static_setBackgroundMusicVolume(float volume)
{
    volume = MAX( MIN(volume, 1.0), 0 );
    [SimpleAudioEngineOC sharedEngine].backgroundMusicVolume = volume;
}
     
static float static_getEffectsVolume()
{
    return [[SimpleAudioEngineOC sharedEngine] effectsVolume];
}
     
static void static_setEffectsVolume(float volume)
{
    volume = MAX( MIN(volume, 1.0), 0 );
    [SimpleAudioEngineOC sharedEngine].effectsVolume = volume;
}
     
static unsigned int static_playEffect(const char* pszFilePath, bool bLoop)
{
    return [[SimpleAudioEngineOC sharedEngine] playEffect:[NSString stringWithUTF8String: pszFilePath] loop:bLoop];
}
     
static void static_stopEffect(int nSoundId)
{
    [[SimpleAudioEngineOC sharedEngine] stopEffect: nSoundId];
}
     
static void static_preloadEffect(const char* pszFilePath)
{
    [[SimpleAudioEngineOC sharedEngine] preloadEffect: [NSString stringWithUTF8String: pszFilePath]];
}
     
static void static_unloadEffect(const char* pszFilePath)
{
    [[SimpleAudioEngineOC sharedEngine] unloadEffect: [NSString stringWithUTF8String: pszFilePath]];
}

static void static_pauseEffect(unsigned int uSoundId)
{
    [[SimpleAudioEngineOC sharedEngine] pauseEffect: uSoundId];
}

static void static_pauseAllEffects()
{
    [[SimpleAudioEngineOC sharedEngine] pauseAllEffects];
}

static void static_resumeEffect(unsigned int uSoundId)
{
    [[SimpleAudioEngineOC sharedEngine] resumeEffect: uSoundId];
}

static void static_resumeAllEffects()
{
    [[SimpleAudioEngineOC sharedEngine] resumeAllEffects];
}

static void static_stopAllEffects()
{
    [[SimpleAudioEngineOC sharedEngine] stopAllEffects];
}

NS_CC_BEGIN

static SimpleAudioEngine *s_pEngine;

SimpleAudioEngine::SimpleAudioEngine()
{

}

SimpleAudioEngine::~SimpleAudioEngine()
{

}

SimpleAudioEngine* SimpleAudioEngine::sharedEngine()
{
    if (! s_pEngine)
    {
        s_pEngine = new SimpleAudioEngine();
    }
    
    return s_pEngine;
}

void SimpleAudioEngine::end()
{
    if (s_pEngine)
    {
        delete s_pEngine;
        s_pEngine = NULL;
    }
    
    static_end();
}

void SimpleAudioEngine::preloadBackgroundMusic(const char* pszFilePath)
{
    // Changing file path to full path
    std::string fullPath = FileSystem::fullPathOfFile(pszFilePath);
    static_preloadBackgroundMusic(fullPath.c_str());
}

void SimpleAudioEngine::playBackgroundMusic(const char* pszFilePath, bool bLoop)
{
    // Changing file path to full path
    std::string fullPath = FileSystem::fullPathOfFile(pszFilePath);
    static_playBackgroundMusic(fullPath.c_str(), bLoop);
}

void SimpleAudioEngine::stopBackgroundMusic(bool bReleaseData)
{
    static_stopBackgroundMusic();
}

void SimpleAudioEngine::pauseBackgroundMusic()
{
    static_pauseBackgroundMusic();
}

void SimpleAudioEngine::resumeBackgroundMusic()
{
    static_resumeBackgroundMusic();
} 

void SimpleAudioEngine::rewindBackgroundMusic()
{
    static_rewindBackgroundMusic();
}

bool SimpleAudioEngine::willPlayBackgroundMusic()
{
    return static_willPlayBackgroundMusic();
}

bool SimpleAudioEngine::isBackgroundMusicPlaying()
{
    return static_isBackgroundMusicPlaying();
}

float SimpleAudioEngine::getBackgroundMusicVolume()
{
    return static_getBackgroundMusicVolume();
}

void SimpleAudioEngine::setBackgroundMusicVolume(float volume)
{
    static_setBackgroundMusicVolume(volume);
}

float SimpleAudioEngine::getEffectsVolume()
{
    return static_getEffectsVolume();
}

void SimpleAudioEngine::setEffectsVolume(float volume)
{
    static_setEffectsVolume(volume);
}

unsigned int SimpleAudioEngine::playEffect(const char* pszFilePath, bool bLoop)
{
    // Changing file path to full path
    std::string fullPath = FileSystem::fullPathOfFile(pszFilePath);
    return static_playEffect(fullPath.c_str(), bLoop);
}

void SimpleAudioEngine::stopEffect(unsigned int nSoundId)
{
    static_stopEffect(nSoundId);
}

void SimpleAudioEngine::preloadEffect(const char* pszFilePath)
{
    // Changing file path to full path
    std::string fullPath = FileSystem::fullPathOfFile(pszFilePath);
    static_preloadEffect(fullPath.c_str());
}

void SimpleAudioEngine::unloadEffect(const char* pszFilePath)
{
    // Changing file path to full path
    std::string fullPath = FileSystem::fullPathOfFile(pszFilePath);
    static_unloadEffect(fullPath.c_str());
}

void SimpleAudioEngine::pauseEffect(unsigned int uSoundId)
{
    static_pauseEffect(uSoundId);
}

void SimpleAudioEngine::resumeEffect(unsigned int uSoundId)
{
    static_resumeEffect(uSoundId);
}

void SimpleAudioEngine::pauseAllEffects()
{
    static_pauseAllEffects();
}

void SimpleAudioEngine::resumeAllEffects()
{
    static_resumeAllEffects();
}

void SimpleAudioEngine::stopAllEffects()
{
    static_stopAllEffects();
}

NS_CC_END
