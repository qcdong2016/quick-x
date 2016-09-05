
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := cocos_external_static
LOCAL_MODULE_FILENAME := libcocos_external_static

LOCAL_SRC_FILES := \
    extra/crypto/CCCrypto.cpp \
    extra/crypto/base64/libbase64.c \
    extra/network/CCNetwork.cpp \
    extra/platform/android/CCCryptoAndroid.cpp \
    extra/platform/android/CCNativeAndroid.cpp \
    extra/platform/android/CCNetworkAndroid.cpp \
    extra/luabinding/cocos2dx_extra_luabinding.cpp

LOCAL_SRC_FILES += \
    extra/luabinding/cocos2dx_httprequest_luabinding.cpp

ifeq ($(CC_CURL_ENABLED),1)
LOCAL_SRC_FILES += extra/network/CCHTTPRequest.cpp
else
LOCAL_SRC_FILES += extra/platform/android/CCHTTPRequestAndroid.cpp
LOCAL_SRC_FILES += extra/crypto/md5/md5.c
endif

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
                           $(LOCAL_PATH)/extra \
                           $(LOCAL_PATH)/extra/luabinding

LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES)

LOCAL_CFLAGS := -Wno-psabi -DUSE_FILE32API -DCC_LUA_ENGINE_ENABLED=1 $(ANDROID_COCOS2D_BUILD_FLAGS) -std=c++11
LOCAL_EXPORT_CFLAGS := -Wno-psabi -DUSE_FILE32API -DCC_LUA_ENGINE_ENABLED=1

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_lua_static

include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,scripting)
