
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := quickx_static
LOCAL_MODULE_FILENAME := quickx

LOCAL_SRC_FILES := ../cocos2d-x/actions/CCAction.cpp	\
../cocos2d-x/actions/CCActionCamera.cpp	\
../cocos2d-x/actions/CCActionCatmullRom.cpp	\
../cocos2d-x/actions/CCActionEase.cpp	\
../cocos2d-x/actions/CCActionFiniteTime.cpp	\
../cocos2d-x/actions/CCActionInstant.cpp	\
../cocos2d-x/actions/CCActionInterval.cpp	\
../cocos2d-x/actions/CCActionManager.cpp	\
../cocos2d-x/actions/CCActionProgressTimer.cpp	\
../cocos2d-x/actions/CCActionTween.cpp	\
../cocos2d-x/audio/android/SimpleAudioEngine.cpp	\
../cocos2d-x/audio/android/SimpleAudioEngineJni.cpp	\
../cocos2d-x/core/CCCamera.cpp	\
../cocos2d-x/core/CCConfiguration.cpp	\
../cocos2d-x/core/CCScheduler.cpp	\
../cocos2d-x/core/base/ID.cpp	\
../cocos2d-x/core/base/MathDefs.cpp	\
../cocos2d-x/core/base/ProcessUtils.cpp	\
../cocos2d-x/core/base/RefCounted.cpp	\
../cocos2d-x/core/base/Variant.cpp	\
../cocos2d-x/core/cocoa/CCAffineTransform.cpp	\
../cocos2d-x/core/cocoa/CCArray.cpp	\
../cocos2d-x/core/cocoa/CCAutoreleasePool.cpp	\
../cocos2d-x/core/cocoa/CCDictionary.cpp	\
../cocos2d-x/core/cocoa/CCGeometry.cpp	\
../cocos2d-x/core/cocoa/CCScriptEventDispatcher.cpp	\
../cocos2d-x/core/cocoa/CCScriptSupport.cpp	\
../cocos2d-x/core/cocoa/CCSet.cpp	\
../cocos2d-x/core/cocoa/CCString.cpp	\
../cocos2d-x/core/engine/CCAttributeAccessor.cpp	\
../cocos2d-x/core/engine/CCDebugHud.cpp	\
../cocos2d-x/core/engine/CCDirector.cpp	\
../cocos2d-x/core/engine/CCEngineEvents.cpp	\
../cocos2d-x/core/engine/CCFileSystem.cpp	\
../cocos2d-x/core/engine/CCObject.cpp	\
../cocos2d-x/core/engine/CCProfiling.cpp	\
../cocos2d-x/core/engine/CCResourceCache.cpp	\
../cocos2d-x/core/engine/CCSubSystem.cpp	\
../cocos2d-x/core/graphics/CCColor.cpp	\
../cocos2d-x/core/graphics/CCRendererBatcher.cpp	\
../cocos2d-x/core/graphics/CCVec2.cpp	\
../cocos2d-x/core/nodes/CCAtlasNode.cpp	\
../cocos2d-x/core/nodes/CCClippingNode.cpp	\
../cocos2d-x/core/nodes/CCClippingRegionNode.cpp	\
../cocos2d-x/core/nodes/CCDrawNode.cpp	\
../cocos2d-x/core/nodes/CCDrawingPrimitives.cpp	\
../cocos2d-x/core/nodes/CCLabelAtlas.cpp	\
../cocos2d-x/core/nodes/CCLabelBMFont.cpp	\
../cocos2d-x/core/nodes/CCLabelTTF.cpp	\
../cocos2d-x/core/nodes/CCLayer.cpp	\
../cocos2d-x/core/nodes/CCNode.cpp	\
../cocos2d-x/core/nodes/CCProgressTimer.cpp	\
../cocos2d-x/core/nodes/CCRenderTexture.cpp	\
../cocos2d-x/core/nodes/CCScale9Sprite.cpp	\
../cocos2d-x/core/nodes/CCScene.cpp	\
../cocos2d-x/core/nodes/CCShapeNode.cpp	\
../cocos2d-x/core/nodes/CCSprite.cpp	\
../cocos2d-x/core/nodes/CCSpriteBatchNode.cpp	\
../cocos2d-x/core/nodes/CCTextFieldTTF.cpp	\
../cocos2d-x/core/platform/CCDevice.cpp	\
../cocos2d-x/core/platform/CCEGLView.cpp	\
../cocos2d-x/core/platform/CCInput.cpp	\
../cocos2d-x/core/platform/CCSAXParser.cpp	\
../cocos2d-x/core/platform/CCTimer.cpp	\
../cocos2d-x/core/platform/android/CCDeviceAndroid.cpp	\
../cocos2d-x/core/platform/android/CCImage.cpp	\
../cocos2d-x/core/platform/android/jni/IMEJni.cpp	\
../cocos2d-x/core/platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxBitmap.cpp	\
../cocos2d-x/core/platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.cpp	\
../cocos2d-x/core/platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxRenderer.cpp	\
../cocos2d-x/core/platform/android/jni/JniHelper.cpp	\
../cocos2d-x/core/platform/android/jni/TouchesJni.cpp	\
../cocos2d-x/core/resources/CCAnimation.cpp	\
../cocos2d-x/core/resources/CCPlistResource.cpp	\
../cocos2d-x/core/resources/CCSpriteFrame.cpp	\
../cocos2d-x/core/shaders/CCGLProgram.cpp	\
../cocos2d-x/core/shaders/CCRenderState.cpp	\
../cocos2d-x/core/shaders/CCShaderCache.cpp	\
../cocos2d-x/core/shaders/ccGLStateCache.cpp	\
../cocos2d-x/core/support/TransformUtils.cpp	\
../cocos2d-x/core/support/ccUTF8.cpp	\
../cocos2d-x/core/support/component/CCComponent.cpp	\
../cocos2d-x/core/support/component/CCComponentContainer.cpp	\
../cocos2d-x/core/support/data_support/ccCArray.cpp	\
../cocos2d-x/core/support/image_support/TGAlib.cpp	\
../cocos2d-x/core/support/tinyxml2/tinyxml2.cpp	\
../cocos2d-x/core/support/user_default/CCUserDefault.cpp	\
../cocos2d-x/core/support/user_default/CCUserDefaultAndroid.cpp	\
../cocos2d-x/core/text_input_node/CCIMEDispatcher.cpp	\
../cocos2d-x/core/textures/CCTexture2D.cpp	\
../cocos2d-x/core/textures/CCTextureAtlas.cpp	\
../cocos2d-x/core/textures/CCTextureETC.cpp	\
../cocos2d-x/core/textures/CCTexturePVR.cpp	\
../cocos2d-x/core/touch_dispatcher/CCTouch.cpp	\
../cocos2d-x/crypto/CCCrypto.cpp	\
../cocos2d-x/crypto/md5.cpp	\
../cocos2d-x/network/CCHTTPRequest.cpp	\
../cocos2d-x/network/CCNetwork.cpp	\
../cocos2d-x/network/CCNetworkAndroid.cpp	\
../cocos2d-x/network/cocos2dx_httprequest_luabinding.cpp	\
../cocos2d-x/particle/CCParticleBatchNode.cpp	\
../cocos2d-x/particle/CCParticleExamples.cpp	\
../cocos2d-x/particle/CCParticleSystem.cpp	\
../cocos2d-x/particle/CCParticleSystemQuad.cpp	\
../cocos2d-x/scripting/CCLuaBridge.cpp	\
../cocos2d-x/scripting/CCLuaBridge_java.cpp	\
../cocos2d-x/scripting/CCLuaEngine.cpp	\
../cocos2d-x/scripting/CCLuaStack.cpp	\
../cocos2d-x/scripting/CCLuaValue.cpp	\
../cocos2d-x/scripting/LuaCocos2d.cpp	\
../cocos2d-x/scripting/LuaFunction.cpp	\
../cocos2d-x/scripting/LuaTable.cpp	\
../cocos2d-x/scripting/lua_cocos2dx_cocostudio_manual.cpp	\
../cocos2d-x/scripting/platform/android/org_cocos2dx_lib_Cocos2dxLuaJavaBridge.cpp	\
../cocos2d-x/spine/PolygonBatch.cpp	\
../cocos2d-x/spine/SkeletonAnimation.cpp	\
../cocos2d-x/spine/SpineResource.cpp	\
../cocos2d-x/spine/spine-cocos2dx.cpp	\
../cocos2d-x/spine/spine_luabinding.cpp	\
../cocos2d-x/tilemap/CCTMXLayer.cpp	\
../cocos2d-x/tilemap/CCTMXObjectGroup.cpp	\
../cocos2d-x/tilemap/CCTMXTiledMap.cpp	\
../cocos2d-x/tilemap/CCTMXXMLParser.cpp	\
../cocos2d-x/tilemap/CCTileMapAtlas.cpp	\
../cocos2d-x/ui/EditBox/CCEditBoxImplAndroid.cpp	\
../cocos2d-x/ui/EditBox/UIEditBox.cpp	\
../cocos2d-x/ui/Layouts/UILayout.cpp	\
../cocos2d-x/ui/Layouts/UILayoutDefine.cpp	\
../cocos2d-x/ui/Layouts/UILayoutParameter.cpp	\
../cocos2d-x/ui/ScrollWidget/UIListView.cpp	\
../cocos2d-x/ui/ScrollWidget/UIPageView.cpp	\
../cocos2d-x/ui/ScrollWidget/UIScrollView.cpp	\
../cocos2d-x/ui/UIButton.cpp	\
../cocos2d-x/ui/UICheckBox.cpp	\
../cocos2d-x/ui/UIImageView.cpp	\
../cocos2d-x/ui/UILabel.cpp	\
../cocos2d-x/ui/UILabelAtlas.cpp	\
../cocos2d-x/ui/UILabelBMFont.cpp	\
../cocos2d-x/ui/UILoadingBar.cpp	\
../cocos2d-x/ui/UIRichText.cpp	\
../cocos2d-x/ui/UISlider.cpp	\
../cocos2d-x/ui/UITextField.cpp	\
../cocos2d-x/ui/UITouchGroup.cpp	\
../cocos2d-x/ui/UIWidget.cpp	\
../cocos2d-x/ui/webview/UIWebView.cpp	\
../cocos2d-x/ui/webview/UIWebViewImpl-android.cpp	\
../cocos2d-x/zip/CCZipFile.cpp	\
../cocos2d-x/zip/ZipUtils.cpp	\
../cocos2d-x/zip/ioapi.cpp	\
../cocos2d-x/zip/ioapi_mem.cpp	\
../cocos2d-x/zip/unzip.cpp \
../cocos2d-x/scripting/tolua/tolua_event.c \
../cocos2d-x/scripting/tolua/tolua_is.c \
../cocos2d-x/scripting/tolua/tolua_map.c \
../cocos2d-x/scripting/tolua/tolua_push.c \
../cocos2d-x/scripting/tolua/tolua_to.c \
../cocos2d-x/scripting/lua_extensions/cjson/fpconv.c \
../cocos2d-x/scripting/lua_extensions/cjson/lua_cjson.c \
../cocos2d-x/scripting/lua_extensions/cjson/strbuf.c \
../cocos2d-x/scripting/lua_extensions/zlib/lua_zlib.c \
../cocos2d-x/scripting/lua_extensions/socket/auxiliar.c \
../cocos2d-x/scripting/lua_extensions/socket/luasocket_buffer.c \
../cocos2d-x/scripting/lua_extensions/socket/except.c \
../cocos2d-x/scripting/lua_extensions/socket/inet.c \
../cocos2d-x/scripting/lua_extensions/socket/luasocket_io.c \
../cocos2d-x/scripting/lua_extensions/socket/luasocket.c \
../cocos2d-x/scripting/lua_extensions/socket/mime.c \
../cocos2d-x/scripting/lua_extensions/socket/options.c \
../cocos2d-x/scripting/lua_extensions/socket/select.c \
../cocos2d-x/scripting/lua_extensions/socket/serial.c \
../cocos2d-x/scripting/lua_extensions/socket/socket_scripts.c \
../cocos2d-x/scripting/lua_extensions/socket/tcp.c \
../cocos2d-x/scripting/lua_extensions/socket/timeout.c \
../cocos2d-x/scripting/lua_extensions/socket/udp.c \
../cocos2d-x/scripting/lua_extensions/socket/unix.c \
../cocos2d-x/scripting/lua_extensions/socket/usocket.c \
../cocos2d-x/scripting/lua_extensions/filesystem/lfs.c \
../cocos2d-x/scripting/lua_extensions/lpack/lpack.c \
../cocos2d-x/scripting/lua_extensions/snapshot/snapshot.c \
../cocos2d-x/scripting/lua_extensions/lsqlite3/sqlite3.c \
../cocos2d-x/scripting/lua_extensions/lsqlite3/lsqlite3.c \
../cocos2d-x/scripting/lua_extensions/lua_extensions.c \
../cocos2d-x/scripting/lua/lapi.c \
../cocos2d-x/scripting/lua/lauxlib.c \
../cocos2d-x/scripting/lua/lbaselib.c \
../cocos2d-x/scripting/lua/lcode.c \
../cocos2d-x/scripting/lua/ldblib.c \
../cocos2d-x/scripting/lua/ldebug.c \
../cocos2d-x/scripting/lua/ldo.c \
../cocos2d-x/scripting/lua/ldump.c \
../cocos2d-x/scripting/lua/lfunc.c \
../cocos2d-x/scripting/lua/lgc.c \
../cocos2d-x/scripting/lua/linit.c \
../cocos2d-x/scripting/lua/liolib.c \
../cocos2d-x/scripting/lua/llex.c \
../cocos2d-x/scripting/lua/lmathlib.c \
../cocos2d-x/scripting/lua/lmem.c \
../cocos2d-x/scripting/lua/loadlib.c \
../cocos2d-x/scripting/lua/lobject.c \
../cocos2d-x/scripting/lua/lopcodes.c \
../cocos2d-x/scripting/lua/loslib.c \
../cocos2d-x/scripting/lua/lparser.c \
../cocos2d-x/scripting/lua/lstate.c \
../cocos2d-x/scripting/lua/lstring.c \
../cocos2d-x/scripting/lua/lstrlib.c \
../cocos2d-x/scripting/lua/ltable.c \
../cocos2d-x/scripting/lua/ltablib.c \
../cocos2d-x/scripting/lua/ltm.c \
../cocos2d-x/scripting/lua/lundump.c \
../cocos2d-x/scripting/lua/lvm.c \
../cocos2d-x/scripting/lua/lzio.c \
../cocos2d-x/scripting/lua/print.c \
../cocos2d-x/spine/spine/AnimationState.c   \
../cocos2d-x/spine/spine/AnimationStateData.c   \
../cocos2d-x/spine/spine/Atlas.c   \
../cocos2d-x/spine/spine/AtlasAttachmentLoader.c   \
../cocos2d-x/spine/spine/Attachment.c   \
../cocos2d-x/spine/spine/AttachmentLoader.c   \
../cocos2d-x/spine/spine/Bone.c   \
../cocos2d-x/spine/spine/BoneData.c   \
../cocos2d-x/spine/spine/BoundingBoxAttachment.c   \
../cocos2d-x/spine/spine/Event.c   \
../cocos2d-x/spine/spine/EventData.c   \
../cocos2d-x/spine/spine/extension.c   \
../cocos2d-x/spine/spine/IkConstraint.c   \
../cocos2d-x/spine/spine/IkConstraintData.c   \
../cocos2d-x/spine/spine/Json.c   \
../cocos2d-x/spine/spine/MeshAttachment.c   \
../cocos2d-x/spine/spine/RegionAttachment.c   \
../cocos2d-x/spine/spine/Skeleton.c   \
../cocos2d-x/spine/spine/SkeletonBounds.c   \
../cocos2d-x/spine/spine/SkeletonData.c   \
../cocos2d-x/spine/spine/SkeletonJson.c   \
../cocos2d-x/spine/spine/Skin.c   \
../cocos2d-x/spine/spine/SkinnedMeshAttachment.c   \
../cocos2d-x/spine/spine/Slot.c   \
../cocos2d-x/spine/spine/SlotData.c   \
../cocos2d-x/spine/spine/Animation.c   \
../cocos2d-x/crypto/libbase64.c   \
../cocos2d-x/crypto/xxtea.c   \
../cocos2d-x/core/kazmath/src/aabb.c \
../cocos2d-x/core/kazmath/src/mat3.c \
../cocos2d-x/core/kazmath/src/mat4.c \
../cocos2d-x/core/kazmath/src/neon_matrix_impl.c \
../cocos2d-x/core/kazmath/src/plane.c \
../cocos2d-x/core/kazmath/src/quaternion.c \
../cocos2d-x/core/kazmath/src/ray2.c \
../cocos2d-x/core/kazmath/src/utility.c \
../cocos2d-x/core/kazmath/src/vec2.c \
../cocos2d-x/core/kazmath/src/vec3.c \
../cocos2d-x/core/kazmath/src/vec4.c \
../cocos2d-x/core/kazmath/src/GL/mat4stack.c \
../cocos2d-x/core/kazmath/src/GL/matrix.c \
../cocos2d-x/scripting/tolua_fix.c \
../cocos2d-x/core/ccFPSImages.c 


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../cocos2d-x \
	$(LOCAL_PATH)/../cocos2d-x/core/platform/ \
	$(LOCAL_PATH)/../cocos2d-x/core/ \
	$(LOCAL_PATH)/../cocos2d-x/core/kazmath/include \
	$(LOCAL_PATH)/../cocos2d-x/actions \
	$(LOCAL_PATH)/../cocos2d-x/scripting \
	$(LOCAL_PATH)/../cocos2d-x/scripting/lua \
	$(LOCAL_PATH)/../cocos2d-x/spine/ \
	$(LOCAL_PATH)/../cocos2d-x/scripting/tolua 

LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES)

LOCAL_WHOLE_STATIC_LIBRARIES := cocos_libpng_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_libxml2_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_zlib_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_curl_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_jpeg_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_libwebp_static
LOCAL_WHOLE_STATIC_LIBRARIES += SDL2_static

LOCAL_EXPORT_LDLIBS := -lGLESv2 \
                       -llog \
                       -landroid

include $(BUILD_STATIC_LIBRARY)
# define the macro to compile through support/zip_support/ioapi.c
LOCAL_CFLAGS := -DUSE_FILE32API -DCC_LUA_ENGINE_ENABLED=1 $(ANDROID_COCOS2D_BUILD_FLAGS)
LOCAL_EXPORT_CFLAGS := -DUSE_FILE32API -DCC_LUA_ENGINE_ENABLED=1



$(call import-module,SDL2)
$(call import-module,libpng)
$(call import-module,zlib)
$(call import-module,libjpeg)
$(call import-module,libwebp)
$(call import-module,libcurl)

