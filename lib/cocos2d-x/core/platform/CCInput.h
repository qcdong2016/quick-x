
#pragma once

#include "engine/SubSystem.h"



NS_CC_BEGIN;


class Input : public SubSystem
{
	CCOBJECT(Input, SubSystem)
public:

	void onKeypadBack();
	void onKeypadMenu();

};

NS_CC_END;