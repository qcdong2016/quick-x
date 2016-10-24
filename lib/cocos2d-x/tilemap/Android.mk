LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := cc_tilemap_static

LOCAL_MODULE_FILENAME := libtilemap

LOCAL_SRC_FILES := \
		CCParallaxNode.cpp   \
		CCTileMapAtlas.cpp   \
		CCTMXLayer.cpp   \
		CCTMXObjectGroup.cpp   \
		CCTMXTiledMap.cpp   \
		CCTMXXMLParser.cpp   

LOCAL_WHOLE_STATIC_LIBRARIES := cc_core_static

LOCAL_CFLAGS := -fexceptions

include $(BUILD_STATIC_LIBRARY)

$(call import-module, core)
