LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := cocos_tilemap_static

LOCAL_MODULE_FILENAME := libtilemap

LOCAL_SRC_FILES := \
		CCParallaxNode.cpp   \
		CCTileMapAtlas.cpp   \
		CCTMXLayer.cpp   \
		CCTMXObjectGroup.cpp   \
		CCTMXTiledMap.cpp   \
		CCTMXXMLParser.cpp   

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
							$(LOCAL_PATH)/../  \
		                    $(LOCAL_PATH)/../cocos2dx/scripting \
		                    $(LOCAL_PATH)/../cocos2dx \
		                    $(LOCAL_PATH)/../cocos2dx/platform \
		                    $(LOCAL_PATH)/../cocos2dx/platform/android \
		                    $(LOCAL_PATH)/../cocos2dx/kazmath/include

LOCAL_C_INCLUDES = $(LOCAL_EXPORT_C_INCLUDES)

LOCAL_CFLAGS := -fexceptions

include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx)
