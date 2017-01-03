
#include "NoUI.h"
#include "CCPlatformMacros.h"
#include "CCPlatformConfig.h"
#include "engine/CCDirector.h"


#include "imgui/nanovg.h"

#if OS_IS(MAC)
#define NANOVG_GL2_IMPLEMENTATION
#else
#define NANOVG_GL3_IMPLEMENTATION
#endif
#include "imgui/nanovg_gl.h"

#include "engine/CCEngineEvents.h"
#include "engine/CCFileSystem.h"

NS_CC_BEGIN


static NVGcontext* NCreate() {
#if OS_IS(MAC)
    return nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#else
	return nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#endif
}

static void NDelete(NVGcontext* nvg) {
#if OS_IS(MAC)
    return nvgDeleteGL2(nvg);
#else
	return nvgDeleteGL3(nvg);
#endif
}

// Returns 1 if col.rgba is 0.0f,0.0f,0.0f,0.0f, 0 otherwise
int isBlack(NVGcolor col)
{
	if (col.r == 0.0f && col.g == 0.0f && col.b == 0.0f && col.a == 0.0f)
	{
		return 1;
	}
	return 0;
}

static char* cpToUTF8(int cp, char* str)
{
	int n = 0;
	if (cp < 0x80) n = 1;
	else if (cp < 0x800) n = 2;
	else if (cp < 0x10000) n = 3;
	else if (cp < 0x200000) n = 4;
	else if (cp < 0x4000000) n = 5;
	else if (cp <= 0x7fffffff) n = 6;
	str[n] = '\0';
	switch (n) {
	case 6: str[5] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x4000000;
	case 5: str[4] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x200000;
	case 4: str[3] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x10000;
	case 3: str[2] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x800;
	case 2: str[1] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0xc0;
	case 1: str[0] = cp;
	}
	return str;
}
#define ICON_SEARCH 0x1F50D
#define ICON_CIRCLED_CROSS 0x2716
#define ICON_CHEVRON_RIGHT 0xE75E
#define ICON_CHECK 0x2713
#define ICON_LOGIN 0xE740
#define ICON_TRASH 0xE729
void drawSearchBox(NVGcontext* vg, const char* text, float x, float y, float w, float h)
{
	NVGpaint bg;
	char icon[8];
	float cornerRadius = h / 2 - 1;

	// Edit
	bg = nvgBoxGradient(vg, x, y + 1.5f, w, h, h / 2, 5, nvgRGBA(0, 0, 0, 16), nvgRGBA(0, 0, 0, 92));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x, y, w, h, cornerRadius);
	nvgFillPaint(vg, bg);
	nvgFill(vg);

	/*	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+0.5f,y+0.5f, w-1,h-1, cornerRadius-0.5f);
	nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
	nvgStroke(vg);*/

	nvgFontSize(vg, h*1.3f);
	nvgFontFace(vg, "icons");
	nvgFillColor(vg, nvgRGBA(255, 255, 255, 64));
	nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	nvgText(vg, x + h*0.55f, y + h*0.55f, cpToUTF8(ICON_SEARCH, icon), NULL);

	nvgFontSize(vg, 20.0f);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(255, 255, 255, 32));

	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
	nvgText(vg, x + h*1.05f, y + h*0.5f, text, NULL);

	nvgFontSize(vg, h*1.3f);
	nvgFontFace(vg, "icons");
	nvgFillColor(vg, nvgRGBA(255, 255, 255, 32));
	nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	nvgText(vg, x + w - h*0.55f, y + h*0.55f, cpToUTF8(ICON_CIRCLED_CROSS, icon), NULL);
}

void drawWindow(NVGcontext* vg, const char* title, float x, float y, float w, float h)
{
	float cornerRadius = 3.0f;
	NVGpaint shadowPaint;
	NVGpaint headerPaint;

	nvgSave(vg);
	//	nvgClearState(vg);

	// Window
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x, y, w, h, cornerRadius);
	nvgFillColor(vg, nvgRGBA(28, 30, 34, 192));
	//	nvgFillColor(vg, nvgRGBA(0,0,0,128));
	nvgFill(vg);

	// Drop shadow
	shadowPaint = nvgBoxGradient(vg, x, y + 2, w, h, cornerRadius * 2, 10, nvgRGBA(0, 0, 0, 128), nvgRGBA(0, 0, 0, 0));
	nvgBeginPath(vg);
	nvgRect(vg, x - 10, y - 10, w + 20, h + 30);
	nvgRoundedRect(vg, x, y, w, h, cornerRadius);
	nvgPathWinding(vg, NVG_HOLE);
	nvgFillPaint(vg, shadowPaint);
	nvgFill(vg);

	// Header
	headerPaint = nvgLinearGradient(vg, x, y, x, y + 15, nvgRGBA(255, 255, 255, 8), nvgRGBA(0, 0, 0, 16));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x + 1, y + 1, w - 2, 30, cornerRadius - 1);
	nvgFillPaint(vg, headerPaint);
	nvgFill(vg);
	nvgBeginPath(vg);
	nvgMoveTo(vg, x + 0.5f, y + 0.5f + 30);
	nvgLineTo(vg, x + 0.5f + w - 1, y + 0.5f + 30);
	nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 32));
	nvgStroke(vg);

	nvgFontSize(vg, 18.0f);
	nvgFontFace(vg, "sans-bold");
	nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

	nvgFontBlur(vg, 2);
	nvgFillColor(vg, nvgRGBA(0, 0, 0, 128));
	nvgText(vg, x + w / 2, y + 16 + 1, title, NULL);

	nvgFontBlur(vg, 0);
	nvgFillColor(vg, nvgRGBA(220, 220, 220, 160));
	nvgText(vg, x + w / 2, y + 16, title, NULL);

	nvgRestore(vg);
}

void drawCheckBox(NVGcontext* vg, const char* text, float x, float y, float w, float h)
{
	NVGpaint bg;
	char icon[8];
	NVG_NOTUSED(w);

	nvgFontSize(vg, 18.0f);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(255, 255, 255, 160));

	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
	nvgText(vg, x + 28, y + h*0.5f, text, NULL);

	bg = nvgBoxGradient(vg, x + 1, y + (int)(h*0.5f) - 9 + 1, 18, 18, 3, 3, nvgRGBA(0, 0, 0, 32), nvgRGBA(0, 0, 0, 92));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x + 1, y + (int)(h*0.5f) - 9, 18, 18, 3);
	nvgFillPaint(vg, bg);
	nvgFill(vg);

	nvgFontSize(vg, 40);
	nvgFontFace(vg, "icons");
	nvgFillColor(vg, nvgRGBA(255, 255, 255, 128));
	nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	nvgText(vg, x + 9 + 2, y + h*0.5f, cpToUTF8(ICON_CHECK, icon), NULL);
}
void drawEditBoxBase(NVGcontext* vg, float x, float y, float w, float h)
{
	NVGpaint bg;
	// Edit
	bg = nvgBoxGradient(vg, x + 1, y + 1 + 1.5f, w - 2, h - 2, 3, 4, nvgRGBA(255, 255, 255, 32), nvgRGBA(32, 32, 32, 32));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x + 1, y + 1, w - 2, h - 2, 4 - 1);
	nvgFillPaint(vg, bg);
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgRoundedRect(vg, x + 0.5f, y + 0.5f, w - 1, h - 1, 4 - 0.5f);
	nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 48));
	nvgStroke(vg);
}

void drawEditBox(NVGcontext* vg, const char* text, float x, float y, float w, float h)
{

	drawEditBoxBase(vg, x, y, w, h);

	nvgFontSize(vg, 20.0f);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(255, 255, 255, 64));
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
	nvgText(vg, x + h*0.3f, y + h*0.5f, text, NULL);
}

void drawEditBoxNum(NVGcontext* vg,
	const char* text, const char* units, float x, float y, float w, float h)
{
	float uw;

	drawEditBoxBase(vg, x, y, w, h);

	uw = nvgTextBounds(vg, 0, 0, units, NULL, NULL);

	nvgFontSize(vg, 18.0f);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(255, 255, 255, 64));
	nvgTextAlign(vg, NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
	nvgText(vg, x + w - h*0.3f, y + h*0.5f, units, NULL);

	nvgFontSize(vg, 20.0f);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(255, 255, 255, 128));
	nvgTextAlign(vg, NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
	nvgText(vg, x + w - uw - h*0.5f, y + h*0.5f, text, NULL);
}
void drawButton(NVGcontext* vg, int preicon, const char* text, float x, float y, float w, float h, NVGcolor col)
{
	NVGpaint bg;
	char icon[8];
	float cornerRadius = 4.0f;
	float tw = 0, iw = 0;

	bg = nvgLinearGradient(vg, x, y, x, y + h, nvgRGBA(255, 255, 255, isBlack(col) ? 16 : 32), nvgRGBA(0, 0, 0, isBlack(col) ? 16 : 32));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x + 1, y + 1, w - 2, h - 2, cornerRadius - 1);
	if (!isBlack(col)) {
		nvgFillColor(vg, col);
		nvgFill(vg);
	}
	nvgFillPaint(vg, bg);
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgRoundedRect(vg, x + 0.5f, y + 0.5f, w - 1, h - 1, cornerRadius - 0.5f);
	nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 48));
	nvgStroke(vg);

	nvgFontSize(vg, 20.0f);
	nvgFontFace(vg, "sans-bold");
	tw = nvgTextBounds(vg, 0, 0, text, NULL, NULL);
	if (preicon != 0) {
		nvgFontSize(vg, h*1.3f);
		nvgFontFace(vg, "icons");
		iw = nvgTextBounds(vg, 0, 0, cpToUTF8(preicon, icon), NULL, NULL);
		iw += h*0.15f;
	}

	if (preicon != 0) {
		nvgFontSize(vg, h*1.3f);
		nvgFontFace(vg, "icons");
		nvgFillColor(vg, nvgRGBA(255, 255, 255, 96));
		nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
		nvgText(vg, x + w*0.5f - tw*0.5f - iw*0.75f, y + h*0.5f, cpToUTF8(preicon, icon), NULL);
	}

	nvgFontSize(vg, 20.0f);
	nvgFontFace(vg, "sans-bold");
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
	nvgFillColor(vg, nvgRGBA(0, 0, 0, 160));
	nvgText(vg, x + w*0.5f - tw*0.5f + iw*0.25f, y + h*0.5f - 1, text, NULL);
	nvgFillColor(vg, nvgRGBA(255, 255, 255, 160));
	nvgText(vg, x + w*0.5f - tw*0.5f + iw*0.25f, y + h*0.5f, text, NULL);
}

void drawLabel(NVGcontext* vg, const char* text, float x, float y, float w, float h)
{
	NVG_NOTUSED(w);

	nvgFontSize(vg, 18.0f);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(255, 255, 255, 128));

	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
	nvgText(vg, x, y + h*0.5f, text, NULL);
}

void drawDropDown(NVGcontext* vg, const char* text, float x, float y, float w, float h)
{
	NVGpaint bg;
	char icon[8];
	float cornerRadius = 4.0f;

	bg = nvgLinearGradient(vg, x, y, x, y + h, nvgRGBA(255, 255, 255, 16), nvgRGBA(0, 0, 0, 16));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x + 1, y + 1, w - 2, h - 2, cornerRadius - 1);
	nvgFillPaint(vg, bg);
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgRoundedRect(vg, x + 0.5f, y + 0.5f, w - 1, h - 1, cornerRadius - 0.5f);
	nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 48));
	nvgStroke(vg);

	nvgFontSize(vg, 20.0f);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(255, 255, 255, 160));
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
	nvgText(vg, x + h*0.3f, y + h*0.5f, text, NULL);

	nvgFontSize(vg, h*1.3f);
	nvgFontFace(vg, "icons");
	nvgFillColor(vg, nvgRGBA(255, 255, 255, 64));
	nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	nvgText(vg, x + w - h*0.5f, y + h*0.5f, cpToUTF8(ICON_CHEVRON_RIGHT, icon), NULL);
}

void drawSlider(NVGcontext* vg, float pos, float x, float y, float w, float h)
{
	NVGpaint bg, knob;
	float cy = y + (int)(h*0.5f);
	float kr = (int)(h*0.25f);

	nvgSave(vg);
	//	nvgClearState(vg);

	// Slot
	bg = nvgBoxGradient(vg, x, cy - 2 + 1, w, 4, 2, 2, nvgRGBA(0, 0, 0, 32), nvgRGBA(0, 0, 0, 128));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x, cy - 2, w, 4, 2);
	nvgFillPaint(vg, bg);
	nvgFill(vg);

	// Knob Shadow
	bg = nvgRadialGradient(vg, x + (int)(pos*w), cy + 1, kr - 3, kr + 3, nvgRGBA(0, 0, 0, 64), nvgRGBA(0, 0, 0, 0));
	nvgBeginPath(vg);
	nvgRect(vg, x + (int)(pos*w) - kr - 5, cy - kr - 5, kr * 2 + 5 + 5, kr * 2 + 5 + 5 + 3);
	nvgCircle(vg, x + (int)(pos*w), cy, kr);
	nvgPathWinding(vg, NVG_HOLE);
	nvgFillPaint(vg, bg);
	nvgFill(vg);

	// Knob
	knob = nvgLinearGradient(vg, x, cy - kr, x, cy + kr, nvgRGBA(255, 255, 255, 16), nvgRGBA(0, 0, 0, 16));
	nvgBeginPath(vg);
	nvgCircle(vg, x + (int)(pos*w), cy, kr - 1);
	nvgFillColor(vg, nvgRGBA(40, 43, 48, 255));
	nvgFill(vg);
	nvgFillPaint(vg, knob);
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgCircle(vg, x + (int)(pos*w), cy, kr - 0.5f);
	nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 92));
	nvgStroke(vg);

	nvgRestore(vg);
}


void NoUIModule::draw(EventData& data)
{
	NVGcontext* vg = (NVGcontext*)_nvg;

	CCSize win = CCDirector::sharedDirector()->getWinSize();

	nvgBeginFrame(vg, win.width, win.height, win.width / win.height);
	drawWindow(vg, "Widgets `n Stuff", 50, 50, 300, 400);

	int x, y;
	x = 60; y = 95;


	drawSearchBox(vg, "Search", x, y, 280, 25);
	y += 40;
	drawDropDown(vg, "Effects", x, y, 280, 28);
	y += 45;

	// Form
	drawLabel(vg, "Login", x, y, 280, 20);
	y += 25;
	drawEditBox(vg, "Email", x, y, 280, 28);
	y += 35;
	drawEditBox(vg, "Password", x, y, 280, 28);
	y += 38;
	drawCheckBox(vg, "Remember me", x, y, 140, 28);
	drawButton(vg, ICON_LOGIN, "Sign in", x + 138, y, 140, 28, nvgRGBA(0, 96, 128, 255));
	y += 45;
	// Slider
	drawLabel(vg, "Diameter", x, y, 280, 20);
	y += 25;
	drawEditBoxNum(vg, "123.00", "px", x + 180, y, 100, 28);
	drawSlider(vg, 0.4f, x, y, 170, 28);
	y += 55;

	drawButton(vg, ICON_TRASH, "Delete", x, y, 160, 28, nvgRGBA(128, 16, 8, 255));
	drawButton(vg, 0, "Cancel", x + 170, y, 110, 28, nvgRGBA(0, 0, 0, 0));

	nvgEndFrame(vg);
}


NoUIModule::NoUIModule()
{
	subscribeToEvent<AfterDraw>(Handler(this, &NoUIModule::draw));

	NVGcontext* vg = NCreate();
	_nvg = vg;

	std::string icon_ttf = FileSystem::fullPathOfFile("res/font/entypo.ttf");
	std::string normal_ttf = FileSystem::fullPathOfFile("res/font/Roboto-Regular.ttf");
	std::string bold_ttf = FileSystem::fullPathOfFile("res/font/Roboto-Bold.ttf");

	int fontIcons = nvgCreateFont(vg, "icons", icon_ttf.c_str());
	if (fontIcons == -1) {
		printf("Could not add font icons.\n");
		return;
	}
	int fontNormal = nvgCreateFont(vg, "sans", normal_ttf.c_str());
	if (fontNormal == -1) {
		printf("Could not add font italic.\n");
		return;
	}
	int fontBold = nvgCreateFont(vg, "sans-bold", bold_ttf.c_str());
	if (fontBold == -1) {
		printf("Could not add font bold.\n");
		return;
	}
}

NoUIModule::~NoUIModule()
{
	NDelete((NVGcontext*)_nvg);
}


NS_CC_END
