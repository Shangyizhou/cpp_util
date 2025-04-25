#pragma once

#include <iostream>
#include <fstream>

#include <sys/stat.h> // mkdir, stat
#include <dirent.h>   // opendir, readdir
#include <unistd.h>   // rmdir, unlink

#include "logger.h"

namespace tool {

class FileTool {
public:
    FileTool() {}

    static void GetDirectoryEntries(const std::string& path, std::vector<std::string>& entries);

    static bool IsDirectory(const std::string& path);
    
    static bool CreateFile(const std::string& file_name);

    static bool CreateDir(const std::string& dirPath, bool recursive = true);

    static std::string ExtractFileFromPath(const std::string& fullPath);

    static bool PathExists(const std::string& path);
};

}