#ifndef _IMGUI_COCOS2DX_H_
#define _IMGUI_COCOS2DX_H_


#include "CCPlatformMacros.h"
#include "base/Ptr.h"
#include "base/RefCounted.h"
#include <string>
#include <map>
#include "engine/CCObject.h"

NS_CC_BEGIN;

class UIInterface : public CCObject
{
public:
	virtual void draw() = 0;

	bool visible;

	UIInterface() : visible(true)
	{
	}
};


class ImGuiCC
{
public:
	static void init();
	static void draw();
	
	static void setFont(const char* font, float size);
	static void setMouseDown(bool down);
	static bool setMousePos(int x, int y);

	static void setKeyDown(int k);
	static void setKeyUp(int k);
	static void setChar(int c);

	static void toggleVisible();
	static void setVisible(bool v);

	template<typename T>
	static void add()
	{
		SharedPtr<UIInterface> ui(new T);
		_interfaces[T::name()] = ui;
	}

	template<typename T>
	static T* get()
	{
		auto it = _interfaces.find(T::name());
		if (it == _interfaces.end())
			return nullptr;

		return (T*)it->second.Get();
	}

	static std::map<std::string, SharedPtr<UIInterface> > _interfaces;
};

NS_CC_END;

#endif // !_IMGUI_COCOS2DX_H_


