#pragma once

#define MAIN_BE int main(int argc, char** argv) {
#define MAIN_EN return 0;}

#include <chrono>

namespace jlib {
    long GetSystemCurrentMs();
    std::string vector_join(const std::string& demi, const std::vector<std::string>& v);
}

#if defined(ALL_IMPL)

inline long jlib::GetSystemCurrentMs()
{ return (long) (std::chrono::system_clock::now().time_since_epoch().count() / 100000); }

std::string jlib::vector_join(const std::string& demi, const std::vector<std::string>& v)
{
    std::string _r;
    for (std::vector<std::string>::size_type i = 0; i < v.size(); i++) {
        if (i == v.size() - 1) _r += v[i]; else _r += v[i] + demi;
    }
    return std::move(_r);
}

#endif