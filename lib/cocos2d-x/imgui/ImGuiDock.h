#pragma once
#include "imgui/imgui.h"
#include <functional>
#include <vector>
#include <string>

namespace ImGuiDock
{
	enum class DockSlot { Left, Right, Top, Bottom, Tab, None };

	struct Dock;
	class Dockspace;

	struct Container
	{
		Container *splits[2];
		Container *parent;
		Dock *activeDock;

		std::vector<Dock*>docks;

		bool verticalSplit;
		bool alwaysAutoResize;
		float size;

		Container() {
			splits[0] = nullptr;
			splits[1] = nullptr;
			parent = nullptr;
			activeDock = nullptr;
			verticalSplit = false;
			alwaysAutoResize = true;
			size = 0;
		}
	};

	struct Dock
	{
		Dock *initialize(const char *dtitle, bool dcloseButton, ImVec2 dminSize, std::function<void(ImVec2)> ddrawFunction)
		{
			title = dtitle;
			closeButton = dcloseButton;
			minSize = dminSize;
			drawFunction = ddrawFunction;
			return this;
		};

		//Container *parent = nullptr;
		Container *container = nullptr;
		Dockspace *redockFrom = nullptr;
		Dock *redockTo = nullptr;

		const char *title;
		DockSlot dockSlot = DockSlot::Tab;
		DockSlot redockSlot = DockSlot::None;
		bool closeButton = true;
		bool undockable = false;
		bool draging = false;
		ImVec2 lastSize;
		ImVec2 minSize;

		std::function<void(ImVec2)> drawFunction;
		std::function<bool(void)> onCloseFunction;
	};

	class Dockspace
	{
	public:
		Dockspace();
		~Dockspace();

		bool dock(Dock *dock, DockSlot dockSlot, float size = 0, bool active = false);
		bool dockWith(Dock *dock, Dock *dockTo, DockSlot dockSlot, float size = 0, bool active = false);
		bool undock(Dock *dock);

		void updateAndDraw(ImVec2 size);
		void clear();

		std::vector<Dock*>m_docks;

		Container m_container;
		std::vector<Container*>m_containers;

	protected:

		void _renderTabBar(Container *container, ImVec2 size, ImVec2 cursorPos);
		bool _getMinSize(Container *container, ImVec2 *min);
		void _renderContainer(unsigned& idgen, Container* container, ImVec2 size, ImVec2 cursor_pos);

		enum DockToAction
		{
			eUndock, eDrag, eClose, eNull
		};

		Dock *m_currentDockTo = nullptr;
		DockToAction m_currentDockToAction = eNull;
	};
};
