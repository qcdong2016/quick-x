LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := cc_audio_static

LOCAL_MODULE_FILENAME := libaudio
LOCAL_WHOLE_STATIC_LIBRARIES += cc_core_static

LOCAL_SRC_FILES := android/SimpleAudioEngine.cpp \
                   android/jni/SimpleAudioEngineJni.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_CFLAGS += $(ANDROID_COCOS2D_BUILD_FLAGS)

$(call import-module, core)
include $(BUILD_STATIC_LIBRARY)
