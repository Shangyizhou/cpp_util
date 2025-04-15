#include "Log/logger.h"
#include "Thread/ThreadPool.h"

// 测试函数
void work() {
    for(int i = 0; i < 100; ++i) {
	if (i == 56) {
	    std::cout << "count is 56!\n\n";
	    std::cout.flush();
	    }
    }
    sleep(1);
}

int main() {
    
    tool::ThreadPool pool(1);
    
    pool.Enqueue(work);
    pool.Start();
    
    sleep(3);
    pool.Stop();
    return(0);
}