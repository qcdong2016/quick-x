LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cc_core_static

LOCAL_MODULE_FILENAME := libcore

LOCAL_SRC_FILES := \
    base/ID.cpp \
    base/MathDefs.cpp \
    base/ProcessUtils.cpp \
    base/RefCounted.cpp \
    base/Variant.cpp \
    base_nodes/CCAtlasNode.cpp \
    base_nodes/CCNode.cpp \
    cocoa/CCAffineTransform.cpp \
    cocoa/CCGeometry.cpp \
    cocoa/CCAutoreleasePool.cpp \
    cocoa/CCDictionary.cpp \
    cocoa/CCString.cpp \
    cocoa/CCArray.cpp \
    cocoa/CCScriptEventDispatcher.cpp \
    cocoa/CCPointArray.cpp \
    cocoa/CCSet.cpp \
    engine/CCDirector.cpp \
    engine/CCEngineEvents.cpp \
    engine/CCModule.cpp \
    engine/CCObject.cpp \
    engine/CCResourceCache.cpp \
    engine/SubSystem.cpp \
    IO/FileSystem.cpp \
    resources/CCPlistResource.cpp \
    crypto/CCCrypto.cpp \
    crypto/libbase64.c \
    crypto/md5.cpp \
    crypto/xxtea.c \cocos
    CCConfiguration.cpp \
    CCScheduler.cpp \
    CCCamera.cpp \
    ccFPSImages.c \
    draw_nodes/CCDrawingPrimitives.cpp \
    draw_nodes/CCDrawNode.cpp \
    draw_nodes/CCShapeNode.cpp \
    effects/CCGrabber.cpp \
    effects/CCGrid.cpp \
    kazmath/src/aabb.c \
    kazmath/src/mat3.c \
    kazmath/src/mat4.c \
    kazmath/src/neon_matrix_impl.c \
    kazmath/src/plane.c \
    kazmath/src/quaternion.c \
    kazmath/src/ray2.c \
    kazmath/src/utility.c \
    kazmath/src/vec2.c \
    kazmath/src/vec3.c \
    kazmath/src/vec4.c \
    kazmath/src/GL/mat4stack.c \
    kazmath/src/GL/matrix.c \
    label_nodes/CCLabelAtlas.cpp \
    label_nodes/CCLabelBMFont.cpp \
    label_nodes/CCLabelTTF.cpp \
    layers_scenes_transitions_nodes/CCLayer.cpp \
    layers_scenes_transitions_nodes/CCScene.cpp \
    layers_scenes_transitions_nodes/CCTransitionPageTurn.cpp \
    layers_scenes_transitions_nodes/CCTransition.cpp \
    layers_scenes_transitions_nodes/CCTransitionProgress.cpp \
    misc_nodes/CCClippingNode.cpp \
    misc_nodes/CCClippingRegionNode.cpp \
    misc_nodes/CCProgressTimer.cpp \
    misc_nodes/CCRenderTexture.cpp \
    platform/CCSAXParser.cpp \
    platform/CCThread.cpp \
    platform/CCZipFile.cpp \
    platform/platform.cpp \
    platform/CCEGLViewProtocol.cpp \
    platform/CCDevice.cpp \
    platform/CCInput.cpp \
    platform/android/CCDeviceAndroid.cpp \
    platform/android/CCEGLView.cpp \
    platform/android/CCAccelerometer.cpp \
    platform/android/CCApplication.cpp \
    platform/android/CCImage.cpp \
    platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxBitmap.cpp \
    platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.cpp \
    platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxRenderer.cpp \
    platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxAccelerometer.cpp \
    platform/android/jni/JniHelper.cpp \
    platform/android/jni/IMEJni.cpp \
    platform/android/jni/TouchesJni.cpp \
    script_support/CCScriptSupport.cpp \
    shaders/CCMaterial.cpp \
    shaders/CCGLProgram.cpp \
    shaders/ccGLStateCache.cpp \
    shaders/CCShaderCache.cpp \
    sprite_nodes/CCAnimation.cpp \
    sprite_nodes/CCAnimationCache.cpp \
    sprite_nodes/CCSprite.cpp \
    sprite_nodes/CCSpriteBatchNode.cpp \
    sprite_nodes/CCSpriteFrame.cpp \
    sprite_nodes/CCScale9Sprite.cpp \
    support/ccUTF8.cpp \
    support/CCProfiling.cpp \
    support/CCPointExtension.cpp \
    support/TransformUtils.cpp \
    support/user_default/CCUserDefaultAndroid.cpp \
    support/CCVertex.cpp \
    support/data_support/ccCArray.cpp \
    support/tinyxml2/tinyxml2.cpp \
    support/zip_support/ZipUtils.cpp \
    support/zip_support/ioapi.cpp \
    support/zip_support/unzip.cpp \
    support/zip_support/ioapi_mem.cpp \
    support/component/CCComponent.cpp \
    support/component/CCComponentContainer.cpp \
    text_input_node/CCIMEDispatcher.cpp \
    text_input_node/CCTextFieldTTF.cpp \
    textures/CCTexture2D.cpp \
    textures/CCTextureAtlas.cpp \
    touch_dispatcher/CCTouchDispatcher.cpp \
    touch_dispatcher/CCTouchHandler.cpp \
    touch_dispatcher/CCTouch.cpp \
    touch_dispatcher/CCTouchTargetNode.cpp \
    support/image_support/TGAlib.cpp \
    textures/CCTextureETC.cpp \
    textures/CCTexturePVR.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)\
                    $(LOCAL_PATH)/platform/ \
                    $(LOCAL_PATH)/platform/android \
                    $(LOCAL_PATH)/kazmath/include \
					$(LOCAL_PATH)/../ \
					$(LOCAL_PATH)/../actions \
					$(LOCAL_PATH)/../scripting/ \
					$(LOCAL_PATH)/../scripting/lua \
					$(LOCAL_PATH)/../scripting/tolua

LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES)

LOCAL_EXPORT_LDLIBS := -lGLESv2 \
                       -llog

LOCAL_WHOLE_STATIC_LIBRARIES := cocos_libpng_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_libxml2_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_zlib_static

ifeq ($(CC_JPEG_ENABLED),1)
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_jpeg_static
endif

ifeq ($(CC_WEBP_ENABLED),1)
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_libwebp_static
endif

# define the macro to compile through support/zip_support/ioapi.c
LOCAL_CFLAGS := -DUSE_FILE32API $(ANDROID_COCOS2D_BUILD_FLAGS)
LOCAL_EXPORT_CFLAGS := -DUSE_FILE32API

include $(BUILD_STATIC_LIBRARY)

$(call import-module,libpng)
$(call import-module,zlib)

ifeq ($(CC_JPEG_ENABLED),1)
$(call import-module,libjpeg)
endif

ifeq ($(CC_WEBP_ENABLED),1)
$(call import-module,libwebp)
endif

ifeq ($(CC_CURL_ENABLED),1)
$(call import-module,libcurl)
endif
