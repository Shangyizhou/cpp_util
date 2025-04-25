#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

void func1() {
  std::vector<int> vec{1, 2, 3, 4, 5};
  for (const auto& it : vec) {
    std::cout << it << " ";
  }
  std::cout << std::endl;
}

void func2() {
  std::vector<int> vec{};
  
  for (int i = 0; i < 5; i++) {
    vec.push_back(i + 1);
    std::cout << "size: " << vec.size() << " capacity: " << vec.capacity() << std::endl;
  }
  // size: 1 capacity: 1
  // size: 2 capacity: 2
  // size: 3 capacity: 4
  // size: 4 capacity: 4
  // size: 5 capacity: 8
}

void func3(const std::vector<int>& vec) {
  for (const auto& it : vec) {
    std::cout << it << " ";
  }
  std::cout << std::endl;
}

void func4() {
  // 1 2 3 
  // 1 2 3 10 4  
  std::vector<int> vec{1, 2, 3, 4, 5};
  vec.insert(vec.begin() + 3, 10);
  vec.erase(vec.end() - 1);

  for (const auto& it : vec) {
    std::cout << it << " ";
  }
  std::cout << std::endl;
}

void func5() {
  std::srand(std::time(nullptr));
  
  std::vector<int> vec;
  vec.reserve(20);
  for (int i = 0; i < 20; ++i) {
      vec.push_back(std::rand() % 10 + 1); // 生成1-10的随机数
  }

  for (int num : vec) {
      std::cout << num << " ";
  }
  std::cout << std::endl;

  sort(vec.begin(), vec.end());

  for (int num : vec) {
      std::cout << num << " ";
  }
  std::cout << std::endl;


  // std::unique只是将重复元素移动到末尾，返回新逻辑末尾的迭代器
  auto last = std::unique(vec.begin(), vec.end());
  
  vec.erase(last, vec.end());
  std::cout << "去重后的vector内容: ";
  for (int num : vec) {
      std::cout << num << " ";
  }
  std::cout << std::endl;

  int target = 5;
  auto it = std::find(vec.begin(), vec.end(), target);

  if (it != vec.end()) {
    std::cout << target << " in index " << (it - vec.begin()) << std::endl;
  } else {
    std::cout << "no target "<< std::endl;
  }
  // 8 1 9 9 4 2 4 2 5 5 8 6 10 3 5 3 1 8 8 8 
  // 1 1 2 2 3 3 4 4 5 5 5 6 8 8 8 8 8 9 9 10 
  // 去重后的vector内容: 1 2 3 4 5 6 8 9 10 
  // 5 in index 4
}

int main() {
  // func1();
  // func2();
  // std::vector<int> vec{1, 2, 3};
  // func3(vec);
  // func4();
  func5();
  return 0;
}