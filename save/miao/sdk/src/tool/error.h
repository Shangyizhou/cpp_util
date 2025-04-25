#pragma once

#include <iostream>
#include <string>
#include <memory>

namespace sdk
{

class SError {
public:
    SError() = default;
    SError(int code, const std::string& desc, const std::string& domain)
        : code_(code), desc_(desc), domain_(domain) {}
    
    int GetCode() { return code_; }
    std::string GetDesc() { return desc_; }
    std::string GetDomain() { return desc_; }

    

private:
    int code_;
    std::string desc_;
    std::string domain_;
};





}
