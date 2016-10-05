
#include "native/CCAlertViewDelegate.h"

#if CC_LUA_ENGINE_ENABLED > 0
#include "CCLuaEngine.h"
#endif

#include <string>
#include <string>


USING_NS_CC;

class CCNativeWin32
{
//    UIActivityIndicatorView *activityIndicatorView_;
    //NSAlert *alertView_;
public:
	static CCNativeWin32* sharedInstance(void);

	void showActivityIndicator(void);
	void hideActivityIndicator(void);

	void createAlertView(const char* title, const char *message, const char* cancelButtonTitle);
	int addAlertButton(const char* buttonTitle);
	void showAlertViewWithDelegate(CCAlertViewDelegate *delegate);
	void removeAlertView(void);
	void cancelAlertView(void);

#if CC_LUA_ENGINE_ENABLED > 0
	void showAlertViewWithLuaListener(LUA_FUNCTION listener);
	void removeAlertViewLuaListener(void);
#endif

	const std::string getInputText(const char* title, const char* message, const char* defaultValue);

	const std::string getUDID();
private:
	static CCNativeWin32* s_sharedInstance;

	CCNativeWin32(void);

	std::string m_alertViewTitle;
	std::string m_alertViewMessage;

	std::string m_macAddress;
};
