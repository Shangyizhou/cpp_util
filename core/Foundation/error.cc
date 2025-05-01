#include "error.h"

namespace tool {

std::string kToolDomain = "com.syz.tool";
std::string kServiceDomain = "com.syz.service";

std::shared_ptr<Error> Error::kNoError = nullptr;

std::shared_ptr<Error> NewToolError(int code, const std::string& desc) {
    return std::make_shared<Error>(code, kToolDomain, desc);
}

std::shared_ptr<Error> NewServiceError(int code, const std::string& desc) {
    return std::make_shared<Error>(code, kServiceDomain, desc);
}

std::string Error::ToString() {
    std::string result;
    result += "Error Code: " + std::to_string(code_) + "\n";
    result += "Domain: " + domain_ + "\n";
    result += "Description: " + desc_ + "\n";
    return result;
}

}