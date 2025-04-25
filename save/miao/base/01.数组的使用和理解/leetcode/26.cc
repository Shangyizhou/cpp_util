// https://leetcode.cn/problems/plus-one/description/
#include <iostream>
#include <vector>
#include <algorithm>

// 思路想错了，想先转化为数字+1然后每位数取余加入数组再反转，但是溢出问题
void func1() {
    std::vector<int> input = {9,8,7,6,5,4,3,2,1,0};
    long long sum = 0;
    for (int i = 0; i < input.size(); i++) {
        sum = sum * 10 + input[i];
    }

    std::cout << "current sum is "<< sum << std::endl;

    sum += 1;

    std::vector<int> output;
    while (sum > 0) {
        int n = sum % 10;
        sum /= 10;
        output.push_back(n);
    }
    
    std::cout << "result is" << std::endl;
    std::reverse(output.begin(), output.end());
    for (const auto& i : output) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void ShowVector(const std::vector<int>& vec) {
    for (const auto& i : vec) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

// 用数组的做法反而正确，倒叙遍历如果不是 9 则直接加 1，如果是9则进位变0，继续倒叙遍历
// 如果一直到第一个元素都为9则会退出循环，此时需要重新赋值数组，首位设置为1
std::vector<int> func2() {
    std::vector<int> input = {9};
    ShowVector(input);
    for (int i = input.size() - 1; i >= 0; i--) {
        if (input[i] != 9) {
            input[i]++;
            return input;
        }
        input[i] = 0;
    }
    std::vector<int> output(input.size() + 1);
    output[0] = 1;
    return output;
}

int main() {
    auto res = func2();
    ShowVector(res);
}