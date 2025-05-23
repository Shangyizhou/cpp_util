// #pragma once

// namespace tool
// {
//     #pragma once

// #include <functional>
// #include <future>
// #include <mutex>
// #include <queue>
// #include <thread>
// #include <utility>
// #include <vector>

// #include "Thread/thread_safe_queue.h"

// class ThreadPool {
// private:
//   class ThreadWorker {
//   private:
//     int m_id;
//     ThreadPool * m_pool;
//   public:
//     ThreadWorker(ThreadPool * pool, const int id)
//       : m_pool(pool), m_id(id) {
//     }

//     void operator()() {
//       std::function<void()> func;
//       bool dequeued;
//       while (!m_pool->m_shutdown) {
//         {
//           std::unique_lock<std::mutex> lock(m_pool->m_conditional_mutex);
//           if (m_pool->m_queue.empty()) {
//             m_pool->m_conditional_lock.wait(lock);
//           }
//           dequeued = m_pool->m_queue.dequeue(func);
//         }
//         if (dequeued) {
//           func();
//         }
//       }
//     }
//   };

//   bool m_shutdown;
//   ThreadSafeQueue<std::function<void()>> m_queue;
//   std::vector<std::thread> m_threads;
//   std::mutex m_conditional_mutex;
//   std::condition_variable m_conditional_lock;
// public:
//   ThreadPool(const int n_threads)
//     : m_threads(std::vector<std::thread>(n_threads)), m_shutdown(false) {
//   }

//   ThreadPool(const ThreadPool &) = delete;
//   ThreadPool(ThreadPool &&) = delete;

//   ThreadPool & operator=(const ThreadPool &) = delete;
//   ThreadPool & operator=(ThreadPool &&) = delete;

//   // Inits thread pool
//   void init() {
//     for (int i = 0; i < m_threads.size(); ++i) {
//       m_threads[i] = std::thread(ThreadWorker(this, i));
//     }
//   }

//   // Waits until threads finish their current task and shutdowns the pool
//   void shutdown() {
//     m_shutdown = true;
//     m_conditional_lock.notify_all();
    
//     for (int i = 0; i < m_threads.size(); ++i) {
//       if(m_threads[i].joinable()) {
//         m_threads[i].join();
//       }
//     }
//   }

//   // Submit a function to be executed asynchronously by the pool
//   template<typename F, typename...Args>
//   // 作用：推导函数 f 在传入参数 args... 时的返回类型。
//   // ​原理：decltype 会在编译时分析表达式 f(args...) 的类型，但不会实际执行该函数调用
//   auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
//     // Create a function with bounded parameters ready to execute
//     // 绑定参数并类型擦除为 std::function
//     std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
//     // Encapsulate it into a shared ptr in order to be able to copy construct / assign 
//     //  封装为 packaged_task 以获取 future
//     auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

//     // Wrap packaged task into void function
//     std::function<void()> wrapper_func = [task_ptr]() {
//       (*task_ptr)(); 
//     };

//     // Enqueue generic wrapper function
//     m_queue.enqueue(wrapper_func);

//     // Wake up one thread if its waiting
//     m_conditional_lock.notify_one();

//     // Return future from promise
//     return task_ptr->get_future();
//   }
// };
    
// } // namespace tool
