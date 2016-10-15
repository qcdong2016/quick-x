LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := cocos_particle_static

LOCAL_MODULE_FILENAME := libparticle

LOCAL_SRC_FILES := \
		CCParticleBatchNode.cpp   \
		CCParticleExamples.cpp   \
		CCParticleSystem.cpp   \
		CCParticleSystemQuad.cpp   \

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
