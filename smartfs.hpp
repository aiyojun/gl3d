#pragma once

#include <string>
#include <vector>

class smartfs {
public:
    enum LIST_TYPE { ALL = 0, ONLY_DIR, ONLY_FILE, RECURSE };
    // string operation
    static std::string vector_join(const std::string& demi, const std::vector<std::string>& v);
    // No side effect methods
    static bool is_dir(const std::string& path);
    static std::string dirname(const std::string& path);
    static std::string path_join(const std::string& dir, const std::string& s);
    static std::vector<std::string> path_split(const std::string& path);
    // Methods have side effect & relative platform
    static bool file_exist(const std::string& path);
    static void list_dir(std::vector<std::string>& ls, const std::string& path, LIST_TYPE ls_type = LIST_TYPE::ALL);
    static std::string subdir_find(const std::string& dir, const std::string& filename);
};

#if defined(ALL_IMPL)

#include <exception>

std::string smartfs::vector_join(const std::string& demi, const std::vector<std::string>& v)
{
    std::string _r;
    for (std::vector<std::string>::size_type i = 0; i < v.size(); i++) {
        if (i == v.size() - 1) _r += v[i]; else _r += v[i] + demi;
    }
    return std::move(_r);
}

std::string smartfs::dirname(const std::string& path)
{
    std::string::size_type last_demi = path.find_last_of('/');
    if (last_demi == std::string::npos) return ".";
    return std::move(path.substr(0, last_demi));
}

std::string smartfs::path_join(const std::string& dir, const std::string& s)
{ return dir + "/" + s; }

std::vector<std::string> smartfs::path_split(const std::string& path)
{
    std::vector<std::string> r;
    std::string::size_type first_index = 0;
    for (std::string::size_type i = 0; i < path.length(); i++) {
        if (path[i] == '/') {
            r.emplace_back(path.substr(first_index, i - first_index));
            first_index = i + 1;
            if (first_index >= path.length()) break;
            continue;
        }
        if (i == path.length() - 1) {
            r.emplace_back(path.substr(first_index, i - first_index));
        }
    }
    return std::move(r);
}

std::string smartfs::subdir_find(const std::string& dir, const std::string& filename)
{
    if (filename.find('/') != std::string::npos) return "";
    std::vector<std::string> ls;
    list_dir(ls, dir, LIST_TYPE::RECURSE);
    for (const auto& file_path : ls) {
        std::string::size_type last_index = file_path.find_last_of('/');
        std::string name;
        if (last_index == std::string::npos) name = file_path;
        else name = file_path.substr(last_index + 1);
        if (name == filename) return file_path;
    }
    return "";
}

#ifdef linux

#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <bits/struct_stat.h>

bool smartfs::is_dir(const std::string& path)
{
    struct stat path_st{};
    stat(path.c_str(), &path_st);
    return S_ISDIR(path_st.st_mode);
}

bool smartfs::file_exist(const std::string& path)
{
    return access(path.c_str(), F_OK) != -1;
}

void smartfs::list_dir(std::vector<std::string>& ls, const std::string& path, LIST_TYPE ls_type)
{
    if (!is_dir(path)) return;
    DIR* dir_ptr = opendir(path.c_str());
    if (dir_ptr == nullptr) return;
    dirent* d_ptr;
    while ((d_ptr = readdir(dir_ptr)) != nullptr) {
        if (std::string(d_ptr->d_name) == "." || std::string(d_ptr->d_name) == "..") continue;
        if (ls_type == LIST_TYPE::ONLY_DIR) {
            if (d_ptr->d_type == 4)
                ls.emplace_back(std::string(d_ptr->d_name));
        } else if (ls_type == LIST_TYPE::ONLY_FILE) {
            if (d_ptr->d_type == 8 || d_ptr->d_type == 10)
                ls.emplace_back(std::string(d_ptr->d_name));
        } else if (ls_type == LIST_TYPE::RECURSE) {
            if (d_ptr->d_type == 4)
                list_dir(ls, path_join(path, d_ptr->d_name), LIST_TYPE::RECURSE);
            else if (d_ptr->d_type == 8 || d_ptr->d_type == 10)
                ls.emplace_back(path_join(path, d_ptr->d_name));
        } else {
            if (d_ptr->d_type == 4 || d_ptr->d_type == 8 || d_ptr->d_type == 10)
                ls.emplace_back(std::string(d_ptr->d_name));
        }
    }
}

#elif _WIN32

bool smartfs::is_dir(const std::string& path)
{
    throw std::runtime_error("unimplemented is_dir in win32");
    return false;
}

bool smartfs::file_exist(const std::string& path)
{
    throw std::runtime_error("unimplemented file_exist in win32");
    return false;
}

void smartfs::list_dir(std::vector<std::string>& ls, const std::string& path, LIST_TYPE ls_type)
{
    throw std::runtime_error("unimplemented list_dir in win32");
}

#elif __WINDOWS_

bool smartfs::is_dir(const std::string& path)
{
    throw std::runtime_error("unimplemented is_dir in windows");
    return false;
}

bool smartfs::file_exist(const std::string& path)
{
    throw std::runtime_error("unimplemented file_exist in windows");
    return false;
}

void smartfs::list_dir(std::vector<std::string>& ls, const std::string& path, LIST_TYPE ls_type)
{
    throw std::runtime_error("unimplemented list_dir in windows");
}

#endif

#endif