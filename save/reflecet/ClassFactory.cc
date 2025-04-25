#include "ClassFactory.h"

void ClassFactory::registerClass(const std::string& className, CreateObject method)
{
    classMap_[className] = method;
}

void* ClassFactory::createClass(const std::string& className)
{
    auto it = classMap_.find(className);
    if (it != classMap_.end())
    {
        return it->second(); 
    }
    return nullptr;
}