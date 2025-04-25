#pragma once

// standard
#include <iostream>
#include <string>
#include <memory>

namespace sdk
{

    class IOTool
    {
    public:
        enum IOToolError
        {
            OPEN_FILE_ERROR         = 1000,
            FILE_NO_EXIST_ERROR     = 1001,
            WRITE_FILE_ERROR        = 1002,
            CREATE_DIR_ERROR        = 1003,
        };

        static std::shared_ptr<FError>
        WriteStringToFile(const std::string &str, const std::string &filename)
        {
            // 父目录不存在则递归创建到父目录
            if (!IsFileParentExist(filename))
            {
                bool ret = CreateDirectoryFromFilePath(filename);
                if (!ret)
                {
                    LOG(ERROR) << "CreateDirectoryFromFilePath failed" << filename;
                    return NewFcpError(IOToolError::CREATE_DIR_ERROR,
                                       "CreateDirectoryFromFilePath failed " + filename);
                }
            }

            // 打开文件，以二进制模式写入
            std::ofstream outFile(filename);
            if (!outFile.is_open())
            {
                LOG(ERROR) << "Failed to open file!" << filename;
                return NewFcpError(IOToolError::WRITE_FILE_ERROR,
                                   "AnalyzeForHighLight Write Performance Error");
            }
            outFile << str;

            // 检查是否写入成功
            if (!outFile)
            {
                outFile.close();
                PG_LOG(PG_ERROR) << "Failed to write to file: " << filename;
                return NewFcpError(IOToolError::WRITE_FILE_ERROR,
                                   "WRITE_FILE_ERROR " + filename);
            }
            outFile.close();
            PG_LOG(PG_INFO) << "string successfully written to file: " << filename;
            return kFcpNoError;
        }

        static std::shared_ptr<FError>
        WriteBufferToFile(const std::shared_ptr<uint8_t> &bufferData,
                          size_t bufferSize, const std::string &filename)
        {
            // 父目录不存在则递归创建到父目录
            if (!IsFileParentExist(filename))
            {
                bool ret = CreateDirectoryFromFilePath(filename);
                if (!ret)
                {
                    LOG(ERROR) << "CreateDirectoryFromFilePath failed" << filename;
                    return NewFcpError(IOToolError::CREATE_DIR_ERROR,
                                       "CreateDirectoryFromFilePath failed " + filename);
                }
            }

            // 打开文件，以二进制模式写入
            std::ofstream outFile(filename, std::ios::out | std::ios::binary);
            if (!outFile)
            {
                PG_LOG(PG_ERROR) << "Failed to open file: " << filename;
                return NewFcpError(IOToolError::OPEN_FILE_ERROR,
                                   "OPEN_FILE_ERROR " + filename);
            }

            // 将缓冲区内容写入文件
            outFile.write(reinterpret_cast<const char *>(bufferData.get()), bufferSize);

            // 检查是否写入成功
            if (!outFile)
            {
                outFile.close();
                PG_LOG(PG_ERROR) << "Failed to write to file: " << filename;
                return NewFcpError(IOToolError::WRITE_FILE_ERROR,
                                   "WRITE_FILE_ERROR " + filename);
            }

            PG_LOG(PG_INFO) << "Buffer successfully written to file: " << filename;

            outFile.close();
            return kFcpNoError;
        }

    private:
        // 判断路径是否为目录
        static bool IsDirectory(const std::string &path)
        {
            struct stat buffer;
            if (stat(path.c_str(), &buffer) != 0)
            {
                return false;
            }
            return S_ISDIR(buffer.st_mode);
        }

        // 判断文件的父目录是否存在
        static bool IsFileParentExist(const std::string &filePath)
        {
            std::string dir = GetDirectoryFromPath(filePath);
            return IsDirectory(dir);
        }

        // 返回此文件的父目录
        static std::string GetDirectoryFromPath(const std::string &filePath)
        {
            // 找到最后一个路径分隔符
            size_t lastSlashPos = filePath.find_last_of("/\\");

            // 如果找到了，就返回目录部分，否则返回空字符串
            if (lastSlashPos != std::string::npos)
            {
                return filePath.substr(0, lastSlashPos);
            }

            return std::string(); // 返回空字符串表示没有目录路径
        }

        static bool StringStartsWith(const std::string &str,
                                     const std::string &prefix)
        {
            return str.find(prefix) ==
                   0; // 如果找到的索引是 0，表示该字符串以 prefix 开头
        }
    };
}