
#pragma once

#include "base/RefCounted.h"
#include "base/Ptr.h"
#include "DTypes.h"
#include <list>

NS_DOLLAR_START

class Renderer;

class Widget : public RefCounted
{
public:
	D_VIRTUAL_GET_SET_CONST_REF(Vec2, _pos, Pos);
	D_VIRTUAL_GET_SET_CONST_REF(Vec2, _size, Size);


	void onDraw(Renderer* render);

	/// draw self
	virtual void draw(Renderer* render);
	/// draw children
	virtual void drawChildren(Renderer* render);

	virtual void addChild(Widget* child);

protected:

	std::list<SharedPtr<Widget> > _children;
};

NS_DOLLAR_END