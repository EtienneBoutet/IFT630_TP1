#pragma once

#include <mutex>
#include <condition_variable>
#include "semaphore.h"

template<class T>
class Moniteur {
public:
    template<typename ...Args>
    Moniteur(Args&&... args) : m_cl(std::forward<Args>(args)...) {}

    struct monitor_helper
    {
        monitor_helper(Moniteur* mon) : m_mon(mon), m_ul(mon->m_lock) {}
        T* operator->() { return &m_mon->m_cl; }
        Moniteur* m_mon;
        std::unique_lock<std::mutex> m_ul;
    };

    monitor_helper operator->() { return monitor_helper(this); }
    T& GetThreadUnsafeAccess() { return m_cl; }

private:
    T m_cl;
    std::mutex m_lock;
};
