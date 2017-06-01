/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2010      Stuart Carnie

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef __SUPPORT_CCPROFILING_H__
#define __SUPPORT_CCPROFILING_H__

#include "platform/CCTimer.h"
#include "CCPlatformMacros.h"
#include "base/RefCounted.h"
#include "base/Ptr.h"
#include <string>
#include <vector>

NS_CC_BEGIN

class ProfilingBlock : public RefCounted
{
public:
	ProfilingBlock(const char* name_, ProfilingBlock* parent_)
		: count(0)
		, time(0)
		, minTime(100000000)
		, maxTime(0)
		, name(name_)
		, parent(parent_)
	{}

	void start()
	{
		timer.reset();
		count++;
	}
	void stop()
	{
		long long t = timer.elapsed();
		if (t > maxTime)
			maxTime = t;
		if (t < minTime)
			minTime = t;
		time += t;
	}

	ProfilingBlock* getChild(const char* name)
	{
		for (auto child : children)
		{
			if (child->name.compare(name) == 0)
				return child;
		}
		SharedPtr<ProfilingBlock> child(new ProfilingBlock(name, this));
		children.push_back(child);
		return child;
	}

	const std::string& toString();

	int               count;
	long long         time;
	long long		  maxTime;
	long long         minTime;
	std::string name;
	TimerHiRes timer;
	ProfilingBlock* parent;
	std::vector<SharedPtr<ProfilingBlock> > children;
};

class CC_DLL CCProfiler : public RefCounted
{
public:
	CCProfiler()
	{
		_root = new ProfilingBlock("Profiling", nullptr);
		_current = _root;
	}

	void displayBlocks();

	void start(const char* name)
	{
		_current = _current->getChild(name);
		_current->start();
	}
	void stop()
	{
		_current->stop();
		if (_current->parent)
			_current = _current->parent;
	}

	ProfilingBlock* getRoot() { return _root; }

public:
	ProfilingBlock* _current;
	SharedPtr<ProfilingBlock> _root;
};

NS_CC_END

#endif // __SUPPORT_CCPROFILING_H__
