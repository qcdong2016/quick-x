LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := cc_spine_static
LOCAL_MODULE_FILENAME := libspine

LOCAL_SRC_FILES := spine/Animation.c \
					spine/AnimationState.c   \
					spine/AnimationStateData.c   \
					spine/Atlas.c   \
					spine/AtlasAttachmentLoader.c   \
					spine/Attachment.c   \
					spine/AttachmentLoader.c   \
					spine/Bone.c   \
					spine/BoneData.c   \
					spine/BoundingBoxAttachment.c   \
					spine/Event.c   \
					spine/EventData.c   \
					spine/extension.c   \
					spine/IkConstraint.c   \
					spine/IkConstraintData.c   \
					spine/Json.c   \
					spine/MeshAttachment.c   \
					spine/RegionAttachment.c   \
					spine/Skeleton.c   \
					spine/SkeletonBounds.c   \
					spine/SkeletonData.c   \
					spine/SkeletonJson.c   \
					spine/Skin.c   \
					spine/SkinnedMeshAttachment.c   \
					spine/Slot.c   \
					spine/SlotData.c   \
					PolygonBatch.cpp   \
					SkeletonAnimation.cpp   \
					spine_luabinding.cpp   \
					spine-cocos2dx.cpp


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
					$(LOCAL_PATH)/spine
LOCAL_C_INCLUDES := $(LOCAL_PATH) \
					$(LOCAL_PATH)/spine \

LOCAL_WHOLE_STATIC_LIBRARIES := cc_core_static

include $(BUILD_STATIC_LIBRARY)
