LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := cocos_network_static

LOCAL_MODULE_FILENAME := libnetwork

LOCAL_SRC_FILES := \
		CCHTTPRequest.cpp   \
		CCNetwork.cpp   \
		cocos2dx_httprequest_luabinding.cpp

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_lua_static

ifeq ($(CC_CURL_ENABLED),1)
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_curl_static
endif

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
							$(LOCAL_PATH)/../  \
		                    $(LOCAL_PATH)/../cocos2dx \
		                    $(LOCAL_PATH)/../cocos2dx/include \
		                    $(LOCAL_PATH)/../cocos2dx/platform \
		                    $(LOCAL_PATH)/../cocos2dx/platform/android \
		                    $(LOCAL_PATH)/../cocos2dx/kazmath/include

LOCAL_C_INCLUDES = $(LOCAL_EXPORT_C_INCLUDES)

LOCAL_CFLAGS := -fexceptions

include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,scripting)
