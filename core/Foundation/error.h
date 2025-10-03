#pragma once

#include <iostream>
#include <string>
#include <memory>

namespace tool {

extern std::string kToolDomain;
extern std::string kServiceDomain;


class Error {
public:
    Error() {}
    Error(int code, const std::string& domain, const std::string& desc) 
        : code_(code), domain_(domain), desc_(desc) {}

    std::string ToString();

    static std::shared_ptr<Error> kNoError;

private:
    int code_;
    std::string domain_;
    std::string desc_;
};

std::shared_ptr<Error> NewToolError(int code, const std::string& desc);

std::shared_ptr<Error> NewServiceError(int code, const std::string& desc);

}