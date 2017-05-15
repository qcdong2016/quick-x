
#include "CCLuaBridge.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

using namespace std;

#include "platform/android/jni/JniHelper.h"
#include <android/log.h>

#define  LOG_TAG    "luajc"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

extern "C" {
#include "tolua_fix.h"
}

NS_CC_BEGIN

inline string jString2stdString(JNIEnv* env, jstring jstr)
{
	std::string ret;
	if(jstr){
		char* utf = (char*)env->GetStringUTFChars(jstr, 0);
		ret = utf;
		env->ReleaseStringUTFChars(jstr, utf);
	}
	else ret ="";
    
	return ret;
}

inline map<string, string> jHashMap2StdMap(JNIEnv* env, jobject jhashmap)
{
    std::map<std::string, std::string> retmap;
	do
	{
		if (jhashmap == NULL)
		{
			break;
		}
        
		if (env == NULL)
		{
			break;
		}
		static jclass mapClass = env->FindClass("java/util/HashMap");
		if (mapClass == NULL)
		{
			break;
		}
		static jmethodID mapClass_keySet = env->GetMethodID(mapClass, "keySet", "()Ljava/util/Set;");
		if (mapClass_keySet == NULL)
		{
			break;
		}
		static jmethodID mapClass_get = env->GetMethodID(mapClass, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
		if (mapClass_get == NULL)
		{
			break;
		}
		jobject keyset =  env->CallObjectMethod(jhashmap, mapClass_keySet);
		if (keyset == NULL)
		{
			break;
		}
		static jclass setClass =  env->FindClass("java/util/Set");
		if (setClass == NULL)
		{
			break;
		}
		static jmethodID setClass_toArray = env->GetMethodID(setClass, "toArray", "()[Ljava/lang/Object;");
		if (setClass_toArray == NULL)
		{
			break;
		}
		jobjectArray keyarray = (jobjectArray)env->CallObjectMethod(keyset, setClass_toArray);
		if (keyarray == NULL)
		{
			break;
		}
		jint arraysize = env->GetArrayLength(keyarray);
		for(jint i = 0; i < arraysize; i++)
		{
			jstring key = (jstring)env->GetObjectArrayElement(keyarray, i);
			jstring value = (jstring)env->CallObjectMethod(jhashmap, mapClass_get, key);
			retmap.insert(pair<std::string, std::string>(jString2stdString(env,key), jString2stdString(env,value)));
			env->DeleteLocalRef(key);
			env->DeleteLocalRef(value);
		}
        env->DeleteLocalRef(keyset);
        env->DeleteLocalRef(keyarray);
	} while (0);
	return retmap;
}

inline vector<string> jVector2stdVector(JNIEnv* env, jobject jVector)
{
    vector<string> v;
    do {
        if (jVector == NULL)
		{
			break;
		}
        
		if (env == NULL)
		{
			break;
		}
		static jclass vectorClass = env->FindClass("java/util/Vector");
		if (vectorClass == NULL)
		{
			break;
		}
        static jmethodID vectorClass_toArray = env->GetMethodID(vectorClass, "toArray", "()[Ljava/lang/Object;");
		if (vectorClass_toArray == NULL)
		{
			break;
		}
		jobjectArray valuearray = (jobjectArray)env->CallObjectMethod(jVector, vectorClass_toArray);
		if (valuearray == NULL)
		{
			break;
		}
		jint arraysize = env->GetArrayLength(valuearray);
		for(jint i = 0; i < arraysize; i++)
		{
			jstring value = (jstring)env->GetObjectArrayElement(valuearray, i);
            v.push_back(jString2stdString(env, value));
			env->DeleteLocalRef(value);
		}
        
    } while (0);
	return v;
}

inline vector<string> jArray2stdVector(JNIEnv* env, jobject jArray)
{
    vector<string> v;
    do {
        if (jArray == NULL)
		{
			break;
		}
        
		if (env == NULL)
		{
			break;
		}
		static jclass arrayClass = env->FindClass("java/util/ArrayList");
		if (arrayClass == NULL)
		{
			break;
		}
        static jmethodID arrayClass_toArray = env->GetMethodID(arrayClass, "toArray", "()[Ljava/lang/Object;");
		if (arrayClass_toArray == NULL)
		{
			break;
		}
		jobjectArray valuearray = (jobjectArray)env->CallObjectMethod(jArray, arrayClass_toArray);
		if (valuearray == NULL)
		{
			break;
		}
		jint arraysize = env->GetArrayLength(valuearray);
		for(jint i = 0; i < arraysize; i++)
		{
			jstring value = (jstring)env->GetObjectArrayElement(valuearray, i);
            v.push_back(jString2stdString(env, value));
			env->DeleteLocalRef(value);
		}
        
    } while (0);
	return v;
}

int CCLuaBridge::callStaticMethod(lua_State* L)
{
    // TODO
    return 0;
}

NS_CC_END

#endif
