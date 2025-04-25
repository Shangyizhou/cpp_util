#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Foo {
public:
  static constexpr int kConstValue = 2;

  Foo() {}
  Foo(double fps, double duration_s, int total_frame)
      : fps_(fps), duration_s_(duration_s), total_frame_(total_frame) {
    std::cout << "Init Func" << std::endl;
  }

  double GetFps() const { return fps_; }
  double GetDurationS() const { return duration_s_; }
  int GetTotalFrame() const { return total_frame_; }

private:
  double fps_;
  double duration_s_;
  int total_frame_;
  std::vector<int> frame_info_list_;
};