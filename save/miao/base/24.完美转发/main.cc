#include <iostream>

/**
 * 类似递归，需要有一个边界条件
 */
template <typename T> T sum(T value) { return value; }

template <typename T, typename... Args> T sum(T first, Args... args) {
  return first + sum(args...);
}

template <typename T> void process(T &) {
  std::cout << "process lvalue" << std::endl;
}

template <typename T> void process(T &&) {
  std::cout << "process rvalue" << std::endl;
}

/**
 * 引用折叠
 *  左值引用 -> 左值 -> 左值
 *  左值引用 -> 右值 -> 左值
 *  右值引用 -> 左值 -> 左值
 *  右值引用 -> 右值 -> 右值
 */
template <typename... Args> void wrapper(Args &&... args) {
  process(std::forward<Args>(args)...);
}

int main() {
  int x = 10;
  wrapper(x);            // 左值引用 process lvalue
  wrapper(20);           // 右值引用 process rvalue
  wrapper(std::move(x)); // 右值引用 process rvalue

  return 0;
}