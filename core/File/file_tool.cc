#include "file_tool.h"

namespace tool {

void FileTool::GetDirectoryEntries(const std::string& path, std::vector<std::string>& entries) {
    entries.clear();
    DIR* dir = opendir(path.c_str());
    if (!dir) return;
    
    struct dirent* ent;
    while ((ent = readdir(dir)) != nullptr) {
        if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
            entries.push_back(ent->d_name);
        }
    }
    closedir(dir);
}

bool FileTool::IsDirectory(const std::string& path) {
    struct stat pathStat;
    if (stat(path.c_str(), &pathStat) != 0) {
        return false; // 路径不存在或无法访问
    }
    return S_ISDIR(pathStat.st_mode);
}

bool FileTool::CreateFile(const std::string& file_name) {
    std::ofstream file(file_name);
    if (!file) {
        LOG_ERROR("Failed to create file: {}", file_name);
        return false;
    }
    return true;
}

// 创建目录（支持递归创建）
bool FileTool::CreateDir(const std::string& dirPath, bool recursive) {
    if (dirPath.empty()) {
        return false;
    }

    size_t pos = 0;
    if (dirPath[0] == '/') {
        pos = 1; // 处理绝对路径
    }

    while ((pos = dirPath.find('/', pos)) != std::string::npos) {
        std::string subdir = dirPath.substr(0, pos);
        if (!PathExists(subdir) && mkdir(subdir.c_str(), 0755) != 0) {
            if (!recursive || errno != ENOENT) {
                LOG_ERROR("Failed to create directory: {}", subdir);
                return false;
            }
        }
        pos++;
    }

    return mkdir(dirPath.c_str(), 0755) == 0 || errno == EEXIST;
}

std::string FileTool::ExtractFileFromPath(const std::string& fullPath) {
    size_t pos = fullPath.find_last_of("/\\"); // 查找路径中最后一个 / 或 \（兼容Windows和Unix路径格式）
    if (pos != std::string::npos) {
        return fullPath.substr(pos + 1);
    }
    return fullPath;
}

 // 检查路径是否存在
bool FileTool::PathExists(const std::string& path) {
    struct stat info;
    return stat(path.c_str(), &info) == 0;
}

}