/*================================================================
Filename: Singleton.h
Date: 2017.9.11
Created by AirGuanZ
================================================================*/
#ifndef __OWE_SINGLETON_H__
#define __OWE_SINGLETON_H__

#include <memory>
#include <cassert>
#include "Common.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(Utility)

/*
非线程安全的单例实现
*/
template<typename T>
class Singleton
{
    class _singleton_deleter
    {
    public:
        void operator()(T *ptr)
        {
            delete ptr;
        }
    };

public:
    using SingletonType = Singleton<T>;
    using SingletonPtr = std::unique_ptr<T, _singleton_deleter>;

    static T *GetInstancePtr(void)
    {
        return _singleton_instance.get();
    }

    static T &GetInstance(void)
    {
        return *GetInstancePtr();
    }

    template<typename...Args>
    static void InitInstance(Args...args)
    {
        assert(!GetInstancePtr());
        _singleton_instance.reset(new T(args...));
    }

    static void DelInstance(void)
    {
        assert(GetInstancePtr());
        _singleton_instance.reset();
    }

    static bool IsInstanceAvailable(void)
    {
        return GetInstancePtr() != nullptr;
    }

private:

    static SingletonPtr _singleton_instance;
};

#define OWE_SINGLETON_INSTANCE_PTR(TYPENAME) \
    OWE::Utility::Singleton<TYPENAME>::SingletonPtr \
    OWE::Utility::Singleton<TYPENAME>::_singleton_instance

__OWE_END_NAMESPACE__(Utility)
__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_SINGLETON_H__
