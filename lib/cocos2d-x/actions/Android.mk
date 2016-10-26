LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := cc_actions_static
LOCAL_MODULE_FILENAME := libactions
LOCAL_WHOLE_STATIC_LIBRARIES += cc_core_static

LOCAL_SRC_FILES := CCAction.cpp \
					CCActionCamera.cpp \
					CCActionCatmullRom.cpp \
					CCActionEase.cpp \
					CCActionFiniteTime.cpp \
					CCActionGrid.cpp \
					CCActionGrid3D.cpp \
					CCActionInstant.cpp \
					CCActionInterval.cpp \
					CCActionManager.cpp \
					CCActionPageTurn3D.cpp \
					CCActionProgressTimer.cpp \
					CCActionTiledGrid.cpp \
					CCActionTween.cpp


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

$(call import-module, core)
include $(BUILD_STATIC_LIBRARY)
