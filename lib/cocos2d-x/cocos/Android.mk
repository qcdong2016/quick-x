LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := cocos_static
LOCAL_MODULE_FILENAME := libcocos

LOCAL_SRC_FILES := ProcessUtils.cpp   \
					RefCounted.cpp


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_C_INCLUDES := $(LOCAL_PATH) \
					$(LOCAL_PATH)/../cocos2dx \
                    $(LOCAL_PATH)/../cocos2dx/include \
                    $(LOCAL_PATH)/../cocos2dx/platform \
                    $(LOCAL_PATH)/../cocos2dx/platform/android \
                    $(LOCAL_PATH)/../cocos2dx/kazmath/include 

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_STATIC_LIBRARY)
