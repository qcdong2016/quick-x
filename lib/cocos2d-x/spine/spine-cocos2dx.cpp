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

#include "spine-cocos2dx.h"
#include <spine/extension.h>
#include "engine/CCFileSystem.h"
#include "engine/CCDirector.h"
#include "SpineResource.h"
#include "engine/CCDirector.h"
#include "textures/CCTexture2D.h"


USING_NS_CC;

void _spAtlasPage_createTexture (spAtlasPage* self, const char* path) {
	CCTexture2D* texture = SubSystem::get<ResourceCache>()->getResource<CCTexture2D>(path);
	texture->retain();
	self->rendererObject = texture;
	self->width = texture->getPixelsWide();
	self->height = texture->getPixelsHigh();
}

void _spAtlasPage_disposeTexture (spAtlasPage* self) {
	((CCTexture2D*)self->rendererObject)->release();
}

char* _spUtil_readFile (const char* path, int* length) {
	SharedPtr<MemBuffer> bf = FileSystem::readAll(path);

	char* data = (char*)malloc(bf->getSize());
	memcpy(data, bf->getData(), bf->getSize());
	*length = bf->getSize();

	return data;
}

NS_CC_BEGIN

SpineModule::SpineModule()
{
	ObjectFactoryManager::addFactory<SpineSkeletonDataResource>();
	ObjectFactoryManager::addFactory<SpineAtlasResource>();
}

NS_CC_END