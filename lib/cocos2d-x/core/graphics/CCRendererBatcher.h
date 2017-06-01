#pragma once

#include <SDL_config.h>
#include <vector>
#include "cocoa/CCGeometry.h"

namespace cocos2d
{

struct Vertex
{
	float x, y;
	float u, v;
	float r, g, b, a;
};

struct Batch
{
	std::vector<unsigned short> _indexBuffer;
	std::vector<Vertex> _vertexBuffer;
};

class RendererBatcher
{
public:
	void addLine(const Vec2& from, const Vec2& to, uint32_t col);
	void addRect(const Vec2& pos, const Vec2& size, uint32_t col);
};

}