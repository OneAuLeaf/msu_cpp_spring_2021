#include <cassert>
#include <thread>

#include "ThreadPool.hpp"

struct A {};

void foo(const A&) {}

// тест из условия
void defaultTest()
{
    ThreadPool pool(8);

    auto task1 = pool.exec(foo, A());
    task1.get();

    auto task2 = pool.exec([]() { return 1; });
    assert(task2.get() == 1);
}

// значение потоков по умолчанию
void defaultPoolNumberUse()
{
    ThreadPool pool; // hardware_concurrency используется
    assert(pool.poolSize() == std::thread::hardware_concurrency());

    auto task1 = pool.exec([]() { return 1; });
    assert(task1.get() == 1);
}

// больше задач
void moreTasksUse()
{
    ThreadPool pool;
    for (size_t i = 0; i < 30; ++i) {
        auto task = pool.exec([]() { return 42; });
        assert(task.get() == 42);
    }
}

// выбрасывание исключений в задаче
void exceptionUse()
{
    ThreadPool pool(2);

    auto task1 = pool.exec([]() { return 1000 - 7; });
    auto task2 = pool.exec([]() { throw std::runtime_error("Test error"); });
    try {
        task2.get();
        assert(false);
    } catch (const std::runtime_error& e) {
        assert(std::string(e.what()) == "Test error");
    } catch (...) {
        assert(false);
    }
    assert(task1.get() == 1000 - 7);
}

int main()
{
    defaultTest();
    defaultPoolNumberUse();
    moreTasksUse();
    exceptionUse();
}