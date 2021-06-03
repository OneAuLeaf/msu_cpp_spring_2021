#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>


class ThreadPool
{
    using Task = std::function<void()>;

    void threadWork()
    {
        while (!done_) {
            std::unique_lock<std::mutex> lock(mutex_work_);
            if (!tasks_.empty()) {
                auto task = std::move(tasks_.front());
                tasks_.pop();

                lock.unlock();

                task();
            } else {
                has_work_.wait(lock);
            }
        }
    }
public:    
    explicit ThreadPool(size_t pool_size = std::thread::hardware_concurrency())
        : pool_size_{pool_size}, done_{false}
    {
        for (size_t i = 0; i < pool_size_; ++i) {
            threads_.emplace_back(&ThreadPool::threadWork, this);
        }
    }

    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))>
    {
        auto wrapper = std::make_shared<std::packaged_task<decltype(func(args...))()>>(std::bind(func, args...));
        auto res = wrapper->get_future();
        {
            std::lock_guard<std::mutex> lock(mutex_work_);
            tasks_.emplace([wrapper]() { (*wrapper)(); });
        }
        has_work_.notify_one();
        return res;
    }

    size_t poolSize() { return pool_size_; }

    ~ThreadPool()
    {
        done_ = true;
        has_work_.notify_all();
        for (size_t i = 0; i < pool_size_; ++i) {
            if (threads_[i].joinable()) {
                threads_[i].join();
            }
        }
    }
private:
    size_t pool_size_;
    std::atomic_bool done_;
    std::condition_variable has_work_;
    std::mutex mutex_work_;
    std::vector<std::thread> threads_;
    std::queue<Task> tasks_;
};

#endif 