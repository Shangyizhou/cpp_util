#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

class Any {
public:
    template<typename T>
    Any(const T& value) : base_ptr_(new Derived<T>(value)) {}

    Any(const Any& any) : base_ptr_(any.Clone()) {}

    template<typename T>
    T& GetClass() {
        // dynamic_pointer_cast 只能用于 shared_ptr 
        auto derived = dynamic_cast<Derived<T>*>(base_ptr_.get());
        return derived->value_; 
    }

    Any& operator=(const Any& a)
    {
        if (base_ptr_ == a.base_ptr_)
            return *this;

        base_ptr_ = a.Clone();
        return *this;
    }

    ~Any() {}

private:
    class Base {
    public:
        virtual std::unique_ptr<Base> Clone() const = 0;
        virtual ~Base() {}
    };

    template<typename T>
    class Derived : public Base {
    public:
        template<typename U>
        Derived(U&& value) : value_(std::forward<U>(value)) {}

        template<typename U>
        Derived(const U& value) : value_(value) {}

        std::unique_ptr<Base> Clone() const {
            return std::unique_ptr<Base>(new Derived(value_));
        }

        ~Derived() {}

    private:
        T value_;
    };

    std::unique_ptr<Base> Clone() const
    {
        if (base_ptr_ != nullptr)
            return base_ptr_->Clone();

        return nullptr;
    }

    std::unique_ptr<Base> base_ptr_;
};



int main() {
    // std::unique_ptr<Base> base(new Derived<std::string>("hello"));

    // std::vector<int> vec{1, 2, 3};
    // std::unique_ptr<Base> base2(new Derived<std::vector<int>>(std::move(vec)));

    // std::unordered_map<std::string, std::string> map;
    // map["a"] = "a";
    // map["b"] = "b";
    // std::unique_ptr<Base> base3(new Derived<std::vector<int>>(std::move(map)));

    Any int_var = 1;
    std::string str = "hello";
    Any str_var = str;

    int get_int = int_var.GetClass<int>();
    std::string get_str = str_var.GetClass<std::string>();

    std::cout << "get_int = " << get_int << std::endl;
    std::cout << "get_str = " << get_str << std::endl;

    return 0;
}