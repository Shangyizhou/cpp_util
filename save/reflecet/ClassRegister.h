#ifndef CLASS_REGISTER_H
#define CLASS_REGISTER_H

#include <functional>

class ClassRegister
{
public:
    using CreateObject = std::function<void*(void)>;

    ClassRegister() = default;
    ~ClassRegister() = default;
    
    ClassRegister(const std::string& className, CreateObject method)
    {

    }
};

// ##在宏里表示字符串的连接
#define REGISTER_CLASS(className)                                       \
    Object * createObject##className()                                  \
    {                                                                   \
        Object * obj = new className();                                 \
        obj->set_class_name(#className);                                \
        return obj;                                                     \
    }                                                                  	\
    ClassRegister classRegister##className(#className, createObject##className)

#define REGISTER_CLASS_FIELD(className, fieldName, fieldType) \
    className className##fieldName; \
    ClassRegister classRegister##className##fieldName(#className, #fieldName, #fieldType, (size_t)(&(className##fieldName.fieldName)) - (size_t)(&className##fieldName))

#define REGISTER_CLASS_METHOD(className, methodName, returnType, ...) \
    std::function<returnType(className *, ##__VA_ARGS__)> className##methodName##method = &className::methodName; \
    ClassRegister classRegister##className##methodName(#className, #methodName, (uintptr_t)&(className##methodName##method))

#endif // CLASS_REGISTER_H