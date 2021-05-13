#pragma once

#include <mutex>
#include "NonCopyable.h"

namespace pb {

    class SingletonFinalizer {
    public:
        using FinalizeFunc = void(*)();
        static void AddFinalizer(FinalizeFunc func);
        static void Finalize();
    };

    template <typename T>
    class Singleton : public NonCopyable<Singleton<T>> {
    public:
        static T& get() {
            std::call_once(initFlag, Init);
            assert(instance);
            return *instance;
        }

    private:
        static void Init() {
            SingletonFinalizer::AddFinalizer(&Singleton<T>::Delete);
            instance = new T;
        }

        static void Delete() {
            delete instance;
            instance = nullptr;
            initFlag._Opaque = nullptr;
        }

    private:
        static std::once_flag initFlag;
        static T* instance;
    };

    template <typename T> std::once_flag Singleton<T>::initFlag;
    template <typename T> T* Singleton<T>::instance = nullptr;

}