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

#include "SkeletonAnimation.h"
#include "spine-cocos2dx.h"
#include <spine/extension.h>
#include <algorithm>
#include "PolygonBatch.h"
#include "SpineEvents.h"
#include "base/MathDefs.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CCShaderCache.h"
#include "draw_nodes/CCDrawingPrimitives.h"
#include "support/CCPointExtension.h"

namespace cocos2d {

void animationCallback (spAnimationState* state, int trackIndex, spEventType type, spEvent* event, int loopCount) {
	((SkeletonAnimation*)state->rendererObject)->onAnimationStateEvent(trackIndex, type, event, loopCount);
}

void trackEntryCallback (spAnimationState* state, int trackIndex, spEventType type, spEvent* event, int loopCount) {
	((SkeletonAnimation*)state->rendererObject)->onTrackEntryEvent(trackIndex, type, event, loopCount);
}

class _TrackEntryListeners {
public:

};

static _TrackEntryListeners* getListeners (spTrackEntry* entry) {
	if (!entry->rendererObject) {
		entry->rendererObject = new (_TrackEntryListeners);
		entry->listener = trackEntryCallback;
	}
	return (_TrackEntryListeners*)entry->rendererObject;
}

void disposeTrackEntry (spTrackEntry* entry) {
	if (entry->rendererObject) 
		delete ((_TrackEntryListeners*)(entry->rendererObject));
	_spTrackEntry_dispose(entry);
}


//

SkeletonAnimation* SkeletonAnimation::create(const char* skeletonDataFile, const char* atlasFile, float scale) 
{
	ResourceCache* rc = SubSystem::get<ResourceCache>();

	SpineAtlasResource* atlas = rc->getResource<SpineAtlasResource>(atlasFile);
	SpineSkeletonDataResource* skeletonData = rc->getResource<SpineSkeletonDataResource>(skeletonDataFile, atlas);

	return create(atlas, skeletonData);
}

SkeletonAnimation* SkeletonAnimation::create(SpineAtlasResource* atlas, SpineSkeletonDataResource* skeletonData)
{
	SkeletonAnimation* ani = new SkeletonAnimation();
	ani->_atlas = atlas;
	ani->_skeletonData = skeletonData;

	ani->setSkeletonData(skeletonData->_skeletonData);
	ani->autorelease();
	ani->initialize();

	return ani;
}
SkeletonAnimation::SkeletonAnimation() 
	: state(nullptr)
	, skeleton(nullptr)
{
}
SkeletonAnimation::~SkeletonAnimation(void) {
	if (_ownsAnimationStateData) 
		spAnimationStateData_dispose(state->data);
	spAnimationState_dispose(state);
	spSkeleton_dispose(skeleton);
	FREE(_worldVertices);
	_batch->release();
}

void SkeletonAnimation::initialize () {
	_atlas = 0;
	debugSlots = false;
	debugBones = false;
	timeScale = 1;

	_worldVertices = MALLOC(float, 1000); // Max number of vertices per mesh.

	_batch = PolygonBatch::createWithCapacity(2000); // Max number of vertices and triangles per batch.
	_batch->retain();

	blendFunc.src = GL_ONE;
	blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
	setOpacityModifyRGB(true);

	setMaterial(CCShaderCache::sharedShaderCache()->getMaterial(kCCShader_PositionTextureColor));
	scheduleUpdate();

	_ownsAnimationStateData = true;
	state = spAnimationState_create(spAnimationStateData_create(skeleton->data));
	state->rendererObject = this;
	state->listener = animationCallback;

	_spAnimationState* stateInternal = (_spAnimationState*)state;
	stateInternal->disposeTrackEntry = disposeTrackEntry;
}


void SkeletonAnimation::update (float deltaTime) {
	spSkeleton_update(skeleton, deltaTime * timeScale);

	deltaTime *= timeScale;
	spAnimationState_update(state, deltaTime);
	spAnimationState_apply(state, skeleton);
	spSkeleton_updateWorldTransform(skeleton);
}

void SkeletonAnimation::setAnimationStateData (spAnimationStateData* stateData) {
	CCAssert(stateData, "stateData cannot be null.");

	if (_ownsAnimationStateData) spAnimationStateData_dispose(state->data);
	spAnimationState_dispose(state);

	_ownsAnimationStateData = false;
	state = spAnimationState_create(stateData);
	state->rendererObject = this;
	state->listener = animationCallback;
}

void SkeletonAnimation::setMix (const char* fromAnimation, const char* toAnimation, float duration) {
	spAnimationStateData_setMixByName(state->data, fromAnimation, toAnimation, duration);
}

spTrackEntry* SkeletonAnimation::setAnimation (int trackIndex, const char* name, bool loop) {
	spAnimation* animation = spSkeletonData_findAnimation(skeleton->data, name);
	if (!animation) {
		CCLOG("Spine: Animation not found: %s", name);
		return 0;
	}
	return spAnimationState_setAnimation(state, trackIndex, animation, loop);
}

spTrackEntry* SkeletonAnimation::addAnimation (int trackIndex, const char* name, bool loop, float delay) {
	spAnimation* animation = spSkeletonData_findAnimation(skeleton->data, name);
	if (!animation) {
		CCLOG("Spine: Animation not found: %s", name);
		return 0;
	}
	return spAnimationState_addAnimation(state, trackIndex, animation, loop, delay);
}

spTrackEntry* SkeletonAnimation::getCurrent (int trackIndex) { 
	return spAnimationState_getCurrent(state, trackIndex);
}

void SkeletonAnimation::clearTracks () {
	spAnimationState_clearTracks(state);
}

void SkeletonAnimation::clearTrack (int trackIndex) {
	spAnimationState_clearTrack(state, trackIndex);
}

template<typename T>
static void send(CCObject* sender, int trackIndex, int loopCount)
{
	EventDataMap map;

	map[T::trackIndex] = trackIndex;
	map[T::loopCount] = loopCount;

	sender->sendEvent<T>(map);
}
static void call(CCObject* sender, int trackIndex, int loopCount, spEvent* event = nullptr)
{
	EventDataMap map;

	map[SpineEvent::Param::trackIndex] = trackIndex;
	map[SpineEvent::Param::loopCount] = loopCount;

	map[SpineEvent::Param::eventName] = event->data->name;
	map[SpineEvent::Param::floatValue] = event->data->floatValue;
	map[SpineEvent::Param::intValue] = event->data->intValue;
	if (event->data->stringValue)
		map[SpineEvent::Param::stringValue] = event->data->stringValue;
	sender->sendEvent<SpineEvent::Param>(map);
}

void SkeletonAnimation::onAnimationStateEvent (int trackIndex, spEventType type, spEvent* event, int loopCount) {

	switch (type) {
	case SP_ANIMATION_START: send<SpineStart::Param>(this, trackIndex, loopCount); break;
	case SP_ANIMATION_END:   send<SpineEnd::Param>(this, trackIndex, loopCount); break;
	case SP_ANIMATION_COMPLETE:send<SpineComplete::Param>(this, trackIndex, loopCount); break;
	case SP_ANIMATION_EVENT:  call(this, trackIndex, loopCount, event); break;
	}
}

void SkeletonAnimation::onTrackEntryEvent (int trackIndex, spEventType type, spEvent* event, int loopCount) {

}

static const int quadTriangles[6] = { 0, 1, 2, 2, 3, 0 };

void SkeletonAnimation::setSkeletonData(spSkeletonData *skeletonData) {
	skeleton = spSkeleton_create(skeletonData);
	rootBone = skeleton->bones[0];
}

void SkeletonAnimation::draw() {
	CC_NODE_DRAW_SETUP();
	ccGLBindVAO(0);

	ccColor3B nodeColor = getColor();
	skeleton->r = nodeColor.r / (float)255;
	skeleton->g = nodeColor.g / (float)255;
	skeleton->b = nodeColor.b / (float)255;
	skeleton->a = getDisplayedOpacity() / (float)255;

	int blendMode = -1;
	ccColor4B color;
	const float* uvs = nullptr;
	int verticesCount = 0;
	const int* triangles = nullptr;
	int trianglesCount = 0;
	float r = 0, g = 0, b = 0, a = 0;
	for (int i = 0, n = skeleton->slotsCount; i < n; i++) {
		spSlot* slot = skeleton->drawOrder[i];
		if (!slot->attachment) continue;
		CCTexture2D *texture = nullptr;
		switch (slot->attachment->type) {
		case SP_ATTACHMENT_REGION: {
			spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
			spRegionAttachment_computeWorldVertices(attachment, slot->bone, _worldVertices);
			texture = getTexture(attachment);
			uvs = attachment->uvs;
			verticesCount = 8;
			triangles = quadTriangles;
			trianglesCount = 6;
			r = attachment->r;
			g = attachment->g;
			b = attachment->b;
			a = attachment->a;
			break;
		}
		case SP_ATTACHMENT_MESH: {
			spMeshAttachment* attachment = (spMeshAttachment*)slot->attachment;
			spMeshAttachment_computeWorldVertices(attachment, slot, _worldVertices);
			texture = getTexture(attachment);
			uvs = attachment->uvs;
			verticesCount = attachment->verticesCount;
			triangles = attachment->triangles;
			trianglesCount = attachment->trianglesCount;
			r = attachment->r;
			g = attachment->g;
			b = attachment->b;
			a = attachment->a;
			break;
		}
		case SP_ATTACHMENT_SKINNED_MESH: {
			spSkinnedMeshAttachment* attachment = (spSkinnedMeshAttachment*)slot->attachment;
			spSkinnedMeshAttachment_computeWorldVertices(attachment, slot, _worldVertices);
			texture = getTexture(attachment);
			uvs = attachment->uvs;
			verticesCount = attachment->uvsCount;
			triangles = attachment->triangles;
			trianglesCount = attachment->trianglesCount;
			r = attachment->r;
			g = attachment->g;
			b = attachment->b;
			a = attachment->a;
			break;
		}
		}
		if (texture) {
			if (slot->data->blendMode != blendMode) {
				_batch->flush();
				blendMode = slot->data->blendMode;
				switch (slot->data->blendMode) {
				case SP_BLEND_MODE_ADDITIVE:
					ccGLBlendFunc(premultipliedAlpha ? GL_ONE : GL_SRC_ALPHA, GL_ONE);
					break;
				case SP_BLEND_MODE_MULTIPLY:
					ccGLBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
					break;
				case SP_BLEND_MODE_SCREEN:
					ccGLBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
					break;
				default:
					ccGLBlendFunc(blendFunc.src, blendFunc.dst);
				}
			}
			color.a = skeleton->a * slot->a * a * 255;
			float multiplier = premultipliedAlpha ? color.a : 255;
			color.r = skeleton->r * slot->r * r * multiplier;
			color.g = skeleton->g * slot->g * g * multiplier;
			color.b = skeleton->b * slot->b * b * multiplier;
			_batch->add(texture, _worldVertices, uvs, verticesCount, triangles, trianglesCount, &color);
		}
	}
	_batch->flush();

	if (debugSlots) {
		// Slots.
		ccDrawColor4B(0, 0, 255, 255);
		glLineWidth(1);
		CCPoint points[4];
		for (int i = 0, n = skeleton->slotsCount; i < n; i++) {
			spSlot* slot = skeleton->drawOrder[i];
			if (!slot->attachment || slot->attachment->type != SP_ATTACHMENT_REGION) continue;
			spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
			spRegionAttachment_computeWorldVertices(attachment, slot->bone, _worldVertices);
			points[0] = ccp(_worldVertices[0], _worldVertices[1]);
			points[1] = ccp(_worldVertices[2], _worldVertices[3]);
			points[2] = ccp(_worldVertices[4], _worldVertices[5]);
			points[3] = ccp(_worldVertices[6], _worldVertices[7]);
			ccDrawPoly(points, 4, true);
		}
	}
	if (debugBones) {
		// Bone lengths.
		glLineWidth(2);
		ccDrawColor4B(255, 0, 0, 255);
		for (int i = 0, n = skeleton->bonesCount; i < n; i++) {
			spBone *bone = skeleton->bones[i];
			float x = bone->data->length * bone->m00 + bone->worldX;
			float y = bone->data->length * bone->m10 + bone->worldY;
			ccDrawLine(ccp(bone->worldX, bone->worldY), ccp(x, y));
		}
		// Bone origins.
		ccPointSize(4);
		ccDrawColor4B(0, 0, 255, 255); // Root bone is blue.
		for (int i = 0, n = skeleton->bonesCount; i < n; i++) {
			spBone *bone = skeleton->bones[i];
			ccDrawPoint(ccp(bone->worldX, bone->worldY));
			if (i == 0) ccDrawColor4B(0, 255, 0, 255);
		}
	}
}

CCTexture2D* SkeletonAnimation::getTexture(spRegionAttachment* attachment) const {
	return (CCTexture2D*)((spAtlasRegion*)attachment->rendererObject)->page->rendererObject;
}

CCTexture2D* SkeletonAnimation::getTexture(spMeshAttachment* attachment) const {
	return (CCTexture2D*)((spAtlasRegion*)attachment->rendererObject)->page->rendererObject;
}

CCTexture2D* SkeletonAnimation::getTexture(spSkinnedMeshAttachment* attachment) const {
	return (CCTexture2D*)((spAtlasRegion*)attachment->rendererObject)->page->rendererObject;
}

CCRect SkeletonAnimation::boundingBox() {
	float minX = FLT_MAX, minY = FLT_MAX, maxX = FLT_MIN, maxY = FLT_MIN;
	float scaleX = getScaleX(), scaleY = getScaleY();
	for (int i = 0; i < skeleton->slotsCount; ++i) {
		spSlot* slot = skeleton->slots[i];
		if (!slot->attachment) continue;
		int verticesCount;
		if (slot->attachment->type == SP_ATTACHMENT_REGION) {
			spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
			spRegionAttachment_computeWorldVertices(attachment, slot->bone, _worldVertices);
			verticesCount = 8;
		}
		else if (slot->attachment->type == SP_ATTACHMENT_MESH) {
			spMeshAttachment* mesh = (spMeshAttachment*)slot->attachment;
			spMeshAttachment_computeWorldVertices(mesh, slot, _worldVertices);
			verticesCount = mesh->verticesCount;
		}
		else if (slot->attachment->type == SP_ATTACHMENT_SKINNED_MESH) {
			spSkinnedMeshAttachment* mesh = (spSkinnedMeshAttachment*)slot->attachment;
			spSkinnedMeshAttachment_computeWorldVertices(mesh, slot, _worldVertices);
			verticesCount = mesh->uvsCount;
		}
		else
			continue;
		for (int ii = 0; ii < verticesCount; ii += 2) {
			float x = _worldVertices[ii] * scaleX, y = _worldVertices[ii + 1] * scaleY;
			minX = Min(minX, x);
			minY = Min(minY, y);
			maxX = Max(maxX, x);
			maxY = Max(maxY, y);
		}
	}
	CCPoint position = getPosition();
	return CCRect(position.x + minX, position.y + minY, maxX - minX, maxY - minY);
}

// --- Convenience methods for Skeleton_* functions.

void SkeletonAnimation::updateWorldTransform() {
	spSkeleton_updateWorldTransform(skeleton);
}

void SkeletonAnimation::setToSetupPose() {
	spSkeleton_setToSetupPose(skeleton);
}
void SkeletonAnimation::setBonesToSetupPose() {
	spSkeleton_setBonesToSetupPose(skeleton);
}
void SkeletonAnimation::setSlotsToSetupPose() {
	spSkeleton_setSlotsToSetupPose(skeleton);
}

spBone* SkeletonAnimation::findBone(const char* boneName) const {
	return spSkeleton_findBone(skeleton, boneName);
}

spSlot* SkeletonAnimation::findSlot(const char* slotName) const {
	return spSkeleton_findSlot(skeleton, slotName);
}

bool SkeletonAnimation::setSkin(const char* skinName) {
	return spSkeleton_setSkinByName(skeleton, skinName) ? true : false;
}

spAttachment* SkeletonAnimation::getAttachment(const char* slotName, const char* attachmentName) const {
	return spSkeleton_getAttachmentForSlotName(skeleton, slotName, attachmentName);
}
bool SkeletonAnimation::setAttachment(const char* slotName, const char* attachmentName) {
	return spSkeleton_setAttachment(skeleton, slotName, attachmentName) ? true : false;
}

// --- CCBlendProtocol

ccBlendFunc SkeletonAnimation::getBlendFunc() {
	return blendFunc;
}

void SkeletonAnimation::setBlendFunc(ccBlendFunc blendFunc) {
	this->blendFunc = blendFunc;
}

void SkeletonAnimation::setOpacityModifyRGB(bool value) {
	premultipliedAlpha = value;
}

bool SkeletonAnimation::isOpacityModifyRGB() {
	return premultipliedAlpha;
}
}


#include "engine/CCEventImpl.h"
#include "SpineEvents.h"