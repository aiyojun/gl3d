#pragma once

#define MAIN_BE int main(int argc, char** argv) {
#define MAIN_EN return 0;}

#include <chrono>

namespace jlib {
    long GetSystemCurrentMs();
}

#if defined(ALL_IMPL)

inline long jlib::GetSystemCurrentMs()
{ return std::chrono::system_clock::now().time_since_epoch().count() / 100000; }

#endif