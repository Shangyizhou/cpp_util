#include "File/file_tool.h"
#include <vector>
#include <map>

int main() {
    std::string path = "/home/shangyizhou/code/cpp/tool/bin/temp";

    std::string file_name = path + "/file";

    tool::FileTool::CreateDir(path + "/test", true);
    tool::FileTool::CreateFile(file_name);

    bool is_exist = tool::FileTool::PathExists(file_name);
    if (is_exist) {
        LOG_INFO("{} is exist ", file_name);
    }

    LOG_INFO("name is {}", tool::FileTool::ExtractFileFromPath(file_name));    

    return 0;
}