LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := cc_particle_static

LOCAL_MODULE_FILENAME := libparticle

LOCAL_SRC_FILES := \
		CCParticleBatchNode.cpp   \
		CCParticleExamples.cpp   \
		CCParticleSystem.cpp   \
		CCParticleSystemQuad.cpp   \

LOCAL_WHOLE_STATIC_LIBRARIES := cc_core_static
LOCAL_CFLAGS := -fexceptions

include $(BUILD_STATIC_LIBRARY)

$(call import-module, core)
