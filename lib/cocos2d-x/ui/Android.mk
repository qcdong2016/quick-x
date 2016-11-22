LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := cc_ui_static

LOCAL_MODULE_FILENAME := libui

LOCAL_SRC_FILES := \
    EditBox/CCEditBox.cpp \
    EditBox/CCEditBoxImplAndroid.cpp \
    Layouts/UILayout.cpp \
    Layouts/UILayoutDefine.cpp \
    Layouts/UILayoutParameter.cpp \
    UIWidget.cpp \
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

LOCAL_WHOLE_STATIC_LIBRARIES := cc_core_static
LOCAL_WHOLE_STATIC_LIBRARIES += cc_lua_static

LOCAL_C_INCLUDES = $(LOCAL_EXPORT_C_INCLUDES)

LOCAL_CFLAGS := -fexceptions

include $(BUILD_STATIC_LIBRARY)

$(call import-module, core)
$(call import-module, audio)
$(call import-module, scripting)
