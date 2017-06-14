
#include "scripting/CCLuaStack.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID


#include "scripting/CCLuaEngine.h"
#include "platform/CCInput.h"
#include "platform/android/jni/JniHelper.h"
#include "platform/CCDevice.h"

using namespace cocos2d;

extern "C" {

static int queryInGLThread(std::function<void()> func)
{
    EventCallFuncData* data = (EventCallFuncData*)calloc(sizeof(EventCallFuncData), 1);
    data->func = func;
    return SubSystem::get<Input>()->pushUserEvent(CALL_FUNC, data);
}

JNIEXPORT jint JNICALL Java_org_cocos2dx_lib_Cocos2dxLuaJavaBridge_callLuaFunctionWithString
  (JNIEnv *env, jclass cls, jint functionId, jstring value)
{
    const char *value_ = env->GetStringUTFChars(value, 0);

	std::string str(value_);
    env->ReleaseStringUTFChars(value, value_);

    return queryInGLThread([=]() {
		CCLuaStack* stack = CCLuaEngine::defaultEngine()->getLuaStack();
        lua_State* L = stack->getLuaState();
        lua_rawgeti(L, LUA_REGISTRYINDEX, functionId);
        lua_pushstring(L, str.c_str());
        stack->executeFunction(1);
	});
}

JNIEXPORT jint JNICALL Java_org_cocos2dx_lib_Cocos2dxLuaJavaBridge_callLuaGlobalFunctionWithString
  (JNIEnv *env, jclass cls, jstring luaFunctionName, jstring value)
{
    const char *luaFunctionName_ = env->GetStringUTFChars(luaFunctionName, 0);
    const char *value_ = env->GetStringUTFChars(value, 0);
   
    std::string funcName(luaFunctionName_);
    std::string str(value_);

    int ret = queryInGLThread([=]() {
		CCLuaStack* stack = CCLuaEngine::defaultEngine()->getLuaStack();
		stack->pushString(str.c_str());
		stack->executeGlobalFunction(funcName.c_str(), 1);
    });
    
    env->ReleaseStringUTFChars(luaFunctionName, luaFunctionName_);
    env->ReleaseStringUTFChars(value, value_);
    return ret;
}

JNIEXPORT jint JNICALL Java_org_cocos2dx_lib_Cocos2dxLuaJavaBridge_releaseLuaFunction
        (JNIEnv *env, jclass cls, jint value)
{

    return queryInGLThread([=]() {
        CCLuaStack* stack = CCLuaEngine::defaultEngine()->getLuaStack();
        luaL_unref(stack->getLuaState(), LUA_REGISTRYINDEX, value);
    });
}
} // extern "C"

class AutoReleaseJni
{
public:
    AutoReleaseJni(JNIEnv* e)
    : env(e)
    {
        env->PushLocalFrame(256);
    }

    ~AutoReleaseJni()
    {
        env->PopLocalFrame(nullptr);
    }

    JNIEnv* env;
};

int callStaticMethod(lua_State* L)
{
    if (!(lua_isstring(L, 1) && lua_isstring(L, 2)))
    {
        lua_pushboolean(L, false);
        lua_pushstring(L, "invalid param.");
        return 2;
    }

    const char* className = lua_tostring(L, 1);
    const char* methodName = lua_tostring(L, 2);
    JNIEnv* env = JniHelper::getEnv();

    AutoReleaseJni r(env);
    jclass jclassId = env->FindClass(className);

    if (lua_istable(L, 3)) {
        jmethodID methodId = env->GetStaticMethodID(jclassId, methodName, "(Ljava/util/HashMap;)V");
        jclass hashMapCls = env->FindClass("java/util/HashMap");
        jmethodID hashMapInit = env->GetMethodID(hashMapCls, "<init>", "()V");
        jobject hashMapObj = env->NewObject(hashMapCls, hashMapInit);

        jmethodID hashMapPut = env->GetMethodID(hashMapCls, "put",
                                                "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

        lua_pushnil(L);/* first key */
        while (lua_next(L, -2)) {
            jstring key = env->NewStringUTF(lua_tostring(L, -2));

            switch (lua_type(L, -1))
            {
                case LUA_TNUMBER:
                {
                    lua_Number number = lua_tonumber(L, -1);
                    jclass doubleCls = env->FindClass("java/lang/Double");
                    jmethodID method = env->GetMethodID(doubleCls, "<init>", "(D)V");
                    jobject doubleObj = env->NewObject(doubleCls, method, number);
                    env->CallObjectMethod(hashMapObj, hashMapPut, key, doubleObj);
                    break;
                }
                case LUA_TBOOLEAN:
                {
                    bool b = !!lua_toboolean(L, -1);
                    jclass booleanCls = env->FindClass("java/lang/Boolean");
                    jmethodID method = env->GetMethodID(booleanCls, "<init>", "(Z)V");
                    jobject booleanObject = env->NewObject(booleanCls, method, b);
                    env->CallObjectMethod(hashMapObj, hashMapPut, key, booleanObject);
                    break;
                }
                case LUA_TSTRING:
                {
                    jstring s = env->NewStringUTF(lua_tostring(L, -1));
                    env->CallObjectMethod(hashMapObj, hashMapPut, key, s);
                    break;
                }
                case LUA_TFUNCTION:
                {
                    lua_pushvalue(L, -1);
                    int funcId = luaL_ref(L, LUA_REGISTRYINDEX);
                    jclass intCls = env->FindClass("java/lang/Integer");
                    jmethodID method = env->GetMethodID(intCls, "<init>", "(I)V");
                    jobject intObj = env->NewObject(intCls, method, funcId);
                    env->CallObjectMethod(hashMapObj, hashMapPut, key, intObj);
                    break;
                }
                case LUA_TNIL:
                {
                    env->CallObjectMethod(hashMapObj, hashMapPut, key, nullptr);
                    break;
                }
                default:
                {
                    CCLog("value type `%s` not support.", lua_typename(L, -1));
                    break;
                }
            }
            lua_pop(L, 1);
        }

        env->CallStaticVoidMethod(jclassId, methodId, hashMapObj);
    } else {
        jmethodID methodId = env->GetStaticMethodID(jclassId, methodName, "()V");
        if (!methodId)
        {
            lua_pushboolean(L, false);
            lua_pushstring(L, "method not found.");
            return 2;
        }
        env->CallStaticVoidMethod(jclassId, methodId);
    }

    lua_pushboolean(L, true);
    return 1;
}


#endif
