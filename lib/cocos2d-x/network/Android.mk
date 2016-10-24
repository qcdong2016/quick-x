LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := cc_network_static

LOCAL_MODULE_FILENAME := libnetwork

LOCAL_SRC_FILES := \
		CCHTTPRequest.cpp   \
		CCNetwork.cpp   \
		cocos2dx_httprequest_luabinding.cpp

LOCAL_WHOLE_STATIC_LIBRARIES := cc_core_static
LOCAL_WHOLE_STATIC_LIBRARIES += cc_lua_static

ifeq ($(CC_CURL_ENABLED),1)
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_curl_static
endif

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_CFLAGS := -fexceptions

include $(BUILD_STATIC_LIBRARY)

$(call import-module, core)
$(call import-module, scripting)
