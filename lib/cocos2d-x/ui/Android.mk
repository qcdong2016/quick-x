LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := cocos_extension_static

LOCAL_MODULE_FILENAME := libextension

LOCAL_SRC_FILES := \
    GUI/CCEditBox/CCEditBox.cpp \
    GUI/CCEditBox/CCEditBoxImplAndroid.cpp \
    GUI/BaseClasses/UIWidget.cpp \
    GUI/Layouts/UILayout.cpp \
    GUI/Layouts/UILayoutDefine.cpp \
    GUI/Layouts/UILayoutParameter.cpp \
    GUI/System/CocosGUI.cpp \
    GUI/System/UIHelper.cpp \
    GUI/System/UITouchGroup.cpp \
    GUI/UIWidgets/ScrollWidget/UIListView.cpp \
    GUI/UIWidgets/ScrollWidget/UIPageView.cpp \
    GUI/UIWidgets/ScrollWidget/UIScrollView.cpp \
    GUI/UIWidgets/UIButton.cpp \
    GUI/UIWidgets/UICheckBox.cpp \
    GUI/UIWidgets/UIImageView.cpp \
    GUI/UIWidgets/UILabel.cpp \
    GUI/UIWidgets/UILabelAtlas.cpp \
    GUI/UIWidgets/UILabelBMFont.cpp \
    GUI/UIWidgets/UILoadingBar.cpp \
    GUI/UIWidgets/UIRichText.cpp \
    GUI/UIWidgets/UITextField.cpp \
    GUI/UIWidgets/UISlider.cpp

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_lua_static

ifeq ($(CC_CURL_ENABLED),1)
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_curl_static
endif

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
                           $(LOCAL_PATH)/GUI/CCEditBox \
                           $(LOCAL_PATH)/network \
                           $(LOCAL_PATH)/Components \
		                    $(LOCAL_PATH)/../cocos2dx \
		                    $(LOCAL_PATH)/../cocos2dx/include \
		                    $(LOCAL_PATH)/../cocos2dx/platform \
		                    $(LOCAL_PATH)/../cocos2dx/platform/android \
		                    $(LOCAL_PATH)/../cocos2dx/kazmath/include

LOCAL_C_INCLUDES = $(LOCAL_EXPORT_C_INCLUDES)

LOCAL_CFLAGS := -fexceptions

include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,scripting)
