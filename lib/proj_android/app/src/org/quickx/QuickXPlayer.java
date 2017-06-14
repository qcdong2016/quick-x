/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

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
package org.quickx;

import org.cocos2dx.lib.Cocos2dxHelper;
import org.cocos2dx.lib.Cocos2dxLuaJavaBridge;
import org.libsdl.app.SDLActivity;
import android.os.Bundle;
import android.util.Log;

import java.util.HashMap;

public class QuickXPlayer extends SDLActivity {

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		Cocos2dxHelper.init(this, null);
	}

	@Override
	protected String[] getLibraries() {
        return new String[] {
            "game",
		};
    }

    // for test
    public static void sayHello() {
		Log.e("QuickX", "hello world");
	}
	public static void sayHelloMap(HashMap map) {
		Log.e("QuickX", map.toString());

		int funcId = (int)map.get("testFunc");
		Cocos2dxLuaJavaBridge.callLuaFunctionWithString(funcId, "hello from java");
		Cocos2dxLuaJavaBridge.releaseLuaFunction(funcId);
	}
}
