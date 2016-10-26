
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := quickcocos2dx
LOCAL_MODULE_FILENAME := libquickcocos2dx

LOCAL_CFLAGS := -DUSE_FILE32API -DCC_LUA_ENGINE_ENABLED=1 $(ANDROID_COCOS2D_BUILD_FLAGS)
LOCAL_EXPORT_CFLAGS := -DUSE_FILE32API -DCC_LUA_ENGINE_ENABLED=1

LOCAL_WHOLE_STATIC_LIBRARIES := cc_core_static
LOCAL_WHOLE_STATIC_LIBRARIES += cc_actions_static
LOCAL_WHOLE_STATIC_LIBRARIES += cc_audio_static
LOCAL_WHOLE_STATIC_LIBRARIES += cc_ui_static
LOCAL_WHOLE_STATIC_LIBRARIES += cc_lua_static
LOCAL_WHOLE_STATIC_LIBRARIES += cc_external_static
LOCAL_WHOLE_STATIC_LIBRARIES += cc_spine_static
LOCAL_WHOLE_STATIC_LIBRARIES += cc_network_static
LOCAL_WHOLE_STATIC_LIBRARIES += cc_particle_static
LOCAL_WHOLE_STATIC_LIBRARIES += cc_tilemap_static

ifeq ($(CC_CURL_ENABLED),1)
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_curl_static
endif

include $(BUILD_STATIC_LIBRARY)

$(call import-module, core)
$(call import-module, actions)
$(call import-module, spine)
$(call import-module, audio)
$(call import-module, network)
$(call import-module, ui)
$(call import-module, tilemap)
$(call import-module, particle)
$(call import-module, scripting)
