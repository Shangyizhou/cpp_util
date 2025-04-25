#ifndef CLASS_FACTORY_H
#define CLASS_FACTORY_H

#include "ClassRegister.h"
#include <string>
#include <map>
#include <functional>

class ClassField;
class Object
{
public:
    Object() = default;
    virtual ~Object();

    void setClassName(const std::string& className);
    const std::string& ClassName() const;

    int fieldCount();
    ClassField* field(int pos);
    ClassField* field(const std::string& fieldName);

    template <typename T>
    void get(const std::string& fieldName, T& value);

    template <typename T>
    void set(const std::string& fieldName, const T& value);
    void set(const std::string&  fieldName, const char* value);
    
    template <typename R = void, typename ...Args>
    R call(const std::string & methodName, Args... args);

private:
    std::string className_;
};

class ClassFactory
{
public:
    using CreateObject = std::function<void*(void)>;

    static ClassFactory* getInstance()
    {
        static ClassFactory* classFactory(new ClassFactory);
        return classFactory;
    }
    void* createClass(const std::string& className);
    void registerClass(const std::string& className, CreateObject method);

private:
    ClassFactory() = default;
    ~ClassFactory() = default;
    ClassFactory(const ClassFactory&) = delete;
    ClassFactory(ClassFactory&&) = delete;
    ClassFactory& operator=(const ClassFactory&) = delete;

    std::map<std::string, CreateObject> classMap_;
    ClassRegister register_;
};



#endif // CLASS_FACTORY_H