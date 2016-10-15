
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := cocos_lua_static
LOCAL_MODULE_FILENAME := liblua

LOCAL_SRC_FILES := tolua/tolua_event.c \
    tolua/tolua_is.c \
    tolua/tolua_map.c \
    tolua/tolua_push.c \
    tolua/tolua_to.c \
    lua_extensions/cjson/fpconv.c \
    lua_extensions/cjson/lua_cjson.c \
    lua_extensions/cjson/strbuf.c \
    lua_extensions/zlib/lua_zlib.c \
    lua_extensions/socket/auxiliar.c \
    lua_extensions/socket/luasocket_buffer.c \
    lua_extensions/socket/except.c \
    lua_extensions/socket/inet.c \
    lua_extensions/socket/luasocket_io.c \
    lua_extensions/socket/luasocket.c \
    lua_extensions/socket/mime.c \
    lua_extensions/socket/options.c \
    lua_extensions/socket/select.c \
    lua_extensions/socket/serial.c \
    lua_extensions/socket/socket_scripts.c \
    lua_extensions/socket/tcp.c \
    lua_extensions/socket/timeout.c \
    lua_extensions/socket/udp.c \
    lua_extensions/socket/unix.c \
    lua_extensions/socket/usocket.c \
    lua_extensions/filesystem/lfs.c \
    lua_extensions/lpack/lpack.c \
    lua_extensions/lua_extensions.c \
    lua/lapi.c \
    lua/lauxlib.c \
    lua/lbaselib.c \
    lua/lcode.c \
    lua/ldblib.c \
    lua/ldebug.c \
    lua/ldo.c \
    lua/ldump.c \
    lua/lfunc.c \
    lua/lgc.c \
    lua/linit.c \
    lua/liolib.c \
    lua/llex.c \
    lua/lmathlib.c \
    lua/lmem.c \
    lua/loadlib.c \
    lua/lobject.c \
    lua/lopcodes.c \
    lua/loslib.c \
    lua/lparser.c \
    lua/lstate.c \
    lua/lstring.c \
    lua/lstrlib.c \
    lua/ltable.c \
    lua/ltablib.c \
    lua/ltm.c \
    lua/lundump.c \
    lua/lvm.c \
    lua/lzio.c \
    lua/print.c \
    lua_cocos2dx_manual.cpp \
    LuaFunction.cpp \
    LuaTable.cpp \
    CCLuaEngine.cpp \
    CCLuaStack.cpp \
    CCLuaValue.cpp \
    Cocos2dxLuaLoader.cpp \
    tolua_fix.c \
    snapshot.c \
    platform/android/CCLuaJavaBridge.cpp \
    platform/android/org_cocos2dx_lib_Cocos2dxLuaJavaBridge.cpp \
    LuaCocos2d.cpp \
    lua_cocos2dx_cocostudio_manual.cpp \
    LuaCocoStudio.cpp

ifeq ($(CC_SQLITE_ENABLED),1)
LOCAL_SRC_FILES += \
    lua_extensions/lsqlite3/sqlite3.c \
    lua_extensions/lsqlite3/lsqlite3.c
endif


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
                           $(LOCAL_PATH)/lua \
		                   $(LOCAL_PATH)/tolua \
		                   $(LOCAL_PATH)/lua_extensions

LOCAL_C_INCLUDES := $(LOCAL_PATH)/ \
                    $(LOCAL_PATH)/.. \
                    $(LOCAL_PATH)/lua \
                    $(LOCAL_PATH)/tolua \
                    $(LOCAL_PATH)/lua_extensions \
                    $(LOCAL_PATH)/../cocos2dx \
                    $(LOCAL_PATH)/../cocos2dx/include \
                    $(LOCAL_PATH)/../cocos2dx/platform \
                    $(LOCAL_PATH)/../cocos2dx/platform/android \
                    $(LOCAL_PATH)/../cocos2dx/kazmath/include \
                    $(LOCAL_PATH)/../CocosDenshion/include \
                    $(LOCAL_PATH)/../ui \
                    $(LOCAL_PATH)/../external

LOCAL_WHOLE_STATIC_LIBRARIES += cocos_ui_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_external_static

ifeq ($(CC_CURL_ENABLED),1)
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_curl_static
endif

LOCAL_CFLAGS += -DCC_LUA_ENGINE_ENABLED=1 $(ANDROID_COCOS2D_BUILD_FLAGS)

include $(BUILD_STATIC_LIBRARY)

$(call import-module,ui)
