#pragma once

#include <ratio>

#include "space.h"

namespace tool {

    template<typename _ValueType, class _Unit = std::ratio<1>>
    class space;

    template<typename _ToSpace, typename _ValueType, typename _Unit>
    constexpr _ToSpace space_cast(const space<_ValueType, _Unit>& __d) {
        auto __r = __d.count() * std::ratio_divide<_Unit, typename _ToSpace::Unit>::num /
                   std::ratio_divide<_Unit, typename _ToSpace::Unit>::den;
        return _ToSpace(__r);
    }

    template <typename _ValueType, class _Unit>
    class space {
    public:
        using ValueType = _ValueType;
        using Unit = _Unit;

        // constexpr 表示函数或对象可以在编译期求值，从而允许：
        // 编译期初始化：在常量表达式中构造和操作 space 对象。
        // 优化机会：编译器可能将计算完全优化为常量，消除运行时开销。
        // 静态约束：保证函数满足常量表达式的要求（如无动态内存分配、无未定义行为）。
        constexpr space() : _value() {}

        // 允许从兼容的原始数值类型 _Rep2（如 int、double）直接构造 space 对象
        template <typename _ValueType2>
        explicit constexpr space(const _ValueType2& __value) : _value(__value) {}

        // 从其他单位类型的 space 对象构造，通过 space_cast 转换单位
        // 在类模板 space 内部，space 本身代表当前实例化的完整类型（即 space<_Rep, _Capacity>）
        template <typename _ValueType2, class _Unit2>
        constexpr space(const space<_ValueType2, _Unit2>& __d)
            : _value(space_cast<space>(__d).count()) {}

        constexpr _ValueType count() const { return _value; }

        constexpr space operator+() const { return *this; }

        // 构造一个 value 为负的 space
        constexpr space operator-() const { return space(-count()); }

        space& operator++() {
            ++_value;
            return *this;
        }

        // 返回原值
        space operator++(int) {
            space __tmp(*this);
            ++_value;
            return __tmp;
        }
        space& operator--() {
            --_value;
            return *this;
        }

        space operator--(int) {
            space __tmp(*this);
            --_value;
            return __tmp;
        }

        space& operator+=(const space& __d) {
            _value += __d.count();
            return *this;
        }

        space& operator-=(const space& __d) {
            _value -= __d.count();
            return *this;
        }

        space& operator*=(const _ValueType& __s) {
            _value *= __s;
            return *this;
        }

        space& operator/=(const _ValueType& __s) {
            _value /= __s;
            return *this;
        }

        space& operator%=(const _ValueType& __s) {
            _value %= __s;
            return *this;
        }

    private:
        ValueType _value;
    };

    template <typename _ValueType, class _Unit>
    constexpr space<_ValueType, _Unit> operator+(const space<_ValueType, _Unit>& __d1,
                                                 const space<_ValueType, _Unit>& __d2) {
        return space<_ValueType, _Unit>(__d1.count() + __d2.count());
    }

    template <typename _ValueType, class _Unit>
    constexpr space<_ValueType, _Unit> operator-(const space<_ValueType, _Unit>& __d1,
                                                 const space<_ValueType, _Unit>& __d2) {
        return space<_ValueType, _Unit>(__d1.count() - __d2.count());
    }

    using kilo = std::ratio<1024L, 1>;
    using mega = std::ratio<1024L * 1024L, 1>;
    using giga = std::ratio<1024L * 1024L * 1024L, 1>;
    using tera = std::ratio<1024L * 1024L * 1024L * 1024L, 1>;

    // 模板特化的空间单位，此处特化的是_Unit
    using bytes = space<std::size_t, std::ratio<1>>;
    using kilobytes = space<std::size_t, kilo>;
    using megabytes = space<std::size_t, mega>;
    using gigabytes = space<std::size_t, giga>;
    using terabytes = space<std::size_t, tera>;
}

