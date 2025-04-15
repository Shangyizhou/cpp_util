#pragma once

#include <iostream>
#include <string>
#include <memory>

namespace tool {

extern std::string kToolDomain = "com.syz.tool";
extern std::string kServiceDomain = "com.syz.service";

class Error {
public:
    Error() {}
    Error(int code, std::string& domain, std::string& desc) 
        : code_(code), domain_(domain), desc_(desc) {}
    
    std::shared_ptr<Error> NewToolError(int code, std::string& desc);
    std::shared_ptr<Error> NewServiceError(int code, std::string& desc);

    std::string ToString();

private:
    int code_;
    std::string domain_;
    std::string desc_;
};

std::shared_ptr<Error> Error::NewToolError(int code, std::string& desc) {
    return std::make_shared<Error>(code, kToolDomain, desc);
}

std::shared_ptr<Error> Error::NewServiceError(int code, std::string& desc) {
    return std::make_shared<Error>(code, kServiceDomain, desc);
}

std::string Error::ToString() {
    std::ostringstream os;
    os << "";
}

}