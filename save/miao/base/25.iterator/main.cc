#include <iostream>
#include <vector>
#include <algorithm>

void func1() {
  std::vector<int> vec{1, 2, 3, 4, 5};

  std::vector<int>::iterator iter = vec.begin();
  while (iter != vec.end()) {
    std::cout << *iter << " ";
    iter++;
  }
  std::cout << std::endl;
}


void func2(const std::vector<int>::iterator& begin, const std::vector<int>::iterator& end) {
  std::sort(begin, end);
}

void func3() {
  std::vector<int> vec = {1, 2, 3, 4, 5};

  auto it = vec.begin() + 2;
  vec.insert(it, 6);

  for (const auto& it : vec) {
    std::cout << it << " ";
  }
  std::cout << std::endl;

  std::cout << "原来迭代器访问 " << *it; // 原来迭代器访问 -1355272176%
}

int main() {
  std::cout << "\n -------- func 1 --------\n" << std::endl;
  func1();

  std::cout << "\n -------- func 2 --------\n" << std::endl;
  std::vector<int> vec = {5, 2, 9, 1, 5, 6, 3, 8, 4, 7};

  for (const auto& it : vec) {
    std::cout << it << " ";
  }
  std::cout << std::endl;

  func2(vec.begin(), vec.end());

  for (const auto& it : vec) {
    std::cout << it << " ";
  }
  std::cout << std::endl;

  std::cout << "\n -------- func 3 --------\n" << std::endl;
  func3();
}