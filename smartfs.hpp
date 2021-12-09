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
    static std::string runtime_path();
    static const std::vector<std::string>& get_cache();
    // Methods have side effect & relative platform
    static bool file_exist(const std::string& path);
    static void cache(const std::string& root);
    static std::string find(const std::string& file);
    static std::string prefix(const std::string& , const std::string& node);
    static void list_dir(std::vector<std::string>& ls, const std::string& path, LIST_TYPE ls_type = LIST_TYPE::ALL);
    static std::string subdir_find(const std::string& dir, const std::string& filename, bool use_cache = false);
private:
    static std::vector<std::string> finding_cache;
};

#if defined(ALL_IMPL)

#include <exception>

std::vector<std::string> smartfs::finding_cache;

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

const std::vector<std::string>& smartfs::get_cache()
{ return finding_cache; }

void smartfs::cache(const std::string& root)
{
    finding_cache.clear();
    list_dir(finding_cache, root, LIST_TYPE::RECURSE);
    std::cout << "Finding Cache:\n" << vector_join("\n  ", finding_cache) << std::endl;
}

std::string smartfs::find(const std::string& filename)
{
    for (const auto& file_path : finding_cache) {
        std::string::size_type last_index = file_path.find_last_of('/');
        std::string name;
        if (last_index == std::string::npos) name = file_path;
        else name = file_path.substr(last_index + 1);
        if (name == filename) return file_path;
    }
    return "";
}

std::string smartfs::subdir_find(const std::string& dir, const std::string& filename, bool use_cache)
{
    if (filename.find('/') != std::string::npos) return "";
    if (!use_cache) {
        finding_cache.clear();
        list_dir(finding_cache, dir, LIST_TYPE::RECURSE);
    }
    for (const auto& file_path : finding_cache) {
        std::string::size_type last_index = file_path.find_last_of('/');
        std::string name;
        if (last_index == std::string::npos) name = file_path;
        else name = file_path.substr(last_index + 1);
        if (name == filename) return file_path;
    }
    return "";
}


//std::string smartfs::runtime_path()
//{
//    char buf[512] = {0};
//    if (!getcwd(buf, 512)) return "";
//    return std::move(std::string(buf));
//}

#ifdef linux

#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <climits>
#include <sys/stat.h>
#include <bits/struct_stat.h>

std::string smartfs::runtime_path()
{
    char buf[512] = {0};
    if (!getcwd(buf, 512)) return "";
    return std::move(std::string(buf));
}


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

std::string smartfs::prefix(const std::string& file_path, const std::string& node)
{
    char buf[512] = {0};
    if (!realpath(file_path.c_str(), buf)) return "";
    std::string absolute_path(buf);
    std::string::size_type index = absolute_path.find_first_of(node);
    std::string r = index == std::string::npos ? absolute_path.substr(0) : absolute_path.substr(0, index);
    if (r.length() == file_path.length()) return "";
    while (r[r.length() - 1] == '/') r = r.substr(0, r.length() - 1);
    return path_join(r, node);
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

#include <io.h>
#include <windows.h>
#include <cstring>
#include <direct.h>

std::string smartfs::runtime_path()
{
    char buf[512] = {0};
    if (!getcwd(buf, 512)) return "";
    return std::move(std::string(buf));
}

bool smartfs::is_dir(const std::string& path)
{
    WIN32_FIND_DATAA FindFileData;
    // throw std::runtime_error("unimplemented is_dir in win32");
    return FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? true : false;
}

bool smartfs::file_exist(const std::string& path)
{
    // throw std::runtime_error("unimplemented file_exist in win32");
    return access(path.c_str(), 0) == 0;
}

// std::string smartfs::runtime_path()
// {
//     return "";
// }

std::string smartfs::prefix(const std::string& file_path, const std::string& node)
{
	// GetModuleFileName(NULL, szPath, sizeof(szPath) - 1);
    return "";
}

void smartfs::list_dir(std::vector<std::string>& ls, const std::string& path, LIST_TYPE ls_type)
{
    // throw std::runtime_error("unimplemented list_dir in win32");
	WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile((path + "/*.*").c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE) return;
    LARGE_INTEGER size;
    do {
        std::string p_name(findData.cFileName);
        if (p_name == "." || p_name == "..") continue;
        int isDir = findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
        if (ls_type == LIST_TYPE::ONLY_DIR) {
            if (isDir)
                ls.emplace_back(p_name);
        } else if (ls_type == LIST_TYPE::ONLY_FILE) {
            if (!isDir)
                ls.emplace_back(p_name);
        } else if (ls_type == LIST_TYPE::RECURSE) {
            if (isDir)
                list_dir(ls, path_join(path, p_name), LIST_TYPE::RECURSE);
            else
                ls.emplace_back(path_join(path, p_name));
        } else {
            ls.emplace_back(std::string(p_name));
        }
	} while (FindNextFile(hFind, &findData));
}

#endif

#endif