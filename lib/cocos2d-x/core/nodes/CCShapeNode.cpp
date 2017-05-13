
#include "nodes/CCShapeNode.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN

void CCShapeNode::draw(void)
{
    beforeDraw();
    drawProc();
    afterDraw();
}

void CCShapeNode::beforeDraw(void)
{
    glLineWidth(m_lineWidth);
    ccDrawColor4F(m_color.r, m_color.g, m_color.b, m_color.a);
#ifdef GL_LINE_STIPPLE
    if (m_lineStippleEnabled)
    {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, m_lineStipple);
    }
#endif
}

void CCShapeNode::afterDraw(void)
{
    glLineWidth(1);
    ccDrawColor4F(1, 1, 1, 1);
#ifdef GL_LINE_STIPPLE
    if (m_lineStippleEnabled)
    {
        glDisable(GL_LINE_STIPPLE);
        glLineStipple(1, 0xFFFF);
    }
#endif
}

CCCircleShape* CCCircleShape::create(float radius)
{
    CCCircleShape* circle = new CCCircleShape(radius);
    circle->autorelease();
    return circle;
}

CCCircleShape* CCCircleShape::create(float radius, bool fill)
{
	CCCircleShape* circle = CCCircleShape::create(radius);
	circle->setFill(fill);
	return circle;
}

void CCCircleShape::drawProc(void)
{
    ccDrawCircle(getDrawPosition(), m_radius, m_angle, m_segments, m_drawLineToCenter, m_scaleX, m_scaleY, m_fill);
}


CCRectShape* CCRectShape::create(const cocos2d::CCSize &size)
{
    CCRectShape* rect = new CCRectShape(size);
    rect->autorelease();
    return rect;
}

void CCRectShape::drawProc(void)
{
    const Vec2 center = getDrawPosition();
    float w = m_size.width / 2;
    float h = m_size.height / 2;
    
    if (m_fill)
    {
        ccDrawSolidRect(ccp(center.x - w, center.y + h), ccp(center.x + w, center.y - h), m_color);
    }
    else
    {
        ccDrawColor4F(m_color.r, m_color.g, m_color.b, m_color.a);
        ccDrawRect(ccp(center.x - w, center.y + h), ccp(center.x + w, center.y - h));
    }
}


CCPointShape* CCPointShape::create(void)
{
    CCPointShape* point = new CCPointShape();
    point->autorelease();
    return point;
}

void CCPointShape::drawProc(void)
{
    ccDrawPoint(getDrawPosition());
}

CCPolygonShape* CCPolygonShape::create(const std::vector<Vec2>& vertices)
{
    CCPolygonShape* polygon = new CCPolygonShape();
    polygon->initWithVertices(vertices);
    polygon->autorelease();
    return polygon;
}

bool CCPolygonShape::initWithVertices(const std::vector<Vec2>& vertices)
{
	_vertices = vertices;
	_drawVertives = vertices;
    return true;
}

CCPolygonShape::~CCPolygonShape(void)
{
}

void CCPolygonShape::drawProc(void)
{
    const Vec2 center = getDrawPosition();
    for (unsigned int i = 0; i < _vertices.size(); ++i)
    {
		_drawVertives[i].x = _vertices[i].x + center.x;
		_drawVertives[i].y = _vertices[i].y + center.y;
    }
    
    if (m_fill)
    {
        ccDrawSolidPoly(&_drawVertives[0], _drawVertives.size(), m_color);
    }
    else
    {
        ccDrawColor4F(m_color.r, m_color.g, m_color.b, m_color.a);
        ccDrawPoly(&_drawVertives[0], _drawVertives.size(), m_close);
    }
}

NS_CC_END
