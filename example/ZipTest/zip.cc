#include <iostream>
#include <fstream>

#include "file_tool.h"

#include "miniz/miniz.h"


bool compressFile(const std::string& inputPath, const std::string& outputPath) {
    // 读取原始文件
    std::ifstream in(inputPath, std::ios::binary | std::ios::ate);
    if (!in) return false;
    size_t fileSize = in.tellg();
    in.seekg(0);
    
    std::vector<uint8_t> inBuf(fileSize);
    in.read((char*)inBuf.data(), fileSize);
    in.close();

    // 压缩数据
    size_t cmpLen = 0;
    uint8_t* pCmpData = (uint8_t*)tdefl_compress_mem_to_heap(
        inBuf.data(), fileSize, &cmpLen, TDEFL_WRITE_ZLIB_HEADER | TDEFL_DEFAULT_MAX_PROBES);
    
    // 写入压缩文件
    std::ofstream out(outputPath, std::ios::binary);
    out.write((char*)pCmpData, cmpLen);
    out.close();
    
    mz_free(pCmpData);
    return true;
}

bool decompressFile(const std::string& inputPath, const std::string& outputPath) {
    // 读取压缩文件
    std::ifstream in(inputPath, std::ios::binary | std::ios::ate);
    if (!in) return false;
    size_t cmpLen = in.tellg();
    in.seekg(0);
    
    std::vector<uint8_t> cmpBuf(cmpLen);
    in.read((char*)cmpBuf.data(), cmpLen);
    in.close();

    // 解压数据
    size_t decmpLen = 0;
    uint8_t* pDecmpData = (uint8_t*)tinfl_decompress_mem_to_heap(
        cmpBuf.data(), cmpLen, &decmpLen, TINFL_FLAG_PARSE_ZLIB_HEADER);
    
    // 写入解压文件
    std::ofstream out(outputPath, std::ios::binary);
    out.write((char*)pDecmpData, decmpLen);
    out.close();
    
    mz_free(pDecmpData);
    return true;
}

void compressDirectory(const std::string& dirPath, const std::string& outputZip) {
    // 1. 初始化zip存档
    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));
    if (!mz_zip_writer_init_file(&zip, outputZip.c_str(), 0)) {
        throw std::runtime_error("Failed to initialize zip archive");
    }

    // 2. 递归添加文件的lambda
    std::function<void(const std::string&)> addFiles = [&](const std::string& currentPath) {
        // 获取目录下所有条目
        std::vector<std::string> entries;
        tool::FileTool::GetDirectoryEntries(currentPath, entries);
        for (const auto& entry : entries) {
            std::string fullPath = currentPath + "/" + entry;
            if (tool::FileTool::IsDirectory(fullPath)) {
                addFiles(fullPath); // 递归处理子目录
            } else {
                // 计算相对路径（相对于dirPath）
                std::string relPath = fullPath.substr(dirPath.length() + 1);
                
                // 3. 添加文件到zip
                if (!mz_zip_writer_add_file(&zip, 
                                          relPath.c_str(), 
                                          fullPath.c_str(), 
                                          nullptr, 
                                          0, 
                                          MZ_BEST_COMPRESSION)) {
                    throw std::runtime_error("Failed to add file: " + fullPath);
                }
            }
        }
    };

    // 4. 开始处理
    addFiles(dirPath);

    // 5. 完成压缩
    if (!mz_zip_writer_finalize_archive(&zip) || 
        !mz_zip_writer_end(&zip)) {
        throw std::runtime_error("Failed to finalize zip archive");
    }
}

void decompressDirectory(const std::string& zipPath, const std::string& outputDir) {
    // 1. 初始化zip存档
    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));
    if (!mz_zip_reader_init_file(&zip, zipPath.c_str(), 0)) {
        throw std::runtime_error("Failed to open zip file");
    }

    // 2. 创建输出目录（如果不存在）
    tool::FileTool::CreateDir(outputDir, true);

    // 3. 遍历zip内所有文件
    for (mz_uint i = 0; i < mz_zip_reader_get_num_files(&zip); i++) {
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zip, i, &file_stat)) {
            mz_zip_reader_end(&zip);
            throw std::runtime_error("Failed to read file info in zip");
        }

        // 处理路径分隔符（Windows兼容）
        std::string filePath = file_stat.m_filename;
        std::replace(filePath.begin(), filePath.end(), '\\', '/');

        // 4. 构建完整输出路径
        std::string fullPath = outputDir + "/" + filePath;

        // 5. 创建子目录（如果需要）
        size_t lastSlash = fullPath.find_last_of('/');
        if (lastSlash != std::string::npos) {
            std::string dirPath = fullPath.substr(0, lastSlash);
            tool::FileTool::CreateDir(dirPath, true);
        }

        // 6. 解压文件
        if (!mz_zip_reader_is_file_a_directory(&zip, i)) {
            // 提取文件内容到内存
            size_t uncomp_size;
            void* p = mz_zip_reader_extract_to_heap(&zip, i, &uncomp_size, 0);
            if (!p) {
                mz_zip_reader_end(&zip);
                throw std::runtime_error("Failed to extract file: " + filePath);
            }

            // 写入磁盘文件
            std::ofstream outFile(fullPath, std::ios::binary);
            if (!outFile.write((char*)p, uncomp_size)) {
                free(p);
                mz_zip_reader_end(&zip);
                throw std::runtime_error("Failed to write file: " + fullPath);
            }
            free(p);
        }
    }

    // 7. 关闭zip存档
    if (!mz_zip_reader_end(&zip)) {
        throw std::runtime_error("Failed to close zip archive");
    }
}

int main() {
    std::string testDir = "test_data";
    std::string zipFile = "test_data.zip";
    std::string extractDir = "extracted_data";

    try {
        // 1. 创建测试目录和文件
        std::cout << "===== 准备测试数据 =====" << std::endl;
        tool::FileTool::CreateDir(testDir, true);
        
        // 创建几个测试文件
        tool::FileTool::CreateFile(testDir + "/file1.txt");
        tool::FileTool::CreateFile(testDir + "/file2.log");
        
        // 创建子目录
        tool::FileTool::CreateDir(testDir + "/subdir", true);
        tool::FileTool::CreateFile(testDir + "/subdir/file3.dat");
        
        std::cout << "测试目录结构已创建: " << testDir << std::endl;

        // 2. 压缩目录
        std::cout << "\n===== 开始压缩目录 =====" << std::endl;
        compressDirectory(testDir, zipFile);
        std::cout << "目录已压缩到: " << zipFile << std::endl;

        // 3. 解压缩
        std::cout << "\n===== 开始解压缩 =====" << std::endl;
        decompressDirectory(zipFile, extractDir);
        std::cout << "文件已解压到: " << extractDir << std::endl;

        // 4. 验证文件
        std::cout << "\n===== 验证解压结果 =====" << std::endl;
        std::vector<std::string> expectedFiles = {
            "/file1.txt",
            "/file2.log",
            "/subdir/file3.dat"
        };

        bool allOK = true;
        for (const auto& file : expectedFiles) {
            std::string path = extractDir + file;
            if (tool::FileTool::PathExists(path)) {
                std::cout << "[✓] 文件存在: " << path << std::endl;
            } else {
                std::cout << "[×] 文件缺失: " << path << std::endl;
                allOK = false;
            }
        }

        if (allOK) {
            std::cout << "\n测试成功！所有文件验证通过。" << std::endl;
        } else {
            std::cout << "\n测试失败！部分文件缺失。" << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "\n错误发生: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}