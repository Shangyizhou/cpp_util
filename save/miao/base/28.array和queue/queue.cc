#include <queue>
#include <iostream>

void func1() {
    std::queue<int> myQueue;

    myQueue.push(1);
    myQueue.push(2);
    myQueue.push(3);
    myQueue.push(4);
    myQueue.push(5);

    while (!myQueue.empty()) { 
        std::cout << myQueue.front() << " ";
        myQueue.pop();
    }
    std::cout << std::endl;
}

void func2() {
    std::queue<int> queue;

    auto check_func = [&queue](){
        std::cout << "queue.empty(): " << queue.empty() << std::endl;
        std::cout << "queue.size(): " << queue.size() << std::endl;
    };

    queue.push(1);
    check_func();

    queue.push(2);
    check_func();

    queue.push(3);
    check_func();

    queue.pop();
    check_func();

    queue.pop();
    check_func();

    queue.pop();
    check_func();

    // queue.empty(): 0
    // queue.size(): 1
    // queue.empty(): 0
    // queue.size(): 2
    // queue.empty(): 0
    // queue.size(): 3
    // queue.empty(): 0
    // queue.size(): 2
    // queue.empty(): 0
    // queue.size(): 1
    // queue.empty(): 1
    // queue.size(): 0
}

int main() {
    func2();

    return 0;
}