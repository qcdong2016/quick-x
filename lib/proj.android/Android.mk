
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := quickcocos2dx
LOCAL_MODULE_FILENAME := libquickcocos2dx

LOCAL_CFLAGS := -DUSE_FILE32API -DCC_LUA_ENGINE_ENABLED=1 $(ANDROID_COCOS2D_BUILD_FLAGS)
LOCAL_EXPORT_CFLAGS := -DUSE_FILE32API -DCC_LUA_ENGINE_ENABLED=1

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_ui_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_lua_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_external_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_spine_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_particle_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_tilemap_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_native_static

ifeq ($(CC_CURL_ENABLED),1)
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_curl_static
endif

include $(BUILD_STATIC_LIBRARY)

$(call import-module, cocos2dx)
$(call import-module, cocos)
$(call import-module, spine)
$(call import-module, audio/android)
$(call import-module, network)
$(call import-module, ui)
$(call import-module, native)
$(call import-module, tilemap)
$(call import-module, particle)
$(call import-module, scripting)
