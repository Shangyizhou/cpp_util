#pragma once

#include <string>

namespace tool {

namespace media
{
/**
 * IOSource 对输入的资源做抽象封装，可以是文件，网络流，某个URL
 */
class IOSource {
public:
    IOSource() = default;

    virtual ~IOSource() = default;

    // 打开资源，参数可以根据具体类型不同而不同，例如传入路径、URL、协议参数等
    virtual bool Open(const std::string &source) = 0;

    // 判断资源是否打开成功
    virtual bool IsOpen() const = 0;

    // 关闭资源
    virtual void Close() = 0;

    virtual std::string GetFileName() const = 0;

    virtual bool IsLocal() const = 0;
}; 

} // namespace media
}