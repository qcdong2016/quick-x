#ifndef _ATTRIBUTEACCESSOR_H_
#define _ATTRIBUTEACCESSOR_H_

#include <string>
#include <list>
#include <map>
#include "base/Variant.h"


namespace cocos2d {
	class CCNode;

class AttributeAccessor : public RefCounted
{
public:
	AttributeAccessor(const std::string& name) : _name(name) {}

	virtual void get(const CCNode* ptr, Variant& dest) const = 0;
	virtual void set(CCNode* ptr, const Variant& src) = 0;

	virtual const std::string& getName() { return _name; }

private:
	std::string _name;
};

template<typename T> struct AttributeTrait
{
	typedef const T& ReturnType;
	typedef const T& ParameterType;
};

template<> struct AttributeTrait<int>
{
	typedef int ReturnType;
	typedef int ParameterType;
};

template<> struct AttributeTrait<bool>
{
	typedef bool ReturnType;
	typedef bool ParameterType;
};

template<> struct AttributeTrait<float>
{
	typedef float ReturnType;
	typedef float ParameterType;
};

template<typename T> struct MixedAttributeTrait
{
	typedef T ReturnType;
	typedef const T& ParameterType;
};

template<typename T> struct AttributeTraitEnum
{
	typedef T ReturnType;
	typedef T ParameterType;
};

template <typename T, typename U, typename Trait> 
class AttributeAccessorImpl : public AttributeAccessor
{
public:

	typedef typename Trait::ReturnType(T::*getFunctionPtr)() const;
	typedef void (T::*setFunctionPtr)(typename Trait::ParameterType);

	AttributeAccessorImpl(const std::string& name, getFunctionPtr getFunction, setFunctionPtr setFunction)
		: AttributeAccessor(name)
		, _getFunc(getFunction)
		, _setFunc(setFunction)
	{}

	/// Invoke getter function.
	virtual void get(const CCNode* ptr, Variant& dest) const
	{
		assert(ptr);
		const T* classPtr = static_cast<const T*>(ptr);
		U value = (classPtr->*_getFunc)();
		dest = value;
	}

	/// Invoke setter function.
	virtual void set(CCNode* ptr, const Variant& value)
	{
		assert(ptr);
		T* classPtr = static_cast<T*>(ptr);
		(classPtr->*_setFunc)(value.Get<U>());
	}

	getFunctionPtr _getFunc;
	setFunctionPtr _setFunc;
};


template <typename T, typename U, typename Trait> 
class AttributeAccessorHelper : public AttributeAccessor
{
public:

	typedef typename Trait::ReturnType(*getFunctionPtr)(const T*);
	typedef void(*setFunctionPtr)(T*, typename Trait::ParameterType);

	AttributeAccessorHelper(const std::string& name, getFunctionPtr getFunction, setFunctionPtr setFunction)
		: AttributeAccessor(name)
		, _getFunc(getFunction)
		, _setFunc(setFunction)
	{}

	/// Invoke getter function.
	virtual void get(const CCNode* ptr, Variant& dest) const
	{
		assert(ptr);
		const T* classPtr = static_cast<const T*>(ptr);
		U value = _getFunc(classPtr);
		dest = value;
	}

	/// Invoke setter function.
	virtual void set(CCNode* ptr, const Variant& value)
	{
		assert(ptr);
		T* classPtr = static_cast<T*>(ptr);
		_setFunc(classPtr, value.Get<U>());
	}
	getFunctionPtr _getFunc;
	setFunctionPtr _setFunc;
};



template <typename T, typename U>
class AttributeAccessorEnum : public AttributeAccessor
{
	typedef U(T::*getFunctionPtr)() const;
	typedef void (T::*setFunctionPtr)(U);
public:
	AttributeAccessorEnum(const std::string& name, getFunctionPtr getFunction, setFunctionPtr setFunction)
		: AttributeAccessor(name)
		, _getFunc(getFunction)
		, _setFunc(setFunction)
	{}

	/// Invoke getter function.
	virtual void get(const CCNode* ptr, Variant& dest) const
	{
		assert(ptr);
		const T* classPtr = static_cast<const T*>(ptr);
		U value = (classPtr->*_getFunc)();
		dest = (int)value;
	}

	/// Invoke setter function.
	virtual void set(CCNode* ptr, const Variant& value)
	{
		assert(ptr);
		T* classPtr = static_cast<T*>(ptr);
		(classPtr->*_setFunc)((U)value.Get<int>());
	}

	getFunctionPtr _getFunc;
	setFunctionPtr _setFunc;
};

struct EnumInfo
{
	const char* name;
	int value;
};

class AttributeInfo
{
public:
	VariantType type;

	SharedPtr<AttributeAccessor> accessor;
	Variant singleStep;
	Variant defaultValue;

	EnumInfo* enuminfo;
	int enuminfo_count;

	AttributeInfo(AttributeAccessor* accessor_, VariantType type_)
		: accessor(accessor_)
		, type(type_)
		, enuminfo(nullptr)
	{
	}

	AttributeInfo(AttributeAccessor* accessor_, Variant def = Variant(), Variant step = Variant())
		: accessor(accessor_)
		, defaultValue(def)
		, singleStep(step)
		, enuminfo(nullptr)
	{
		type = defaultValue.GetType();
	}

	AttributeInfo(AttributeAccessor* accessor_, Variant def, EnumInfo* info, int count)
		: accessor(accessor_)
		, defaultValue(def)
		, enuminfo(info)
		, enuminfo_count(count)
	{
		type = VAR_INT;
	}

	bool isEnum() { return enuminfo != nullptr; }
};

#define ATTR_(trait, attributeName, get, set, typeName, defaultValue) \
	ObjectFactoryManager::registerAttribute(getTypeStatic(), AttributeInfo(new AttributeAccessorImpl<SelfType, typeName, trait<typeName> >(attributeName, &SelfType::get, &SelfType::set), defaultValue))

#define ATTR(attributeName, get, set, typeName, defaultValue) ATTR_(AttributeTrait, attributeName, get, set, typeName, defaultValue)

}
#endif//_ATTRIBUTEACCESSOR_H_
