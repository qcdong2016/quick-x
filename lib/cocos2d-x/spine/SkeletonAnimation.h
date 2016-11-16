/******************************************************************************
 * Spine Runtimes Software License
 * Version 2.3
 * 
 * Copyright (c) 2013-2015, Esoteric Software
 * All rights reserved.
 * 
 * You are granted a perpetual, non-exclusive, non-sublicensable and
 * non-transferable license to use, install, execute and perform the Spine
 * Runtimes Software (the "Software") and derivative works solely for personal
 * or internal use. Without the written permission of Esoteric Software (see
 * Section 2 of the Spine Software License Agreement), you may not (a) modify,
 * translate, adapt or otherwise create derivative works, improvements of the
 * Software or develop new applications using the Software or (b) remove,
 * delete, alter or obscure any trademarks or any copyright, trademark, patent
 * or other intellectual property or proprietary rights notices on or in the
 * Software, including any copy thereof. Redistributions in binary or source
 * form must include this license and terms.
 * 
 * THIS SOFTWARE IS PROVIDED BY ESOTERIC SOFTWARE "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL ESOTERIC SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#ifndef SPINE_SKELETONANIMATION_H_
#define SPINE_SKELETONANIMATION_H_

#include <spine/spine.h>
#include "PolygonBatch.h"
#include "base/Ptr.h"
#include "base_nodes/CCNode.h"
#include "SpineResource.h"

namespace cocos2d {


/** Draws an animated skeleton, providing an AnimationState for applying one or more animations and queuing animations to be
  * played later. */
class CC_DLL SkeletonAnimation : public CCNode
{
public:
	spAnimationState* state;
	spSkeleton* skeleton;
	spBone* rootBone;
	float timeScale;
	bool debugSlots;
	bool debugBones;
	bool premultipliedAlpha;

	SkeletonAnimation();
	static SkeletonAnimation* create(const char* skeletonDataFile, const char* atlasFile, float scale = 0);
	static SkeletonAnimation* create(SpineAtlasResource* atlas, SpineSkeletonDataResource* skeletonData);
	virtual ~SkeletonAnimation(void);

	virtual void update(float deltaTime);
	virtual void draw();
	virtual CCRect boundingBox();

	// --- Convenience methods for common Skeleton_* functions.
	void updateWorldTransform();

	void setToSetupPose();
	void setBonesToSetupPose();
	void setSlotsToSetupPose();

	/* Returns 0 if the bone was not found. */
	spBone* findBone(const char* boneName) const;
	/* Returns 0 if the slot was not found. */
	spSlot* findSlot(const char* slotName) const;

	/* Sets the skin used to look up attachments not found in the SkeletonData defaultSkin. Attachments from the new skin are
	* attached if the corresponding attachment from the old skin was attached. If there was no old skin, each slot's setup mode
	* attachment is attached from the new skin. Returns false if the skin was not found.
	* @param skin May be 0.*/
	bool setSkin(const char* skinName);

	/* Returns 0 if the slot or attachment was not found. */
	spAttachment* getAttachment(const char* slotName, const char* attachmentName) const;
	/* Returns false if the slot or attachment was not found. */
	bool setAttachment(const char* slotName, const char* attachmentName);

	// --- BlendProtocol
	CC_PROPERTY(cocos2d::ccBlendFunc, blendFunc, BlendFunc);
	virtual void setOpacityModifyRGB(bool value);
	virtual bool isOpacityModifyRGB();

	void setSkeletonData(spSkeletonData* skeletonData);

	virtual cocos2d::CCTexture2D* getTexture(spRegionAttachment* attachment) const;
	virtual cocos2d::CCTexture2D* getTexture(spMeshAttachment* attachment) const;
	virtual cocos2d::CCTexture2D* getTexture(spSkinnedMeshAttachment* attachment) const;

	void setAnimationStateData (spAnimationStateData* stateData);
	void setMix (const char* fromAnimation, const char* toAnimation, float duration);

	spTrackEntry* setAnimation (int trackIndex, const char* name, bool loop);
	spTrackEntry* addAnimation (int trackIndex, const char* name, bool loop, float delay = 0);
	spTrackEntry* getCurrent (int trackIndex = 0);
	void clearTracks ();
	void clearTrack (int trackIndex = 0);

	virtual void onAnimationStateEvent (int trackIndex, spEventType type, spEvent* event, int loopCount);
	virtual void onTrackEntryEvent (int trackIndex, spEventType type, spEvent* event, int loopCount);

private:
	SharedPtr<SpineAtlasResource> _atlas;
	SharedPtr<SpineSkeletonDataResource> _skeletonData;

	PolygonBatch* _batch;
	float* _worldVertices;

	bool _ownsAnimationStateData;

	void initialize ();
};

}

#endif /* SPINE_SKELETONANIMATION_H_ */
