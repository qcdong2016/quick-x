
#include "DTypes.h"

NS_DOLLAR_START

class Renderer
{
public:
	virtual ~Renderer() {}

	virtual void beginPaint(int render_target_w, int render_target_h);
	virtual void endPaint();

	virtual void translate(int dx, int dy);

	virtual void setOpacity(float opacity);
	virtual float getOpacity();

	virtual Rect setClipRect(const Rect &rect);
	virtual Rect getClipRect();

	virtual void drawRect(const Rect& rect);
	virtual void drawRect(const Vec2& pos, const Vec2& size);
	virtual void drawRectSolid(const Rect& rect);
	virtual void drawRectSolid(const Vec2& pos, const Vec2& size);

	virtual void drawIcon(int icon, const Vec2& pos);

	virtual void drawText(const char* text, int fontsize, const Vec2& pos);
};

NS_DOLLAR_END
