#pragma once

#include "Foundation/error.h"

#include <memory>

namespace tool {

/**
 * 1. 构造函数不能是虚函数，但需要提供实现（可空）
 * 2. 虚析构函数（必须！）
 * 3. 纯虚函数（接口方法）
 */
class DecoderInterface {
public:
    DecoderInterface() = default;
    virtual ~DecoderInterface() = default;
    virtual std::shared_ptr<Error> Prepare() = 0; 
    virtual std::shared_ptr<Error> Close() = 0;
    virtual std::shared_ptr<Error> Decode() = 0;
};

}