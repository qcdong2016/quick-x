#include "DWidget.h"

NS_DOLLAR_START

void Widget::onDraw(Renderer* render)
{
	draw(render);

	render->translate(_pos.x, _pos.y);

	drawChildren(render);
}

void Widget::draw(Renderer* render)
{
	render->drawRect(_pos, _size);
}

void Widget::drawChildren(Renderer* render)
{
	for (auto& child : _children) {
		child->onDraw(render);
	}
}

void Widget::addChild(Widget* child)
{
	_children.push_back(SharedPtr<Widget>(child));
}

NS_DOLLAR_END

