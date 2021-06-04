#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#include "ExternSort.hpp"

// ограничение памяти
const size_t max_memory = 2 * 1024 * 1024 / sizeof(uint64_t);

// создаем случайный файл
void generate(const char *filename, size_t len)
{
    std::srand(std::time(nullptr));
    std::ofstream file(filename, std::ios::binary);
    for (size_t i = 0; i < len; ++i) {
        uint64_t elem = std::rand();
        file.write(reinterpret_cast<char *>(&elem), sizeof(elem));
    }
    file.close();
}

// проверка файла с выводом времени работы
void test()
{
    const size_t test_len = 1'000'000;
    generate("testfile.bin", test_len);

    clock_t tik = std::clock();
    externalThreadSort("testfile.bin", "sortedfile.bin", max_memory);
    clock_t tak = std::clock();

    double elapsed = static_cast<double>(tak - tik) / CLOCKS_PER_SEC;
    std::cout << "Time: " << elapsed << " seconds" << std::endl;

    std::ifstream file("sortedfile.bin", std::ios::binary);
    bool sorted = true;
    size_t len = 0;
    uint64_t prev = 0, curr;
    while (file.read(reinterpret_cast<char *>(&curr), sizeof(curr))) {
        if (curr < prev) {
            sorted = false;
            break;
        }
        ++len;
        prev = curr;
    }
    file.close();
    assert(sorted && len == test_len);
    
    std::remove("testfile.bin");
    std::remove("sortedfile.bin");
}

int main() 
{
    test();
}