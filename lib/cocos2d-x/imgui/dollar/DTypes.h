

#define NS_DOLLAR_START namespace cocos2d { namespace dollar {
#define NS_DOLLAR_END }}

#define D_VIRTUAL_GET_SET_CONST_REF(Type, VarName, FuncName) \
		protected:\
		Type VarName; \
		public:\
		virtual void set##FuncName(const Type& v) { VarName = v; } \
		virtual const Type& get##FuncName() { return VarName; }

class Vec2 {

};

class Rect {

};