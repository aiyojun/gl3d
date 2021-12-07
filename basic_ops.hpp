#pragma once

#include <chrono>

namespace jlib {
    long GetSystemCurrentMs();
}

#if defined(ALL_IMPL)

inline long jlib::GetSystemCurrentMs()
{ return std::chrono::system_clock::now().time_since_epoch().count() / 100000; }

#endif