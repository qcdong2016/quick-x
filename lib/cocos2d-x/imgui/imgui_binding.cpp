#include "scripting/CCLuaEngine.h"
#include "imgui/imgui.h"
#include "scripting/LuaFunction.h"
#include "cocos/Ptr.h"
#include <vector>
#include "imgui_cocos2dx.h"
#include "PlayerUI.h"
USING_NS_CC;

static int lua_push_ImVec2(lua_State* L, const ImVec2& vec2)
{
	lua_pushnumber(L, vec2.x);
	lua_pushnumber(L, vec2.y);
	return 2;
}
static int lua_push_ImVec4(lua_State* L, const ImVec4& vec2)
{
	lua_pushnumber(L, vec2.x);
	lua_pushnumber(L, vec2.y);
	lua_pushnumber(L, vec2.z);
	lua_pushnumber(L, vec2.w);
	return 4;
}

//     IMGUI_API void          NewFrame();                                 
static int imgui_lua_NewFrame(lua_State* L) {
	ImGui::NewFrame();
	return 0;
}

//     IMGUI_API void          Render();                                   
static int imgui_lua_Render(lua_State* L) {
	ImGui::Render();
	return 0;
}

//     IMGUI_API void          ShowUserGuide();                            
static int imgui_lua_ShowUserGuide(lua_State* L) {
	ImGui::ShowUserGuide();
	return 0;
}

//     IMGUI_API void          ShowTestWindow(bool* p_open = NULL);        
static int imgui_lua_ShowTestWindow(lua_State* L) {
	int arg = 1;
	bool p_open_arg = lua_toboolean(L, arg++);
	bool* p_open = &p_open_arg;
	ImGui::ShowTestWindow(p_open);
	lua_pushboolean(L, *p_open);
	return 1;
}

//     IMGUI_API void          ShowMetricsWindow(bool* p_open = NULL);     
static int imgui_lua_ShowMetricsWindow(lua_State* L) {
	int arg = 1;
	bool p_open_arg = lua_toboolean(L, arg++);
	bool* p_open = &p_open_arg;
	ImGui::ShowMetricsWindow(p_open);
	lua_pushboolean(L, *p_open);
	return 1;
}

//     IMGUI_API bool          Begin(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0);                                                   
static int imgui_lua_Begin(lua_State* L) {
	int arg = 1;
	const char* name = lua_tostring(L, arg++);
	bool p_open_arg = lua_toboolean(L, arg++);
	bool* p_open = &p_open_arg;
	ImGuiWindowFlags flags = luaL_optint(L, arg++, 0);
	bool ret = ImGui::Begin(name, p_open, flags);
	lua_pushboolean(L, ret);
	lua_pushboolean(L, *p_open);
	return 2;
}

//     IMGUI_API void          End();                                                                                                                      
static int imgui_lua_End(lua_State* L) {
	ImGui::End();
	return 0;
}

//     IMGUI_API void          EndChild();
static int imgui_lua_EndChild(lua_State* L) {
	ImGui::EndChild();
	return 0;
}

//     IMGUI_API ImVec2        GetContentRegionMax();                                              
static int imgui_lua_GetContentRegionMax(lua_State* L) {
	ImVec2 ret = ImGui::GetContentRegionMax();
	return lua_push_ImVec2(L, ret);
}

//     IMGUI_API ImVec2        GetContentRegionAvail();                                            
static int imgui_lua_GetContentRegionAvail(lua_State* L) {
	ImVec2 ret = ImGui::GetContentRegionAvail();
	return lua_push_ImVec2(L, ret);
}

//     IMGUI_API float         GetContentRegionAvailWidth();                                       
static int imgui_lua_GetContentRegionAvailWidth(lua_State* L) {
	float ret = ImGui::GetContentRegionAvailWidth();
	lua_pushnumber(L, ret);
	return 1;
}

//     IMGUI_API ImVec2        GetWindowContentRegionMin();                                        
static int imgui_lua_GetWindowContentRegionMin(lua_State* L) {
	ImVec2 ret = ImGui::GetWindowContentRegionMin();
	return lua_push_ImVec2(L, ret);
}

//     IMGUI_API ImVec2        GetWindowContentRegionMax();                                        
static int imgui_lua_GetWindowContentRegionMax(lua_State* L) {
	ImVec2 ret = ImGui::GetWindowContentRegionMax();
	return lua_push_ImVec2(L, ret);
}

//     IMGUI_API float         GetWindowContentRegionWidth();                                      
static int imgui_lua_GetWindowContentRegionWidth(lua_State* L) {
	float ret = ImGui::GetWindowContentRegionWidth();
	lua_pushnumber(L, ret);
	return 1;
}

//     IMGUI_API ImVec2        GetWindowPos();                                                     
static int imgui_lua_GetWindowPos(lua_State* L) {
	ImVec2 ret = ImGui::GetWindowPos();
	return lua_push_ImVec2(L, ret);
}

//     IMGUI_API ImVec2        GetWindowSize();                                                    
static int imgui_lua_GetWindowSize(lua_State* L) {
	ImVec2 ret = ImGui::GetWindowSize();
	return lua_push_ImVec2(L, ret);
}

//     IMGUI_API float         GetWindowWidth();
static int imgui_lua_GetWindowWidth(lua_State* L) {
	float ret = ImGui::GetWindowWidth();
	lua_pushnumber(L, ret);
	return 1;
}

//     IMGUI_API float         GetWindowHeight();
static int imgui_lua_GetWindowHeight(lua_State* L) {
	float ret = ImGui::GetWindowHeight();
	lua_pushnumber(L, ret);
	return 1;
}

//     IMGUI_API bool          IsWindowCollapsed();
static int imgui_lua_IsWindowCollapsed(lua_State* L) {
	bool ret = ImGui::IsWindowCollapsed();
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API void          SetWindowFontScale(float scale);                                    
static int imgui_lua_SetWindowFontScale(lua_State* L) {
	int arg = 1;
	float scale = lua_tonumber(L, arg++);
	ImGui::SetWindowFontScale(scale);
	return 0;
}

//     IMGUI_API void          SetNextWindowPosCenter(ImGuiSetCond cond = 0);                      
static int imgui_lua_SetNextWindowPosCenter(lua_State* L) {
	int arg = 1;
	ImGuiSetCond cond = luaL_optint(L, arg++, 0);
	ImGui::SetNextWindowPosCenter(cond);
	return 0;
}

//     IMGUI_API void          SetNextWindowContentWidth(float width);                             
static int imgui_lua_SetNextWindowContentWidth(lua_State* L) {
	int arg = 1;
	float width = lua_tonumber(L, arg++);
	ImGui::SetNextWindowContentWidth(width);
	return 0;
}

//     IMGUI_API void          SetNextWindowCollapsed(bool collapsed, ImGuiSetCond cond = 0);      
static int imgui_lua_SetNextWindowCollapsed(lua_State* L) {
	int arg = 1;
	bool collapsed = lua_toboolean(L, arg++);
	ImGuiSetCond cond = luaL_optint(L, arg++, 0);
	ImGui::SetNextWindowCollapsed(collapsed, cond);
	return 0;
}

//     IMGUI_API void          SetNextWindowFocus();                                               
static int imgui_lua_SetNextWindowFocus(lua_State* L) {
	ImGui::SetNextWindowFocus();
	return 0;
}

//     IMGUI_API void          SetWindowCollapsed(bool collapsed, ImGuiSetCond cond = 0);          
//     IMGUI_API void          SetWindowCollapsed(const char* name, bool collapsed, ImGuiSetCond cond = 0);   
static int imgui_lua_SetWindowCollapsed(lua_State* L) {
	int arg = 1;
	if (lua_type(L, 1) == LUA_TSTRING)
	{
		const char* name = lua_tostring(L, arg++);
		bool collapsed = lua_toboolean(L, arg++);
		ImGuiSetCond cond = luaL_optint(L, arg++, 0);
		ImGui::SetWindowCollapsed(name, collapsed, cond);
	}
	else
	{
		bool collapsed = lua_toboolean(L, arg++);
		ImGuiSetCond cond = luaL_optint(L, arg++, 0);
		ImGui::SetWindowCollapsed(collapsed, cond);
	}
	return 0;
}

//     IMGUI_API void          SetWindowFocus();                                                   
//     IMGUI_API void          SetWindowFocus(const char* name);                                              
static int imgui_lua_SetWindowFocus(lua_State* L) {
	if (lua_type(L, 1) == LUA_TSTRING)
	{
		int arg = 1;
		const char* name = lua_tostring(L, arg++);
		ImGui::SetWindowFocus(name);
	}
	else
	{
		ImGui::SetWindowFocus();
	}
	return 0;
}

//     IMGUI_API float         GetScrollX();                                                       
static int imgui_lua_GetScrollX(lua_State* L) {
	float ret = ImGui::GetScrollX();
	lua_pushnumber(L, ret);
	return 1;
}

//     IMGUI_API float         GetScrollY();                                                       
static int imgui_lua_GetScrollY(lua_State* L) {
	float ret = ImGui::GetScrollY();
	lua_pushnumber(L, ret);
	return 1;
}

//     IMGUI_API float         GetScrollMaxX();                                                    
static int imgui_lua_GetScrollMaxX(lua_State* L) {
	float ret = ImGui::GetScrollMaxX();
	lua_pushnumber(L, ret);
	return 1;
}

//     IMGUI_API float         GetScrollMaxY();                                                    
static int imgui_lua_GetScrollMaxY(lua_State* L) {
	float ret = ImGui::GetScrollMaxY();
	lua_pushnumber(L, ret);
	return 1;
}

//     IMGUI_API void          SetScrollX(float scroll_x);                                         
static int imgui_lua_SetScrollX(lua_State* L) {
	int arg = 1;
	float scroll_x = lua_tonumber(L, arg++);
	ImGui::SetScrollX(scroll_x);
	return 0;
}

//     IMGUI_API void          SetScrollY(float scroll_y);                                         
static int imgui_lua_SetScrollY(lua_State* L) {
	int arg = 1;
	float scroll_y = lua_tonumber(L, arg++);
	ImGui::SetScrollY(scroll_y);
	return 0;
}

//     IMGUI_API void          SetScrollHere(float center_y_ratio = 0.5f);                         
static int imgui_lua_SetScrollHere(lua_State* L) {
	int arg = 1;
	float center_y_ratio = luaL_optnumber(L, arg++, 0.5f);
	ImGui::SetScrollHere(center_y_ratio);
	return 0;
}

//     IMGUI_API void          SetScrollFromPosY(float pos_y, float center_y_ratio = 0.5f);        
static int imgui_lua_SetScrollFromPosY(lua_State* L) {
	int arg = 1;
	float pos_y = lua_tonumber(L, arg++);
	float center_y_ratio = luaL_optnumber(L, arg++, 0.5f);
	ImGui::SetScrollFromPosY(pos_y, center_y_ratio);
	return 0;
}

//     IMGUI_API void          SetKeyboardFocusHere(int offset = 0);                               
static int imgui_lua_SetKeyboardFocusHere(lua_State* L) {
	int arg = 1;
	int offset = luaL_optint(L, arg++, 0);
	ImGui::SetKeyboardFocusHere(offset);
	return 0;
}

//     IMGUI_API void          PopFont();
static int imgui_lua_PopFont(lua_State* L) {
	ImGui::PopFont();
	return 0;
}

//     IMGUI_API void          PopStyleColor(int count = 1);
static int imgui_lua_PopStyleColor(lua_State* L) {
	int arg = 1;
	int count = luaL_optint(L, arg++, 1);
	ImGui::PopStyleColor(count);
	return 0;
}

//     IMGUI_API void          PushStyleVar(ImGuiStyleVar idx, float val);
static int imgui_lua_PushStyleVar(lua_State* L) {
	int arg = 1;
	ImGuiStyleVar idx = lua_tointeger(L, arg++);
	float val = lua_tonumber(L, arg++);
	ImGui::PushStyleVar(idx, val);
	return 0;
}

//     IMGUI_API void          PopStyleVar(int count = 1);
static int imgui_lua_PopStyleVar(lua_State* L) {
	int arg = 1;
	int count = luaL_optint(L, arg++, 1);
	ImGui::PopStyleVar(count);
	return 0;
}


//     IMGUI_API float         GetFontSize();                                                      
static int imgui_lua_GetFontSize(lua_State* L) {
	float ret = ImGui::GetFontSize();
	lua_pushnumber(L, ret);
	return 1;
}

//     IMGUI_API ImVec2        GetFontTexUvWhitePixel();                                           
static int imgui_lua_GetFontTexUvWhitePixel(lua_State* L) {
	ImVec2 ret = ImGui::GetFontTexUvWhitePixel();
	return lua_push_ImVec2(L, ret);
}

//     IMGUI_API ImU32         GetColorU32(ImGuiCol idx, float alpha_mul = 1.0f);                  
static int imgui_lua_GetColorU32(lua_State* L) {
	int arg = 1;
	ImGuiCol idx = lua_tointeger(L, arg++);
	float alpha_mul = luaL_optnumber(L, arg++, 1.0f);
	ImU32 ret = ImGui::GetColorU32(idx, alpha_mul);
	lua_pushinteger(L, ret);
	return 1;
}

//     IMGUI_API void          PushItemWidth(float item_width);                                    
static int imgui_lua_PushItemWidth(lua_State* L) {
	int arg = 1;
	float item_width = lua_tonumber(L, arg++);
	ImGui::PushItemWidth(item_width);
	return 0;
}

//     IMGUI_API void          PopItemWidth();
static int imgui_lua_PopItemWidth(lua_State* L) {
	ImGui::PopItemWidth();
	return 0;
}

//     IMGUI_API float         CalcItemWidth();                                                    
static int imgui_lua_CalcItemWidth(lua_State* L) {
	float ret = ImGui::CalcItemWidth();
	lua_pushnumber(L, ret);
	return 1;
}

//     IMGUI_API void          PushTextWrapPos(float wrap_pos_x = 0.0f);                           
static int imgui_lua_PushTextWrapPos(lua_State* L) {
	int arg = 1;
	float wrap_pos_x = luaL_optnumber(L, arg++, 0.0f);
	ImGui::PushTextWrapPos(wrap_pos_x);
	return 0;
}

//     IMGUI_API void          PopTextWrapPos();
static int imgui_lua_PopTextWrapPos(lua_State* L) {
	ImGui::PopTextWrapPos();
	return 0;
}

//     IMGUI_API void          PushAllowKeyboardFocus(bool v);                                     
static int imgui_lua_PushAllowKeyboardFocus(lua_State* L) {
	int arg = 1;
	bool v = lua_toboolean(L, arg++);
	ImGui::PushAllowKeyboardFocus(v);
	return 0;
}

//     IMGUI_API void          PopAllowKeyboardFocus();
static int imgui_lua_PopAllowKeyboardFocus(lua_State* L) {
	ImGui::PopAllowKeyboardFocus();
	return 0;
}

//     IMGUI_API void          PushButtonRepeat(bool repeat);                                      
static int imgui_lua_PushButtonRepeat(lua_State* L) {
	int arg = 1;
	bool repeat = lua_toboolean(L, arg++);
	ImGui::PushButtonRepeat(repeat);
	return 0;
}

//     IMGUI_API void          PopButtonRepeat();
static int imgui_lua_PopButtonRepeat(lua_State* L) {
	ImGui::PopButtonRepeat();
	return 0;
}

//     IMGUI_API void          Separator();                                                        
static int imgui_lua_Separator(lua_State* L) {
	ImGui::Separator();
	return 0;
}

//     IMGUI_API void          SameLine(float pos_x = 0.0f, float spacing_w = -1.0f);              
static int imgui_lua_SameLine(lua_State* L) {
	int arg = 1;
	float pos_x = luaL_optnumber(L, arg++, 0.0f);
	float spacing_w = luaL_optnumber(L, arg++, -1.0f);
	ImGui::SameLine(pos_x, spacing_w);
	return 0;
}

//     IMGUI_API void          NewLine();                                                          
static int imgui_lua_NewLine(lua_State* L) {
	ImGui::NewLine();
	return 0;
}

//     IMGUI_API void          Spacing();                                                          
static int imgui_lua_Spacing(lua_State* L) {
	ImGui::Spacing();
	return 0;
}

//     IMGUI_API void          Indent(float indent_w = 0.0f);                                      
static int imgui_lua_Indent(lua_State* L) {
	int arg = 1;
	float indent_w = luaL_optnumber(L, arg++, 0.0f);
	ImGui::Indent(indent_w);
	return 0;
}

//     IMGUI_API void          Unindent(float indent_w = 0.0f);                                    
static int imgui_lua_Unindent(lua_State* L) {
	int arg = 1;
	float indent_w = luaL_optnumber(L, arg++, 0.0f);
	ImGui::Unindent(indent_w);
	return 0;
}

//     IMGUI_API void          BeginGroup();                                                       
static int imgui_lua_BeginGroup(lua_State* L) {
	ImGui::BeginGroup();
	return 0;
}

//     IMGUI_API void          EndGroup();
static int imgui_lua_EndGroup(lua_State* L) {
	ImGui::EndGroup();
	return 0;
}

//     IMGUI_API ImVec2        GetCursorPos();                                                     
static int imgui_lua_GetCursorPos(lua_State* L) {
	ImVec2 ret = ImGui::GetCursorPos();
	return lua_push_ImVec2(L, ret);
}

//     IMGUI_API float         GetCursorPosX();                                                    
static int imgui_lua_GetCursorPosX(lua_State* L) {
	float ret = ImGui::GetCursorPosX();
	lua_pushnumber(L, ret);
	return 1;
}

//     IMGUI_API float         GetCursorPosY();                                                    
static int imgui_lua_GetCursorPosY(lua_State* L) {
	float ret = ImGui::GetCursorPosY();
	lua_pushnumber(L, ret);
	return 1;
}

//     IMGUI_API void          SetCursorPosX(float x);                                             
static int imgui_lua_SetCursorPosX(lua_State* L) {
	int arg = 1;
	float x = lua_tonumber(L, arg++);
	ImGui::SetCursorPosX(x);
	return 0;
}

//     IMGUI_API void          SetCursorPosY(float y);                                             
static int imgui_lua_SetCursorPosY(lua_State* L) {
	int arg = 1;
	float y = lua_tonumber(L, arg++);
	ImGui::SetCursorPosY(y);
	return 0;
}

//     IMGUI_API ImVec2        GetCursorStartPos();                                                
static int imgui_lua_GetCursorStartPos(lua_State* L) {
	ImVec2 ret = ImGui::GetCursorStartPos();
	return lua_push_ImVec2(L, ret);
}

//     IMGUI_API ImVec2        GetCursorScreenPos();                                               
static int imgui_lua_GetCursorScreenPos(lua_State* L) {
	ImVec2 ret = ImGui::GetCursorScreenPos();
	return lua_push_ImVec2(L, ret);
}

//     IMGUI_API void          AlignFirstTextHeightToWidgets();                                    
static int imgui_lua_AlignFirstTextHeightToWidgets(lua_State* L) {
	ImGui::AlignFirstTextHeightToWidgets();
	return 0;
}

//     IMGUI_API float         GetTextLineHeight();                                                
static int imgui_lua_GetTextLineHeight(lua_State* L) {
	float ret = ImGui::GetTextLineHeight();
	lua_pushnumber(L, ret);
	return 1;
}

//     IMGUI_API float         GetTextLineHeightWithSpacing();                                     
static int imgui_lua_GetTextLineHeightWithSpacing(lua_State* L) {
	float ret = ImGui::GetTextLineHeightWithSpacing();
	lua_pushnumber(L, ret);
	return 1;
}

//     IMGUI_API float         GetItemsLineHeightWithSpacing();                                    
static int imgui_lua_GetItemsLineHeightWithSpacing(lua_State* L) {
	float ret = ImGui::GetItemsLineHeightWithSpacing();
	lua_pushnumber(L, ret);
	return 1;
}

//     IMGUI_API void          Columns(int count = 1, const char* id = NULL, bool border = true);  
static int imgui_lua_Columns(lua_State* L) {
	int arg = 1;
	int count = luaL_optint(L, arg++, 1);
	const char* id = luaL_optstring(L, arg++, NULL);
	bool border = !!luaL_optint(L, arg++, true);
	ImGui::Columns(count, id, border);
	return 0;
}

//     IMGUI_API void          NextColumn();                                                       
static int imgui_lua_NextColumn(lua_State* L) {
	ImGui::NextColumn();
	return 0;
}

//     IMGUI_API int           GetColumnIndex();                                                   
static int imgui_lua_GetColumnIndex(lua_State* L) {
	int ret = ImGui::GetColumnIndex();
	lua_pushinteger(L, ret);
	return 1;
}

//     IMGUI_API float         GetColumnOffset(int column_index = -1);                             
static int imgui_lua_GetColumnOffset(lua_State* L) {
	int arg = 1;
	int column_index = luaL_optint(L, arg++, -1);
	float ret = ImGui::GetColumnOffset(column_index);
	lua_pushnumber(L, ret);
	return 1;
}

//     IMGUI_API void          SetColumnOffset(int column_index, float offset_x);                  
static int imgui_lua_SetColumnOffset(lua_State* L) {
	int arg = 1;
	int column_index = lua_tointeger(L, arg++);
	float offset_x = lua_tonumber(L, arg++);
	ImGui::SetColumnOffset(column_index, offset_x);
	return 0;
}

//     IMGUI_API float         GetColumnWidth(int column_index = -1);                              
static int imgui_lua_GetColumnWidth(lua_State* L) {
	int arg = 1;
	int column_index = luaL_optint(L, arg++, -1);
	float ret = ImGui::GetColumnWidth(column_index);
	lua_pushnumber(L, ret);
	return 1;
}

//     IMGUI_API int           GetColumnsCount();                                                  
static int imgui_lua_GetColumnsCount(lua_State* L) {
	int ret = ImGui::GetColumnsCount();
	lua_pushinteger(L, ret);
	return 1;
}

//     IMGUI_API void          PushID(int int_id);
//     IMGUI_API void          PushID(const char* str_id);                                         
//     IMGUI_API void          PushID(const char* str_id_begin, const char* str_id_end);
static int imgui_lua_PushID(lua_State* L) {
	int arg = 1;
	if (lua_gettop(L) == 2) {
		const char* str_id_begin = lua_tostring(L, arg++);
		const char* str_id_end = lua_tostring(L, arg++);
		ImGui::PushID(str_id_begin, str_id_end);
	}
	else if (lua_type(L, 1) == LUA_TNUMBER)
	{
		int int_id = lua_tointeger(L, arg++);
		ImGui::PushID(int_id);
	}
	else
	{
		const char* str_id = lua_tostring(L, arg++);
		ImGui::PushID(str_id);
	}
	return 0;
}

//     IMGUI_API void          PopID();
static int imgui_lua_PopID(lua_State* L) {
	ImGui::PopID();
	return 0;
}

//     IMGUI_API ImGuiID       GetID(const char* str_id);                                          
//     IMGUI_API ImGuiID       GetID(const char* str_id_begin, const char* str_id_end);
static int imgui_lua_GetID(lua_State* L) {
	int arg = 1;
	if (lua_gettop(L) == 2)
	{
		const char* str_id_begin = lua_tostring(L, arg++);
		const char* str_id_end = lua_tostring(L, arg++);
		ImGuiID ret = ImGui::GetID(str_id_begin, str_id_end);
		lua_pushinteger(L, ret);
	}
	else
	{
		const char* str_id = lua_tostring(L, arg++);
		ImGuiID ret = ImGui::GetID(str_id);
		lua_pushinteger(L, ret);
	}
	return 1;
}

//     IMGUI_API void          TextUnformatted(const char* text, const char* text_end = NULL);         
static int imgui_lua_TextUnformatted(lua_State* L) {
	int arg = 1;
	const char* text = lua_tostring(L, arg++);
	const char* text_end = luaL_optstring(L, arg++, NULL);
	ImGui::TextUnformatted(text, text_end);
	return 0;
}

//     IMGUI_API void          Bullet();                                                               
static int imgui_lua_Bullet(lua_State* L) {
	ImGui::Bullet();
	return 0;
}

//     IMGUI_API bool          SmallButton(const char* label);                                         
static int imgui_lua_SmallButton(lua_State* L) {
	int arg = 1;
	const char* label = lua_tostring(L, arg++);
	bool ret = ImGui::SmallButton(label);
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          Checkbox(const char* label, bool* v);
static int imgui_lua_Checkbox(lua_State* L) {
	int arg = 1;
	const char* label = lua_tostring(L, arg++);
	bool v_arg = lua_toboolean(L, arg++);
	bool* v = &v_arg;
	bool ret = ImGui::Checkbox(label, v);
	lua_pushboolean(L, ret);
	lua_pushboolean(L, *v);
	return 2;
}

//     IMGUI_API bool          CheckboxFlags(const char* label, unsigned int* flags, unsigned int flags_value);
static int imgui_lua_CheckboxFlags(lua_State* L) {
	int arg = 1;
	const char* label = lua_tostring(L, arg++);
	unsigned int flags_arg = lua_tointeger(L, arg++);
	unsigned int* flags = &flags_arg;
	unsigned int flags_value = lua_tointeger(L, arg++);
	bool ret = ImGui::CheckboxFlags(label, flags, flags_value);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, *flags);
	return 2;
}

//     IMGUI_API bool          RadioButton(const char* label, bool active);
//     IMGUI_API bool          RadioButton(const char* label, int* v, int v_button);
static int imgui_lua_RadioButton(lua_State* L) {
	int arg = 1;
	if (lua_gettop(L) == 3)
	{
		const char* label = lua_tostring(L, arg++);
		int v_arg = lua_tointeger(L, arg++);
		int* v = &v_arg;
		int v_button = lua_tointeger(L, arg++);
		bool ret = ImGui::RadioButton(label, v, v_button);
		lua_pushboolean(L, ret);
		lua_pushinteger(L, *v);
	}
	else
	{
		const char* label = lua_tostring(L, arg++);
		bool active = lua_toboolean(L, arg++);
		bool ret = ImGui::RadioButton(label, active);
		lua_pushboolean(L, ret);
	}
	return 1;
}

//     IMGUI_API bool          Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items = -1);      
static int imgui_lua_Combo(lua_State* L) {
	int arg = 1;
	const char* label = lua_tostring(L, arg++);
	int current_item_arg = lua_tointeger(L, arg++);
	int* current_item = &current_item_arg;
	const char* items_separated_by_zeros = lua_tostring(L, arg++);
	int height_in_items = luaL_optint(L, arg++, -1);
	bool ret = ImGui::Combo(label, current_item, items_separated_by_zeros, height_in_items);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, *current_item);
	return 2;
}

//     IMGUI_API void          ColorEditMode(ImGuiColorEditMode mode);                                 
static int imgui_lua_ColorEditMode(lua_State* L) {
	int arg = 1;
	ImGuiColorEditMode mode = lua_tointeger(L, arg++);
	ImGui::ColorEditMode(mode);
	return 0;
}

//     IMGUI_API bool          DragFloat(const char* label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* display_format = "%.3f", float power = 1.0f);     
static int imgui_lua_DragFloat(lua_State* L) {
	int arg = 1;
	const char* label = lua_tostring(L, arg++);
	float v_arg = lua_tonumber(L, arg++);
	float* v = &v_arg;
	float v_speed = luaL_optnumber(L, arg++, 1.0f);
	float v_min = luaL_optnumber(L, arg++, 0.0f);
	float v_max = luaL_optnumber(L, arg++, 0.0f);
	const char* display_format = luaL_optstring(L, arg++, "%.3f");
	float power = luaL_optnumber(L, arg++, 1.0f);
	bool ret = ImGui::DragFloat(label, v, v_speed, v_min, v_max, display_format, power);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, *v);
	return 2;
}

//     IMGUI_API bool          DragFloatRange2(const char* label, float* v_current_min, float* v_current_max, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* display_format = "%.3f", const char* display_format_max = NULL, float power = 1.0f);
static int imgui_lua_DragFloatRange2(lua_State* L) {
	int arg = 1;
	const char* label = lua_tostring(L, arg++);
	float v_current_min_arg = lua_tonumber(L, arg++);
	float* v_current_min = &v_current_min_arg;
	float v_current_max_arg = lua_tonumber(L, arg++);
	float* v_current_max = &v_current_max_arg;
	float v_speed = luaL_optnumber(L, arg++, 1.0f);
	float v_min = luaL_optnumber(L, arg++, 0.0f);
	float v_max = luaL_optnumber(L, arg++, 0.0f);
	const char* display_format = luaL_optstring(L, arg++, "%.3f");
	const char* display_format_max = luaL_optstring(L, arg++, NULL);
	float power = luaL_optnumber(L, arg++, 1.0f);
	bool ret = ImGui::DragFloatRange2(label, v_current_min, v_current_max, v_speed, v_min, v_max, display_format, display_format_max, power);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, *v_current_min);
	lua_pushnumber(L, *v_current_max);
	return 3;
}

//     IMGUI_API bool          DragInt(const char* label, int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* display_format = "%.0f");                                       
static int imgui_lua_DragInt(lua_State* L) {
	int arg = 1;
	const char* label = lua_tostring(L, arg++);
	int v_arg = lua_tointeger(L, arg++);
	int* v = &v_arg;
	float v_speed = luaL_optnumber(L, arg++, 1.0f);
	int v_min = luaL_optint(L, arg++, 0);
	int v_max = luaL_optint(L, arg++, 0);
	const char* display_format = luaL_optstring(L, arg++, "%.0f");
	bool ret = ImGui::DragInt(label, v, v_speed, v_min, v_max, display_format);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, *v);
	return 2;
}

//     IMGUI_API bool          DragIntRange2(const char* label, int* v_current_min, int* v_current_max, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* display_format = "%.0f", const char* display_format_max = NULL);
static int imgui_lua_DragIntRange2(lua_State* L) {
	int arg = 1;
	const char* label = lua_tostring(L, arg++);
	int v_current_min_arg = lua_tointeger(L, arg++);
	int* v_current_min = &v_current_min_arg;
	int v_current_max_arg = lua_tointeger(L, arg++);
	int* v_current_max = &v_current_max_arg;
	float v_speed = luaL_optnumber(L, arg++, 1.0f);
	int v_min = luaL_optint(L, arg++, 0);
	int v_max = luaL_optint(L, arg++, 0);
	const char* display_format = luaL_optstring(L, arg++, "%.0f");
	const char* display_format_max = luaL_optstring(L, arg++, NULL);
	bool ret = ImGui::DragIntRange2(label, v_current_min, v_current_max, v_speed, v_min, v_max, display_format, display_format_max);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, *v_current_min);
	lua_pushinteger(L, *v_current_max);
	return 3;
}

//     IMGUI_API bool          InputFloat(const char* label, float* v, float step = 0.0f, float step_fast = 0.0f, int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0);
static int imgui_lua_InputFloat(lua_State* L) {
	int arg = 1;
	const char* label = lua_tostring(L, arg++);
	float v_arg = lua_tonumber(L, arg++);
	float* v = &v_arg;
	float step = luaL_optnumber(L, arg++, 0.0f);
	float step_fast = luaL_optnumber(L, arg++, 0.0f);
	int decimal_precision = luaL_optint(L, arg++, -1);
	ImGuiInputTextFlags extra_flags = luaL_optint(L, arg++, 0);
	bool ret = ImGui::InputFloat(label, v, step, step_fast, decimal_precision, extra_flags);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, *v);
	return 2;
}

//     IMGUI_API bool          InputInt(const char* label, int* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags extra_flags = 0);
static int imgui_lua_InputInt(lua_State* L) {
	int arg = 1;
	const char* label = lua_tostring(L, arg++);
	int v_arg = lua_tointeger(L, arg++);
	int* v = &v_arg;
	int step = luaL_optint(L, arg++, 1);
	int step_fast = luaL_optint(L, arg++, 100);
	ImGuiInputTextFlags extra_flags = luaL_optint(L, arg++, 0);
	bool ret = ImGui::InputInt(label, v, step, step_fast, extra_flags);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, *v);
	return 2;
}

//     IMGUI_API bool          SliderFloat(const char* label, float* v, float v_min, float v_max, const char* display_format = "%.3f", float power = 1.0f);     
static int imgui_lua_SliderFloat(lua_State* L) {
	int arg = 1;
	const char* label = lua_tostring(L, arg++);
	float v_arg = lua_tonumber(L, arg++);
	float* v = &v_arg;
	float v_min = lua_tonumber(L, arg++);
	float v_max = lua_tonumber(L, arg++);
	const char* display_format = luaL_optstring(L, arg++, "%.3f");
	float power = luaL_optnumber(L, arg++, 1.0f);
	bool ret = ImGui::SliderFloat(label, v, v_min, v_max, display_format, power);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, *v);
	return 2;
}

//     IMGUI_API bool          SliderAngle(const char* label, float* v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f);
static int imgui_lua_SliderAngle(lua_State* L) {
	int arg = 1;
	const char* label = lua_tostring(L, arg++);
	float v_rad_arg = lua_tonumber(L, arg++);
	float* v_rad = &v_rad_arg;
	float v_degrees_min = luaL_optnumber(L, arg++, -360.0f);
	float v_degrees_max = luaL_optnumber(L, arg++, +360.0f);
	bool ret = ImGui::SliderAngle(label, v_rad, v_degrees_min, v_degrees_max);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, *v_rad);
	return 2;
}

//     IMGUI_API bool          SliderInt(const char* label, int* v, int v_min, int v_max, const char* display_format = "%.0f");
static int imgui_lua_SliderInt(lua_State* L) {
	int arg = 1;
	const char* label = lua_tostring(L, arg++);
	int v_arg = lua_tointeger(L, arg++);
	int* v = &v_arg;
	int v_min = lua_tointeger(L, arg++);
	int v_max = lua_tointeger(L, arg++);
	const char* display_format = luaL_optstring(L, arg++, "%.0f");
	bool ret = ImGui::SliderInt(label, v, v_min, v_max, display_format);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, *v);
	return 2;
}

//     IMGUI_API bool          TreeNode(const char* label);                                            
static int imgui_lua_TreeNode(lua_State* L) {
	int arg = 1;
	const char* label = lua_tostring(L, arg++);
	bool ret = ImGui::TreeNode(label);
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags = 0);
static int imgui_lua_TreeNodeEx(lua_State* L) {
	int arg = 1;
	const char* label = lua_tostring(L, arg++);
	ImGuiTreeNodeFlags flags = luaL_optint(L, arg++, 0);
	bool ret = ImGui::TreeNodeEx(label, flags);
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API void          TreePush(const char* str_id = NULL);                                    
static int imgui_lua_TreePush(lua_State* L) {
	int arg = 1;
	const char* str_id = luaL_optstring(L, arg++, NULL);
	ImGui::TreePush(str_id);
	return 0;
}

//     IMGUI_API void          TreePop();                                                              
static int imgui_lua_TreePop(lua_State* L) {
	ImGui::TreePop();
	return 0;
}

//     IMGUI_API void          TreeAdvanceToLabelPos();                                                
static int imgui_lua_TreeAdvanceToLabelPos(lua_State* L) {
	ImGui::TreeAdvanceToLabelPos();
	return 0;
}

//     IMGUI_API float         GetTreeNodeToLabelSpacing();                                            
static int imgui_lua_GetTreeNodeToLabelSpacing(lua_State* L) {
	float ret = ImGui::GetTreeNodeToLabelSpacing();
	lua_pushnumber(L, ret);
	return 1;
}

//     IMGUI_API void          SetNextTreeNodeOpen(bool is_open, ImGuiSetCond cond = 0);               
static int imgui_lua_SetNextTreeNodeOpen(lua_State* L) {
	int arg = 1;
	bool is_open = lua_toboolean(L, arg++);
	ImGuiSetCond cond = luaL_optint(L, arg++, 0);
	ImGui::SetNextTreeNodeOpen(is_open, cond);
	return 0;
}

//     IMGUI_API bool          CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags = 0);      
//     IMGUI_API bool          CollapsingHeader(const char* label, bool* p_open, ImGuiTreeNodeFlags flags = 0); 
static int imgui_lua_CollapsingHeader(lua_State* L) {
	int arg = 1;
	if (lua_gettop(L) == 3)
	{
		const char* label = lua_tostring(L, arg++);
		bool p_open_arg = lua_toboolean(L, arg++);
		bool* p_open = &p_open_arg;
		ImGuiTreeNodeFlags flags = luaL_optint(L, arg++, 0);
		bool ret = ImGui::CollapsingHeader(label, p_open, flags);
		lua_pushboolean(L, ret);
		lua_pushboolean(L, *p_open);
		return 2;
	}
	else
	{
		const char* label = lua_tostring(L, arg++);
		ImGuiTreeNodeFlags flags = luaL_optint(L, arg++, 0);
		bool ret = ImGui::CollapsingHeader(label, flags);
		lua_pushboolean(L, ret);
	}
	return 1;
}

//     IMGUI_API bool          ListBoxHeader(const char* label, int items_count, int height_in_items = -1); 
static int imgui_lua_ListBoxHeader(lua_State* L) {
	int arg = 1;
	const char* label = lua_tostring(L, arg++);
	int items_count = lua_tointeger(L, arg++);
	int height_in_items = luaL_optint(L, arg++, -1);
	bool ret = ImGui::ListBoxHeader(label, items_count, height_in_items);
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API void          ListBoxFooter();                                                    
static int imgui_lua_ListBoxFooter(lua_State* L) {
	ImGui::ListBoxFooter();
	return 0;
}

//     IMGUI_API void          Value(const char* prefix, bool b);
//     IMGUI_API void          Value(const char* prefix, int v);
//     IMGUI_API void          Value(const char* prefix, unsigned int v);
//     IMGUI_API void          Value(const char* prefix, float v, const char* float_format = NULL);
static int imgui_lua_Value(lua_State* L) {
	int arg = 1;
	if (lua_type(L, 2) == LUA_TBOOLEAN)
	{
		const char* prefix = lua_tostring(L, arg++);
		bool b = lua_toboolean(L, arg++);
		ImGui::Value(prefix, b);
	}
	else
	{
		const char* prefix = lua_tostring(L, arg++);
		int v = lua_tointeger(L, arg++);
		ImGui::Value(prefix, v);
	}

	return 0;
}

//     IMGUI_API void          ValueColor(const char* prefix, ImU32 v);
static int imgui_lua_ValueColor(lua_State* L) {
	int arg = 1;
	const char* prefix = lua_tostring(L, arg++);
	ImU32 v = lua_tointeger(L, arg++);
	ImGui::ValueColor(prefix, v);
	return 0;
}

//     IMGUI_API void          BeginTooltip();                                                     
static int imgui_lua_BeginTooltip(lua_State* L) {
	ImGui::BeginTooltip();
	return 0;
}

//     IMGUI_API void          EndTooltip();
static int imgui_lua_EndTooltip(lua_State* L) {
	ImGui::EndTooltip();
	return 0;
}

//     IMGUI_API bool          BeginMainMenuBar();                                                 
static int imgui_lua_BeginMainMenuBar(lua_State* L) {
	bool ret = ImGui::BeginMainMenuBar();
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API void          EndMainMenuBar();
static int imgui_lua_EndMainMenuBar(lua_State* L) {
	ImGui::EndMainMenuBar();
	return 0;
}

//     IMGUI_API bool          BeginMenuBar();                                                     
static int imgui_lua_BeginMenuBar(lua_State* L) {
	bool ret = ImGui::BeginMenuBar();
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API void          EndMenuBar();
static int imgui_lua_EndMenuBar(lua_State* L) {
	ImGui::EndMenuBar();
	return 0;
}

//     IMGUI_API bool          BeginMenu(const char* label, bool enabled = true);                  
static int imgui_lua_BeginMenu(lua_State* L) {
	int arg = 1;
	const char* label = lua_tostring(L, arg++);
	bool enabled = !!luaL_optint(L, arg++, true);
	bool ret = ImGui::BeginMenu(label, enabled);
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API void          EndMenu();
static int imgui_lua_EndMenu(lua_State* L) {
	ImGui::EndMenu();
	return 0;
}

//     IMGUI_API bool          MenuItem(const char* label, const char* shortcut = NULL, bool selected = false, bool enabled = true);  
//     IMGUI_API bool          MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled = true);              
static int imgui_lua_MenuItem(lua_State* L) {
	int arg = 1;
	const char* label = lua_tostring(L, arg++);
	const char* shortcut = luaL_optstring(L, arg++, NULL);
	bool selected = !!luaL_optint(L, arg++, false);
	bool enabled = !!luaL_optint(L, arg++, true);
	bool ret = ImGui::MenuItem(label, shortcut, selected, enabled);
	lua_pushboolean(L, ret);
	return 1;
}
// todo

// static int imgui_lua_MenuItem(lua_State* L) {
// 	int arg = 1;
// 	const char* label = lua_tostring(L, arg++);
// 	const char* shortcut = lua_tostring(L, arg++);
// 	bool p_selected_arg = lua_toboolean(L, arg++);
// 	bool* p_selected = &p_selected_arg;
// 	bool enabled = !!luaL_optint(L, arg++, true);
// 	bool ret = ImGui::MenuItem(label, shortcut, p_selected, enabled);
// 	lua_pushboolean(L, ret);
// 	lua_pushboolean(L, *p_selected);
// 	return 2;
// }

//     IMGUI_API void          OpenPopup(const char* str_id);                                      
static int imgui_lua_OpenPopup(lua_State* L) {
	int arg = 1;
	const char* str_id = lua_tostring(L, arg++);
	ImGui::OpenPopup(str_id);
	return 0;
}

//     IMGUI_API bool          BeginPopup(const char* str_id);                                     
static int imgui_lua_BeginPopup(lua_State* L) {
	int arg = 1;
	const char* str_id = lua_tostring(L, arg++);
	bool ret = ImGui::BeginPopup(str_id);
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          BeginPopupModal(const char* name, bool* p_open = NULL, ImGuiWindowFlags extra_flags = 0);               
static int imgui_lua_BeginPopupModal(lua_State* L) {
	int arg = 1;
	const char* name = lua_tostring(L, arg++);
	bool p_open_arg = lua_toboolean(L, arg++);
	bool* p_open = &p_open_arg;
	ImGuiWindowFlags extra_flags = luaL_optint(L, arg++, 0);
	bool ret = ImGui::BeginPopupModal(name, p_open, extra_flags);
	lua_pushboolean(L, ret);
	lua_pushboolean(L, *p_open);
	return 2;
}

//     IMGUI_API bool          BeginPopupContextItem(const char* str_id, int mouse_button = 1);                                        
static int imgui_lua_BeginPopupContextItem(lua_State* L) {
	int arg = 1;
	const char* str_id = lua_tostring(L, arg++);
	int mouse_button = luaL_optint(L, arg++, 1);
	bool ret = ImGui::BeginPopupContextItem(str_id, mouse_button);
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          BeginPopupContextWindow(bool also_over_items = true, const char* str_id = NULL, int mouse_button = 1);  
static int imgui_lua_BeginPopupContextWindow(lua_State* L) {
	int arg = 1;
	bool also_over_items = !!luaL_optint(L, arg++, true);
	const char* str_id = luaL_optstring(L, arg++, NULL);
	int mouse_button = luaL_optint(L, arg++, 1);
	bool ret = ImGui::BeginPopupContextWindow(also_over_items, str_id, mouse_button);
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          BeginPopupContextVoid(const char* str_id = NULL, int mouse_button = 1);                                 
static int imgui_lua_BeginPopupContextVoid(lua_State* L) {
	int arg = 1;
	const char* str_id = luaL_optstring(L, arg++, NULL);
	int mouse_button = luaL_optint(L, arg++, 1);
	bool ret = ImGui::BeginPopupContextVoid(str_id, mouse_button);
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API void          EndPopup();
static int imgui_lua_EndPopup(lua_State* L) {
	ImGui::EndPopup();
	return 0;
}

//     IMGUI_API void          CloseCurrentPopup();                                                
static int imgui_lua_CloseCurrentPopup(lua_State* L) {
	ImGui::CloseCurrentPopup();
	return 0;
}

//     IMGUI_API void          LogToTTY(int max_depth = -1);                                       
static int imgui_lua_LogToTTY(lua_State* L) {
	int arg = 1;
	int max_depth = luaL_optint(L, arg++, -1);
	ImGui::LogToTTY(max_depth);
	return 0;
}

//     IMGUI_API void          LogToFile(int max_depth = -1, const char* filename = NULL);         
static int imgui_lua_LogToFile(lua_State* L) {
	int arg = 1;
	int max_depth = luaL_optint(L, arg++, -1);
	const char* filename = luaL_optstring(L, arg++, NULL);
	ImGui::LogToFile(max_depth, filename);
	return 0;
}

//     IMGUI_API void          LogToClipboard(int max_depth = -1);                                 
static int imgui_lua_LogToClipboard(lua_State* L) {
	int arg = 1;
	int max_depth = luaL_optint(L, arg++, -1);
	ImGui::LogToClipboard(max_depth);
	return 0;
}

//     IMGUI_API void          LogFinish();                                                        
static int imgui_lua_LogFinish(lua_State* L) {
	ImGui::LogFinish();
	return 0;
}

//     IMGUI_API void          LogButtons();                                                       
static int imgui_lua_LogButtons(lua_State* L) {
	ImGui::LogButtons();
	return 0;
}

//     IMGUI_API void          PopClipRect();
static int imgui_lua_PopClipRect(lua_State* L) {
	ImGui::PopClipRect();
	return 0;
}

//     IMGUI_API bool          IsItemHovered();                                                    
static int imgui_lua_IsItemHovered(lua_State* L) {
	bool ret = ImGui::IsItemHovered();
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          IsItemHoveredRect();                                                
static int imgui_lua_IsItemHoveredRect(lua_State* L) {
	bool ret = ImGui::IsItemHoveredRect();
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          IsItemActive();                                                     
static int imgui_lua_IsItemActive(lua_State* L) {
	bool ret = ImGui::IsItemActive();
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          IsItemClicked(int mouse_button = 0);                                
static int imgui_lua_IsItemClicked(lua_State* L) {
	int arg = 1;
	int mouse_button = luaL_optint(L, arg++, 0);
	bool ret = ImGui::IsItemClicked(mouse_button);
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          IsItemVisible();                                                    
static int imgui_lua_IsItemVisible(lua_State* L) {
	bool ret = ImGui::IsItemVisible();
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          IsAnyItemHovered();
static int imgui_lua_IsAnyItemHovered(lua_State* L) {
	bool ret = ImGui::IsAnyItemHovered();
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          IsAnyItemActive();
static int imgui_lua_IsAnyItemActive(lua_State* L) {
	bool ret = ImGui::IsAnyItemActive();
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API ImVec2        GetItemRectMin();                                                   
static int imgui_lua_GetItemRectMin(lua_State* L) {
	ImVec2 ret = ImGui::GetItemRectMin();
	return lua_push_ImVec2(L, ret);
}

//     IMGUI_API ImVec2        GetItemRectMax();                                                   
static int imgui_lua_GetItemRectMax(lua_State* L) {
	ImVec2 ret = ImGui::GetItemRectMax();
	return lua_push_ImVec2(L, ret);
}

//     IMGUI_API ImVec2        GetItemRectSize();                                                  
static int imgui_lua_GetItemRectSize(lua_State* L) {
	ImVec2 ret = ImGui::GetItemRectSize();
	return lua_push_ImVec2(L, ret);
}

//     IMGUI_API void          SetItemAllowOverlap();                                              
static int imgui_lua_SetItemAllowOverlap(lua_State* L) {
	ImGui::SetItemAllowOverlap();
	return 0;
}

//     IMGUI_API bool          IsWindowHovered();                                                  
static int imgui_lua_IsWindowHovered(lua_State* L) {
	bool ret = ImGui::IsWindowHovered();
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          IsWindowFocused();                                                  
static int imgui_lua_IsWindowFocused(lua_State* L) {
	bool ret = ImGui::IsWindowFocused();
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          IsRootWindowFocused();                                              
static int imgui_lua_IsRootWindowFocused(lua_State* L) {
	bool ret = ImGui::IsRootWindowFocused();
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          IsRootWindowOrAnyChildFocused();                                    
static int imgui_lua_IsRootWindowOrAnyChildFocused(lua_State* L) {
	bool ret = ImGui::IsRootWindowOrAnyChildFocused();
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          IsRootWindowOrAnyChildHovered();                                    
static int imgui_lua_IsRootWindowOrAnyChildHovered(lua_State* L) {
	bool ret = ImGui::IsRootWindowOrAnyChildHovered();
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API float         GetTime();
static int imgui_lua_GetTime(lua_State* L) {
	float ret = ImGui::GetTime();
	lua_pushnumber(L, ret);
	return 1;
}

//     IMGUI_API int           GetFrameCount();
static int imgui_lua_GetFrameCount(lua_State* L) {
	int ret = ImGui::GetFrameCount();
	lua_pushinteger(L, ret);
	return 1;
}

//     IMGUI_API const char*   GetStyleColName(ImGuiCol idx);
static int imgui_lua_GetStyleColName(lua_State* L) {
	int arg = 1;
	ImGuiCol idx = lua_tointeger(L, arg++);
	const char* ret = ImGui::GetStyleColName(idx);
	lua_pushstring(L, ret);
	return 1;
}

//     IMGUI_API ImVec2        CalcTextSize(const char* text, const char* text_end = NULL, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);
static int imgui_lua_CalcTextSize(lua_State* L) {
	int arg = 1;
	const char* text = lua_tostring(L, arg++);
	const char* text_end = luaL_optstring(L, arg++, NULL);
	bool hide_text_after_double_hash = !!luaL_optint(L, arg++, false);
	float wrap_width = luaL_optnumber(L, arg++, -1.0f);
	ImVec2 ret = ImGui::CalcTextSize(text, text_end, hide_text_after_double_hash, wrap_width);
	return lua_push_ImVec2(L, ret);
}

//     IMGUI_API void          CalcListClipping(int items_count, float items_height, int* out_items_display_start, int* out_items_display_end);    
static int imgui_lua_CalcListClipping(lua_State* L) {
	int arg = 1;
	int items_count = lua_tointeger(L, arg++);
	float items_height = lua_tonumber(L, arg++);
	int out_items_display_start_arg = lua_tointeger(L, arg++);
	int* out_items_display_start = &out_items_display_start_arg;
	int out_items_display_end_arg = lua_tointeger(L, arg++);
	int* out_items_display_end = &out_items_display_end_arg;
	ImGui::CalcListClipping(items_count, items_height, out_items_display_start, out_items_display_end);
	lua_pushinteger(L, *out_items_display_start);
	lua_pushinteger(L, *out_items_display_end);
	return 2;
}

//     IMGUI_API void          EndChildFrame();
static int imgui_lua_EndChildFrame(lua_State* L) {
	ImGui::EndChildFrame();
	return 0;
}

//     IMGUI_API ImVec4        ColorConvertU32ToFloat4(ImU32 in);
static int imgui_lua_ColorConvertU32ToFloat4(lua_State* L) {
	int arg = 1;
	ImU32 in = lua_tointeger(L, arg++);
	ImVec4 ret = ImGui::ColorConvertU32ToFloat4(in);
	return lua_push_ImVec4(L, ret);
}

//     IMGUI_API void          ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v);
static int imgui_lua_ColorConvertRGBtoHSV(lua_State* L) {
	int arg = 1;
	float r = lua_tonumber(L, arg++);
	float g = lua_tonumber(L, arg++);
	float b = lua_tonumber(L, arg++);
	float out_h = lua_tonumber(L, arg++);
	float out_s = lua_tonumber(L, arg++);
	float out_v = lua_tonumber(L, arg++);
	ImGui::ColorConvertRGBtoHSV(r, g, b, out_h, out_s, out_v);
	lua_pushnumber(L, out_h);
	lua_pushnumber(L, out_s);
	lua_pushnumber(L, out_v);
	return 3;
}

//     IMGUI_API void          ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b);
static int imgui_lua_ColorConvertHSVtoRGB(lua_State* L) {
	int arg = 1;
	float h = lua_tonumber(L, arg++);
	float s = lua_tonumber(L, arg++);
	float v = lua_tonumber(L, arg++);
	float out_r = lua_tonumber(L, arg++);
	float out_g = lua_tonumber(L, arg++);
	float out_b = lua_tonumber(L, arg++);
	ImGui::ColorConvertHSVtoRGB(h, s, v, out_r, out_g, out_b);
	lua_pushnumber(L, out_r);
	lua_pushnumber(L, out_g);
	lua_pushnumber(L, out_b);
	return 3;
}

//     IMGUI_API int           GetKeyIndex(ImGuiKey key);                                          
static int imgui_lua_GetKeyIndex(lua_State* L) {
	int arg = 1;
	ImGuiKey key = lua_tointeger(L, arg++);
	int ret = ImGui::GetKeyIndex(key);
	lua_pushinteger(L, ret);
	return 1;
}

//     IMGUI_API bool          IsKeyDown(int key_index);                                           
static int imgui_lua_IsKeyDown(lua_State* L) {
	int arg = 1;
	int key_index = lua_tointeger(L, arg++);
	bool ret = ImGui::IsKeyDown(key_index);
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          IsKeyPressed(int key_index, bool repeat = true);                    
static int imgui_lua_IsKeyPressed(lua_State* L) {
	int arg = 1;
	int key_index = lua_tointeger(L, arg++);
	bool repeat = !!luaL_optint(L, arg++, true);
	bool ret = ImGui::IsKeyPressed(key_index, repeat);
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          IsKeyReleased(int key_index);                                       
static int imgui_lua_IsKeyReleased(lua_State* L) {
	int arg = 1;
	int key_index = lua_tointeger(L, arg++);
	bool ret = ImGui::IsKeyReleased(key_index);
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          IsMouseDown(int button);                                            
static int imgui_lua_IsMouseDown(lua_State* L) {
	int arg = 1;
	int button = lua_tointeger(L, arg++);
	bool ret = ImGui::IsMouseDown(button);
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          IsMouseClicked(int button, bool repeat = false);                    
static int imgui_lua_IsMouseClicked(lua_State* L) {
	int arg = 1;
	int button = lua_tointeger(L, arg++);
	bool repeat = !!luaL_optint(L, arg++, false);
	bool ret = ImGui::IsMouseClicked(button, repeat);
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          IsMouseDoubleClicked(int button);                                   
static int imgui_lua_IsMouseDoubleClicked(lua_State* L) {
	int arg = 1;
	int button = lua_tointeger(L, arg++);
	bool ret = ImGui::IsMouseDoubleClicked(button);
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          IsMouseReleased(int button);                                        
static int imgui_lua_IsMouseReleased(lua_State* L) {
	int arg = 1;
	int button = lua_tointeger(L, arg++);
	bool ret = ImGui::IsMouseReleased(button);
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          IsMouseHoveringWindow();                                            
static int imgui_lua_IsMouseHoveringWindow(lua_State* L) {
	bool ret = ImGui::IsMouseHoveringWindow();
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          IsMouseHoveringAnyWindow();                                         
static int imgui_lua_IsMouseHoveringAnyWindow(lua_State* L) {
	bool ret = ImGui::IsMouseHoveringAnyWindow();
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API bool          IsMouseDragging(int button = 0, float lock_threshold = -1.0f);      
static int imgui_lua_IsMouseDragging(lua_State* L) {
	int arg = 1;
	int button = luaL_optint(L, arg++, 0);
	float lock_threshold = luaL_optnumber(L, arg++, -1.0f);
	bool ret = ImGui::IsMouseDragging(button, lock_threshold);
	lua_pushboolean(L, ret);
	return 1;
}

//     IMGUI_API ImVec2        GetMousePos();                                                      
static int imgui_lua_GetMousePos(lua_State* L) {
	ImVec2 ret = ImGui::GetMousePos();
	return lua_push_ImVec2(L, ret);
}

//     IMGUI_API ImVec2        GetMousePosOnOpeningCurrentPopup();                                 
static int imgui_lua_GetMousePosOnOpeningCurrentPopup(lua_State* L) {
	ImVec2 ret = ImGui::GetMousePosOnOpeningCurrentPopup();
	return lua_push_ImVec2(L, ret);
}

//     IMGUI_API ImVec2        GetMouseDragDelta(int button = 0, float lock_threshold = -1.0f);    
static int imgui_lua_GetMouseDragDelta(lua_State* L) {
	int arg = 1;
	int button = luaL_optint(L, arg++, 0);
	float lock_threshold = luaL_optnumber(L, arg++, -1.0f);
	ImVec2 ret = ImGui::GetMouseDragDelta(button, lock_threshold);
	return lua_push_ImVec2(L, ret);
}

//     IMGUI_API void          ResetMouseDragDelta(int button = 0);                                
static int imgui_lua_ResetMouseDragDelta(lua_State* L) {
	int arg = 1;
	int button = luaL_optint(L, arg++, 0);
	ImGui::ResetMouseDragDelta(button);
	return 0;
}

//     IMGUI_API ImGuiMouseCursor GetMouseCursor();                                                
static int imgui_lua_GetMouseCursor(lua_State* L) {
	ImGuiMouseCursor ret = ImGui::GetMouseCursor();
	lua_pushinteger(L, ret);
	return 1;
}

//     IMGUI_API void          SetMouseCursor(ImGuiMouseCursor type);                              
static int imgui_lua_SetMouseCursor(lua_State* L) {
	int arg = 1;
	ImGuiMouseCursor type = lua_tointeger(L, arg++);
	ImGui::SetMouseCursor(type);
	return 0;
}

//     IMGUI_API void          CaptureKeyboardFromApp(bool capture = true);                        
static int imgui_lua_CaptureKeyboardFromApp(lua_State* L) {
	int arg = 1;
	bool capture = !!luaL_optint(L, arg++, true);
	ImGui::CaptureKeyboardFromApp(capture);
	return 0;
}

//     IMGUI_API void          CaptureMouseFromApp(bool capture = true);                           
static int imgui_lua_CaptureMouseFromApp(lua_State* L) {
	int arg = 1;
	bool capture = !!luaL_optint(L, arg++, true);
	ImGui::CaptureMouseFromApp(capture);
	return 0;
}

//     IMGUI_API const char*   GetClipboardText();
static int imgui_lua_GetClipboardText(lua_State* L) {
	const char* ret = ImGui::GetClipboardText();
	lua_pushstring(L, ret);
	return 1;
}

//     IMGUI_API void          SetClipboardText(const char* text);
static int imgui_lua_SetClipboardText(lua_State* L) {
	int arg = 1;
	const char* text = lua_tostring(L, arg++);
	ImGui::SetClipboardText(text);
	return 0;
}

//     IMGUI_API const char*   GetVersion();
static int imgui_lua_GetVersion(lua_State* L) {
	const char* ret = ImGui::GetVersion();
	lua_pushstring(L, ret);
	return 1;
}

//

//     IMGUI_API bool          Button(const char* label, const ImVec2& size = ImVec2(0,0));            
static int imgui_lua_Button(lua_State* L) {
	int arg = 1;
	const char* text = lua_tostring(L, arg++);
	bool ret;
	if (lua_type(L, 2) == LUA_TNUMBER) 
	{
		float w = lua_tonumber(L, arg++);
		float h = lua_tonumber(L, arg++);
		ret = ImGui::Button(text, ImVec2(w, h));
	}
	else
	{
		ret = ImGui::Button(text);
	}

	lua_pushboolean(L, ret);
	return 1;
}

#define InputInt(N) \
static int imgui_lua_InputInt##N(lua_State* L) \
{\
	static int s[N];\
	int arg = 1;\
	const char* name = lua_tostring(L, arg++);\
	for (int i = 0; i < N; i++) {\
		s[i] = lua_tointeger(L, arg++);\
	}\
	bool ret = ImGui::InputInt##N(name, &s[0]);\
	lua_pushboolean(L, ret); \
	for (int i = 0; i < N; i++) {\
		lua_pushinteger(L, s[i]);\
	}\
	return N + 1;\
}

#define InputFloat(N) \
static int imgui_lua_InputFloat##N(lua_State* L) \
{\
	static float s[N];\
	int arg = 1;\
	const char* name = lua_tostring(L, arg++);\
	for (int i = 0; i < N; i++) {\
		s[i] = lua_tonumber(L, arg++);\
	}\
	bool ret = ImGui::InputFloat##N(name, &s[0]);\
	lua_pushboolean(L, ret); \
	for (int i = 0; i < N; i++) {\
		lua_pushnumber(L, s[i]);\
	}\
	return N + 1;\
}
//     IMGUI_API bool          InputFloat2(const char* label, float v[2], int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0);
//     IMGUI_API bool          InputFloat3(const char* label, float v[3], int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0);
//     IMGUI_API bool          InputFloat4(const char* label, float v[4], int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0);
//     IMGUI_API bool          InputInt2(const char* label, int v[2], ImGuiInputTextFlags extra_flags = 0);
//     IMGUI_API bool          InputInt3(const char* label, int v[3], ImGuiInputTextFlags extra_flags = 0);
//     IMGUI_API bool          InputInt4(const char* label, int v[4], ImGuiInputTextFlags extra_flags = 0);
InputInt(2)
InputInt(3)
InputInt(4)
InputFloat(2)
InputFloat(3)
InputFloat(4)

#define ColorEdit(N) \
static int imgui_lua_ColorEdit##N(lua_State* L) \
{\
	static float s[N];\
	int arg = 1;\
	const char* name = lua_tostring(L, arg++);\
	for (int i = 0; i < N; i++) {\
		s[i] = lua_tonumber(L, arg++);\
	}\
	bool ret = ImGui::ColorEdit##N(name, &s[0]);\
	lua_pushboolean(L, ret); \
	for (int i = 0; i < N; i++) {\
		lua_pushnumber(L, s[i]);\
	}\
	return N + 1;\
}
//     IMGUI_API bool          ColorEdit3(const char* label, float col[3]);                            
//     IMGUI_API bool          ColorEdit4(const char* label, float col[4], bool show_alpha = true);   
ColorEdit(3)
ColorEdit(4)
//     IMGUI_API bool          InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiTextEditCallback callback = NULL, void* user_data = NULL);

static int imgui_lua_InputText(lua_State* L)
{
	static char buf[2048];

	int arg = 1;
	const char* label = lua_tostring(L, arg++);
	const char* text = lua_tostring(L, arg++);
	strcpy(buf, text);

	ImGui::InputText(label, buf, sizeof(buf), 0);

	lua_pushstring(L, buf);

	return 1;
}

static int addDrawFunction(lua_State* L)
{
	SharedPtr<LuaFunction> func(new LuaFunction(L, 1));

	ImGuiCC::get<LuaUI>()->addFunction(func);

	return 0;
}

static const luaL_Reg lua_imgui_auto_funcs[] = {

	{ "addDraw", addDrawFunction },
	{ "InputText", imgui_lua_InputText },

	{ "InputInt2", imgui_lua_InputInt2 },
	{ "InputInt3", imgui_lua_InputInt3 },
	{ "InputInt4", imgui_lua_InputInt4 },
	{ "InputFloat2", imgui_lua_InputFloat2 },
	{ "InputFloat3", imgui_lua_InputFloat3 },
	{ "InputFloat4", imgui_lua_InputFloat4 },
	{ "ColorEdit3", imgui_lua_ColorEdit3 },
	{ "ColorEdit4", imgui_lua_ColorEdit4 },
	
	{ "Button", imgui_lua_Button },

	{ "NewFrame", imgui_lua_NewFrame },
	 { "Render", imgui_lua_Render },
	 { "ShowUserGuide", imgui_lua_ShowUserGuide },
	 { "ShowTestWindow", imgui_lua_ShowTestWindow },
	 { "ShowMetricsWindow", imgui_lua_ShowMetricsWindow },
	 { "Begin", imgui_lua_Begin },
	 { "End", imgui_lua_End },
	 { "EndChild", imgui_lua_EndChild },
	 { "GetContentRegionMax", imgui_lua_GetContentRegionMax },
	 { "GetContentRegionAvail", imgui_lua_GetContentRegionAvail },
	 { "GetContentRegionAvailWidth", imgui_lua_GetContentRegionAvailWidth },
	 { "GetWindowContentRegionMin", imgui_lua_GetWindowContentRegionMin },
	 { "GetWindowContentRegionMax", imgui_lua_GetWindowContentRegionMax },
	 { "GetWindowContentRegionWidth", imgui_lua_GetWindowContentRegionWidth },
	 { "GetWindowPos", imgui_lua_GetWindowPos },
	 { "GetWindowSize", imgui_lua_GetWindowSize },
	 { "GetWindowWidth", imgui_lua_GetWindowWidth },
	 { "GetWindowHeight", imgui_lua_GetWindowHeight },
	 { "IsWindowCollapsed", imgui_lua_IsWindowCollapsed },
	 { "SetWindowFontScale", imgui_lua_SetWindowFontScale },
	 { "SetNextWindowPosCenter", imgui_lua_SetNextWindowPosCenter },
	 { "SetNextWindowContentWidth", imgui_lua_SetNextWindowContentWidth },
	 { "SetNextWindowCollapsed", imgui_lua_SetNextWindowCollapsed },
	 { "SetNextWindowFocus", imgui_lua_SetNextWindowFocus },
	 { "SetWindowCollapsed", imgui_lua_SetWindowCollapsed },
	 { "SetWindowFocus", imgui_lua_SetWindowFocus },
	 { "SetWindowCollapsed", imgui_lua_SetWindowCollapsed },
	 { "SetWindowFocus", imgui_lua_SetWindowFocus },
	 { "GetScrollX", imgui_lua_GetScrollX },
	 { "GetScrollY", imgui_lua_GetScrollY },
	 { "GetScrollMaxX", imgui_lua_GetScrollMaxX },
	 { "GetScrollMaxY", imgui_lua_GetScrollMaxY },
	 { "SetScrollX", imgui_lua_SetScrollX },
	 { "SetScrollY", imgui_lua_SetScrollY },
	 { "SetScrollHere", imgui_lua_SetScrollHere },
	 { "SetScrollFromPosY", imgui_lua_SetScrollFromPosY },
	 { "SetKeyboardFocusHere", imgui_lua_SetKeyboardFocusHere },
	 { "PopFont", imgui_lua_PopFont },
	 { "PopStyleColor", imgui_lua_PopStyleColor },
	 { "PushStyleVar", imgui_lua_PushStyleVar },
	 { "PopStyleVar", imgui_lua_PopStyleVar },
	 { "GetFontSize", imgui_lua_GetFontSize },
	 { "GetFontTexUvWhitePixel", imgui_lua_GetFontTexUvWhitePixel },
	 { "GetColorU32", imgui_lua_GetColorU32 },
	 { "PushItemWidth", imgui_lua_PushItemWidth },
	 { "PopItemWidth", imgui_lua_PopItemWidth },
	 { "CalcItemWidth", imgui_lua_CalcItemWidth },
	 { "PushTextWrapPos", imgui_lua_PushTextWrapPos },
	 { "PopTextWrapPos", imgui_lua_PopTextWrapPos },
	 { "PushAllowKeyboardFocus", imgui_lua_PushAllowKeyboardFocus },
	 { "PopAllowKeyboardFocus", imgui_lua_PopAllowKeyboardFocus },
	 { "PushButtonRepeat", imgui_lua_PushButtonRepeat },
	 { "PopButtonRepeat", imgui_lua_PopButtonRepeat },
	 { "Separator", imgui_lua_Separator },
	 { "SameLine", imgui_lua_SameLine },
	 { "NewLine", imgui_lua_NewLine },
	 { "Spacing", imgui_lua_Spacing },
	 { "Indent", imgui_lua_Indent },
	 { "Unindent", imgui_lua_Unindent },
	 { "BeginGroup", imgui_lua_BeginGroup },
	 { "EndGroup", imgui_lua_EndGroup },
	 { "GetCursorPos", imgui_lua_GetCursorPos },
	 { "GetCursorPosX", imgui_lua_GetCursorPosX },
	 { "GetCursorPosY", imgui_lua_GetCursorPosY },
	 { "SetCursorPosX", imgui_lua_SetCursorPosX },
	 { "SetCursorPosY", imgui_lua_SetCursorPosY },
	 { "GetCursorStartPos", imgui_lua_GetCursorStartPos },
	 { "GetCursorScreenPos", imgui_lua_GetCursorScreenPos },
	 { "AlignFirstTextHeightToWidgets", imgui_lua_AlignFirstTextHeightToWidgets },
	 { "GetTextLineHeight", imgui_lua_GetTextLineHeight },
	 { "GetTextLineHeightWithSpacing", imgui_lua_GetTextLineHeightWithSpacing },
	 { "GetItemsLineHeightWithSpacing", imgui_lua_GetItemsLineHeightWithSpacing },
	 { "Columns", imgui_lua_Columns },
	 { "NextColumn", imgui_lua_NextColumn },
	 { "GetColumnIndex", imgui_lua_GetColumnIndex },
	 { "GetColumnOffset", imgui_lua_GetColumnOffset },
	 { "SetColumnOffset", imgui_lua_SetColumnOffset },
	 { "GetColumnWidth", imgui_lua_GetColumnWidth },
	 { "GetColumnsCount", imgui_lua_GetColumnsCount },
	 { "PushID", imgui_lua_PushID },
	 { "PopID", imgui_lua_PopID },
	 { "TextUnformatted", imgui_lua_TextUnformatted },
	 { "Bullet", imgui_lua_Bullet },
	 { "SmallButton", imgui_lua_SmallButton },
	 { "Checkbox", imgui_lua_Checkbox },
	 { "CheckboxFlags", imgui_lua_CheckboxFlags },
	 { "RadioButton", imgui_lua_RadioButton },
	 { "Combo", imgui_lua_Combo },
	 { "ColorEditMode", imgui_lua_ColorEditMode },
	 { "DragFloat", imgui_lua_DragFloat },
	 { "DragFloatRange2", imgui_lua_DragFloatRange2 },
	 { "DragInt", imgui_lua_DragInt },
	 { "DragIntRange2", imgui_lua_DragIntRange2 },
	 { "InputFloat", imgui_lua_InputFloat },
	 { "InputInt", imgui_lua_InputInt },
	 { "SliderFloat", imgui_lua_SliderFloat },
	 { "SliderAngle", imgui_lua_SliderAngle },
	 { "SliderInt", imgui_lua_SliderInt },
	 { "TreeNode", imgui_lua_TreeNode },
	 { "TreeNodeEx", imgui_lua_TreeNodeEx },
	 { "TreePush", imgui_lua_TreePush },
	 { "TreePop", imgui_lua_TreePop },
	 { "TreeAdvanceToLabelPos", imgui_lua_TreeAdvanceToLabelPos },
	 { "GetTreeNodeToLabelSpacing", imgui_lua_GetTreeNodeToLabelSpacing },
	 { "SetNextTreeNodeOpen", imgui_lua_SetNextTreeNodeOpen },
	 { "CollapsingHeader", imgui_lua_CollapsingHeader },
	 { "ListBoxHeader", imgui_lua_ListBoxHeader },
	 { "ListBoxFooter", imgui_lua_ListBoxFooter },
	 { "Value", imgui_lua_Value },
	 { "ValueColor", imgui_lua_ValueColor },
	 { "BeginTooltip", imgui_lua_BeginTooltip },
	 { "EndTooltip", imgui_lua_EndTooltip },
	 { "BeginMainMenuBar", imgui_lua_BeginMainMenuBar },
	 { "EndMainMenuBar", imgui_lua_EndMainMenuBar },
	 { "BeginMenuBar", imgui_lua_BeginMenuBar },
	 { "EndMenuBar", imgui_lua_EndMenuBar },
	 { "BeginMenu", imgui_lua_BeginMenu },
	 { "EndMenu", imgui_lua_EndMenu },
	 { "MenuItem", imgui_lua_MenuItem },
	 { "OpenPopup", imgui_lua_OpenPopup },
	 { "BeginPopup", imgui_lua_BeginPopup },
	 { "BeginPopupModal", imgui_lua_BeginPopupModal },
	 { "BeginPopupContextItem", imgui_lua_BeginPopupContextItem },
	 { "BeginPopupContextWindow", imgui_lua_BeginPopupContextWindow },
	 { "BeginPopupContextVoid", imgui_lua_BeginPopupContextVoid },
	 { "EndPopup", imgui_lua_EndPopup },
	 { "CloseCurrentPopup", imgui_lua_CloseCurrentPopup },
	 { "LogToTTY", imgui_lua_LogToTTY },
	 { "LogToFile", imgui_lua_LogToFile },
	 { "LogToClipboard", imgui_lua_LogToClipboard },
	 { "LogFinish", imgui_lua_LogFinish },
	 { "LogButtons", imgui_lua_LogButtons },
	 { "PopClipRect", imgui_lua_PopClipRect },
	 { "IsItemHovered", imgui_lua_IsItemHovered },
	 { "IsItemHoveredRect", imgui_lua_IsItemHoveredRect },
	 { "IsItemActive", imgui_lua_IsItemActive },
	 { "IsItemClicked", imgui_lua_IsItemClicked },
	 { "IsItemVisible", imgui_lua_IsItemVisible },
	 { "IsAnyItemHovered", imgui_lua_IsAnyItemHovered },
	 { "IsAnyItemActive", imgui_lua_IsAnyItemActive },
	 { "GetItemRectMin", imgui_lua_GetItemRectMin },
	 { "GetItemRectMax", imgui_lua_GetItemRectMax },
	 { "GetItemRectSize", imgui_lua_GetItemRectSize },
	 { "SetItemAllowOverlap", imgui_lua_SetItemAllowOverlap },
	 { "IsWindowHovered", imgui_lua_IsWindowHovered },
	 { "IsWindowFocused", imgui_lua_IsWindowFocused },
	 { "IsRootWindowFocused", imgui_lua_IsRootWindowFocused },
	 { "IsRootWindowOrAnyChildFocused", imgui_lua_IsRootWindowOrAnyChildFocused },
	 { "IsRootWindowOrAnyChildHovered", imgui_lua_IsRootWindowOrAnyChildHovered },
	 { "GetTime", imgui_lua_GetTime },
	 { "GetFrameCount", imgui_lua_GetFrameCount },
	 { "GetStyleColName", imgui_lua_GetStyleColName },
	 { "CalcTextSize", imgui_lua_CalcTextSize },
	 { "CalcListClipping", imgui_lua_CalcListClipping },
	 { "EndChildFrame", imgui_lua_EndChildFrame },
	 { "ColorConvertU32ToFloat4", imgui_lua_ColorConvertU32ToFloat4 },
	 { "ColorConvertRGBtoHSV", imgui_lua_ColorConvertRGBtoHSV },
	 { "ColorConvertHSVtoRGB", imgui_lua_ColorConvertHSVtoRGB },
	 { "GetKeyIndex", imgui_lua_GetKeyIndex },
	 { "IsKeyDown", imgui_lua_IsKeyDown },
	 { "IsKeyPressed", imgui_lua_IsKeyPressed },
	 { "IsKeyReleased", imgui_lua_IsKeyReleased },
	 { "IsMouseDown", imgui_lua_IsMouseDown },
	 { "IsMouseClicked", imgui_lua_IsMouseClicked },
	 { "IsMouseDoubleClicked", imgui_lua_IsMouseDoubleClicked },
	 { "IsMouseReleased", imgui_lua_IsMouseReleased },
	 { "IsMouseHoveringWindow", imgui_lua_IsMouseHoveringWindow },
	 { "IsMouseHoveringAnyWindow", imgui_lua_IsMouseHoveringAnyWindow },
	 { "IsMouseDragging", imgui_lua_IsMouseDragging },
	 { "GetMousePos", imgui_lua_GetMousePos },
	 { "GetMousePosOnOpeningCurrentPopup", imgui_lua_GetMousePosOnOpeningCurrentPopup },
	 { "GetMouseDragDelta", imgui_lua_GetMouseDragDelta },
	 { "ResetMouseDragDelta", imgui_lua_ResetMouseDragDelta },
	 { "GetMouseCursor", imgui_lua_GetMouseCursor },
	 { "SetMouseCursor", imgui_lua_SetMouseCursor },
	 { "CaptureKeyboardFromApp", imgui_lua_CaptureKeyboardFromApp },
	 { "CaptureMouseFromApp", imgui_lua_CaptureMouseFromApp },
	 { "GetClipboardText", imgui_lua_GetClipboardText },
	 { "SetClipboardText", imgui_lua_SetClipboardText },
	 { "GetVersion", imgui_lua_GetVersion },
    {NULL, NULL}
};

#if LUA_VERSION_NUM < 502
#  define luaL_newlib(L,l) (lua_newtable(L), luaL_register(L,NULL,l))
#endif

TOLUA_API
int luaopen_imgui(lua_State *L) {
	luaL_newlib(L, lua_imgui_auto_funcs);
	lua_pushvalue(L, -1);
	lua_setglobal(L, "imgui");
	return 1;
}

//     IMGUI_API void          PushID(const void* ptr_id);
//     IMGUI_API void          SetStateStorage(ImGuiStorage* tree);                                
//     IMGUI_API ImGuiStorage* GetStateStorage();
//     IMGUI_API void          TreePush(const void* ptr_id = NULL);                                    
//     IMGUI_API ImGuiID       GetID(const void* ptr_id);
//     IMGUI_API void          PushFont(ImFont* font);                                             
//     IMGUI_API ImFont*       GetFont();    
//     IMGUI_API void          ShowStyleEditor(ImGuiStyle* ref = NULL);    

//     IMGUI_API ImGuiIO&      GetIO();
//     IMGUI_API ImGuiStyle&   GetStyle();
//     IMGUI_API ImDrawData*   GetDrawData();                              
//     IMGUI_API void          Shutdown();
//     IMGUI_API bool          Begin(const char* name, bool* p_open, const ImVec2& size_on_first_use, float bg_alpha = -1.0f, ImGuiWindowFlags flags = 0); 
//     IMGUI_API bool          BeginChild(const char* str_id, const ImVec2& size = ImVec2(0,0), bool border = false, ImGuiWindowFlags extra_flags = 0);    
//     IMGUI_API bool          BeginChild(ImGuiID id, const ImVec2& size = ImVec2(0,0), bool border = false, ImGuiWindowFlags extra_flags = 0);            
//     IMGUI_API ImDrawList*   GetWindowDrawList();                                                
//     IMGUI_API void          SetNextWindowPos(const ImVec2& pos, ImGuiSetCond cond = 0);         
//     IMGUI_API void          SetNextWindowSize(const ImVec2& size, ImGuiSetCond cond = 0);       
//     IMGUI_API void          SetNextWindowSizeConstraints(const ImVec2& size_min, const ImVec2& size_max, ImGuiSizeConstraintCallback custom_callback = NULL, void* custom_callback_data = NULL); 
//     IMGUI_API void          SetNextWindowContentSize(const ImVec2& size);                       
//     IMGUI_API void          SetWindowPos(const ImVec2& pos, ImGuiSetCond cond = 0);             
//     IMGUI_API void          SetWindowSize(const ImVec2& size, ImGuiSetCond cond = 0);           
//     IMGUI_API void          SetWindowPos(const char* name, const ImVec2& pos, ImGuiSetCond cond = 0);      
//     IMGUI_API void          SetWindowSize(const char* name, const ImVec2& size, ImGuiSetCond cond = 0);    
//     IMGUI_API void          PushStyleColor(ImGuiCol idx, const ImVec4& col);
//     IMGUI_API void          PushStyleVar(ImGuiStyleVar idx, const ImVec2& val);
//     IMGUI_API ImU32         GetColorU32(const ImVec4& col);                                     
//     IMGUI_API void          Dummy(const ImVec2& size);                                          
//     IMGUI_API void          SetCursorPos(const ImVec2& local_pos);                              
//     IMGUI_API void          SetCursorScreenPos(const ImVec2& pos);                              
//     IMGUI_API void          Text(const char* fmt, ...) IM_PRINTFARGS(1);
//     IMGUI_API void          TextV(const char* fmt, va_list args);
//     IMGUI_API void          TextColored(const ImVec4& col, const char* fmt, ...) IM_PRINTFARGS(2);  
//     IMGUI_API void          TextColoredV(const ImVec4& col, const char* fmt, va_list args);
//     IMGUI_API void          TextDisabled(const char* fmt, ...) IM_PRINTFARGS(1);                    
//     IMGUI_API void          TextDisabledV(const char* fmt, va_list args);
//     IMGUI_API void          TextWrapped(const char* fmt, ...) IM_PRINTFARGS(1);                     
//     IMGUI_API void          TextWrappedV(const char* fmt, va_list args);
//     IMGUI_API void          LabelText(const char* label, const char* fmt, ...) IM_PRINTFARGS(2);    
//     IMGUI_API void          LabelTextV(const char* label, const char* fmt, va_list args);
//     IMGUI_API void          BulletText(const char* fmt, ...) IM_PRINTFARGS(1);                      
//     IMGUI_API void          BulletTextV(const char* fmt, va_list args);
//     IMGUI_API bool          InvisibleButton(const char* str_id, const ImVec2& size);
//     IMGUI_API void          Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0,0), const ImVec2& uv1 = ImVec2(1,1), const ImVec4& tint_col = ImVec4(1,1,1,1), const ImVec4& border_col = ImVec4(0,0,0,0));
//     IMGUI_API bool          ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0,0),  const ImVec2& uv1 = ImVec2(1,1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0,0,0,0), const ImVec4& tint_col = ImVec4(1,1,1,1));    
//     IMGUI_API bool          Combo(const char* label, int* current_item, const char** items, int items_count, int height_in_items = -1);
//     IMGUI_API bool          Combo(const char* label, int* current_item, bool (*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items = -1);
//     IMGUI_API bool          ColorButton(const ImVec4& col, bool small_height = false, bool outline_border = true);
//     IMGUI_API bool          ColorEdit3(const char* label, float col[3]);                            
//     IMGUI_API bool          ColorEdit4(const char* label, float col[4], bool show_alpha = true);    
//     IMGUI_API void          PlotLines(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0,0), int stride = sizeof(float));
//     IMGUI_API void          PlotLines(const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0,0));
//     IMGUI_API void          PlotHistogram(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0,0), int stride = sizeof(float));
//     IMGUI_API void          PlotHistogram(const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0,0));
//     IMGUI_API void          ProgressBar(float fraction, const ImVec2& size_arg = ImVec2(-1,0), const char* overlay = NULL);
//     IMGUI_API bool          DragFloat2(const char* label, float v[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* display_format = "%.3f", float power = 1.0f);
//     IMGUI_API bool          DragFloat3(const char* label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* display_format = "%.3f", float power = 1.0f);
//     IMGUI_API bool          DragFloat4(const char* label, float v[4], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* display_format = "%.3f", float power = 1.0f);
//     IMGUI_API bool          DragInt2(const char* label, int v[2], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* display_format = "%.0f");
//     IMGUI_API bool          DragInt3(const char* label, int v[3], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* display_format = "%.0f");
//     IMGUI_API bool          DragInt4(const char* label, int v[4], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* display_format = "%.0f");
//     IMGUI_API bool          InputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2& size = ImVec2(0,0), ImGuiInputTextFlags flags = 0, ImGuiTextEditCallback callback = NULL, void* user_data = NULL);
//     IMGUI_API bool          SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* display_format = "%.3f", float power = 1.0f);
//     IMGUI_API bool          SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* display_format = "%.3f", float power = 1.0f);
//     IMGUI_API bool          SliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* display_format = "%.3f", float power = 1.0f);
//     IMGUI_API bool          SliderInt2(const char* label, int v[2], int v_min, int v_max, const char* display_format = "%.0f");
//     IMGUI_API bool          SliderInt3(const char* label, int v[3], int v_min, int v_max, const char* display_format = "%.0f");
//     IMGUI_API bool          SliderInt4(const char* label, int v[4], int v_min, int v_max, const char* display_format = "%.0f");
//     IMGUI_API bool          VSliderFloat(const char* label, const ImVec2& size, float* v, float v_min, float v_max, const char* display_format = "%.3f", float power = 1.0f);
//     IMGUI_API bool          VSliderInt(const char* label, const ImVec2& size, int* v, int v_min, int v_max, const char* display_format = "%.0f");
//     IMGUI_API bool          TreeNode(const char* str_id, const char* fmt, ...) IM_PRINTFARGS(2);    
//     IMGUI_API bool          TreeNode(const void* ptr_id, const char* fmt, ...) IM_PRINTFARGS(2);    
//     IMGUI_API bool          TreeNodeV(const char* str_id, const char* fmt, va_list args);           
//     IMGUI_API bool          TreeNodeV(const void* ptr_id, const char* fmt, va_list args);           
//     IMGUI_API bool          TreeNodeEx(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_PRINTFARGS(3);
//     IMGUI_API bool          TreeNodeEx(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_PRINTFARGS(3);
//     IMGUI_API bool          TreeNodeExV(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args);
//     IMGUI_API bool          TreeNodeExV(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args);
//     IMGUI_API bool          Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0,0));  
//     IMGUI_API bool          Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0,0));
//     IMGUI_API bool          ListBox(const char* label, int* current_item, const char** items, int items_count, int height_in_items = -1);
//     IMGUI_API bool          ListBox(const char* label, int* current_item, bool (*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items = -1);
//     IMGUI_API bool          ListBoxHeader(const char* label, const ImVec2& size = ImVec2(0,0)); 
//     IMGUI_API void          ValueColor(const char* prefix, const ImVec4& v);
//     IMGUI_API void          SetTooltip(const char* fmt, ...) IM_PRINTFARGS(1);                  
//     IMGUI_API void          SetTooltipV(const char* fmt, va_list args);
//     IMGUI_API void          LogText(const char* fmt, ...) IM_PRINTFARGS(1);                     
//     IMGUI_API void          PushClipRect(const ImVec2& clip_rect_min, const ImVec2& clip_rect_max, bool intersect_with_current_clip_rect);
//     IMGUI_API bool          IsRectVisible(const ImVec2& size);                                  
//     IMGUI_API bool          IsRectVisible(const ImVec2& rect_min, const ImVec2& rect_max);      
//     IMGUI_API bool          IsPosHoveringAnyWindow(const ImVec2& pos);                          
//     IMGUI_API ImVec2        CalcItemRectClosestPoint(const ImVec2& pos, bool on_edge = false, float outward = +0.0f);   
//     IMGUI_API bool          BeginChildFrame(ImGuiID id, const ImVec2& size, ImGuiWindowFlags extra_flags = 0);	
//     IMGUI_API ImU32         ColorConvertFloat4ToU32(const ImVec4& in);
//     IMGUI_API bool          IsMouseHoveringRect(const ImVec2& r_min, const ImVec2& r_max, bool clip = true);  
//     IMGUI_API void*         MemAlloc(size_t sz);
//     IMGUI_API void          MemFree(void* ptr);
//     IMGUI_API ImGuiContext* CreateContext(void* (*malloc_fn)(size_t) = NULL, void (*free_fn)(void*) = NULL);
//     IMGUI_API void          DestroyContext(ImGuiContext* ctx);
//     IMGUI_API ImGuiContext* GetCurrentContext();
//     IMGUI_API void          SetCurrentContext(ImGuiContext* ctx);
