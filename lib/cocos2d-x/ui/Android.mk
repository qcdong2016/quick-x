LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := cocos_ui_static

LOCAL_MODULE_FILENAME := libui

LOCAL_SRC_FILES := \
    EditBox/CCEditBox.cpp \
    EditBox/CCEditBoxImplAndroid.cpp \
    Layouts/UILayout.cpp \
    Layouts/UILayoutDefine.cpp \
    Layouts/UILayoutParameter.cpp \
    UIWidget.cpp \
    UIHelper.cpp \
    UITouchGroup.cpp \
    ScrollWidget/UIListView.cpp \
    ScrollWidget/UIPageView.cpp \
    ScrollWidget/UIScrollView.cpp \
    webview/UIWebView.cpp \
    webview/UIWebViewImpl-android.cpp \
    UIButton.cpp \
    UICheckBox.cpp \
    UIImageView.cpp \
    UILabel.cpp \
    UILabelAtlas.cpp \
    UILabelBMFont.cpp \
    UILoadingBar.cpp \
    UIRichText.cpp \
    UITextField.cpp \
    UISlider.cpp

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_lua_static


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
							$(LOCAL_PATH)/../ \
                    $(LOCAL_PATH)/../cocos2dx \
                    $(LOCAL_PATH)/../cocos2dx/platform \
                    $(LOCAL_PATH)/../cocos2dx/platform/android

LOCAL_C_INCLUDES = $(LOCAL_EXPORT_C_INCLUDES)

LOCAL_CFLAGS := -fexceptions

include $(BUILD_STATIC_LIBRARY)

$(call import-module, cocos2dx)
$(call import-module, audio/android)
$(call import-module, scripting)
