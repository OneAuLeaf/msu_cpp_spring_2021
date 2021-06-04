#include <fstream>
#include <cstdio>
#include <thread>
#include <string>
#include <algorithm>

#include "ExternSort.hpp"

void splitSeries(const char *filename, const char *series1_name, const char *series2_name, size_t series_len)
{
    std::ifstream file(filename, std::ios::binary);
    std::ofstream series1(series1_name, std::ios::binary | std::ios::trunc);
    std::ofstream series2(series2_name, std::ios::binary | std::ios::trunc);

    uint64_t elem;
    file.read(reinterpret_cast<char *>(&elem), sizeof(elem));
    while (file) {
        for (size_t i = 0; i < series_len && file; ++i) {
            series1.write(reinterpret_cast<char *>(&elem), sizeof(elem));
            file.read(reinterpret_cast<char *>(&elem), sizeof(elem));
        }
        for (size_t i = 0; i < series_len && file; ++i) {
            series2.write(reinterpret_cast<char *>(&elem), sizeof(elem));
            file.read(reinterpret_cast<char *>(&elem), sizeof(elem));
        }
    }

    file.close();
    series1.close();
    series2.close();
}

void createSeries(const char *filename, const char *series1_name, const char *series2_name, const size_t max_memory)
{
    size_t series_len = max_memory / 2;
    std::ifstream file(filename, std::ios::binary);
    std::ofstream series1(series1_name, std::ios::binary | std::ios::trunc);
    std::ofstream series2(series2_name, std::ios::binary | std::ios::trunc);

    uint64_t *buffer = new uint64_t[series_len];
    const size_t buffer_size = series_len * sizeof(uint64_t);
    file.read(reinterpret_cast<char *>(buffer), buffer_size);
    while (file.gcount()) {
        std::sort(buffer, buffer + file.gcount() / sizeof(uint64_t));
        series1.write(reinterpret_cast<char *>(buffer), file.gcount());
        file.read(reinterpret_cast<char *>(buffer), buffer_size);
        if (file.gcount()) {
            std::sort(buffer, buffer + file.gcount() / sizeof(uint64_t));
            series2.write(reinterpret_cast<char *>(buffer), file.gcount());
            file.read(reinterpret_cast<char *>(buffer), buffer_size);
        }
    }

    delete [] buffer;
    file.close();
    series1.close();
    series2.close();
}

void mergeSeries(const char *filename, const char *series1_name, const char *series2_name, size_t series_len)
{
    std::ofstream file(filename, std::ios::binary);
    std::ifstream series1(series1_name, std::ios::binary);
    std::ifstream series2(series2_name, std::ios::binary);

    uint64_t left, right;
    series1.read(reinterpret_cast<char *>(&left), sizeof(left));
    series2.read(reinterpret_cast<char *>(&right), sizeof(right));
    while (series1 && series2) {
        size_t len1 = 0, len2 = 0;
        while (series1 && series2 && len1 < series_len && len2 < series_len) {
            if (left < right) {
                file.write(reinterpret_cast<char *>(&left), sizeof(left));
                series1.read(reinterpret_cast<char *>(&left), sizeof(left));
                ++len1;
            }
            else {
                file.write(reinterpret_cast<char *>(&right), sizeof(right));
                series2.read(reinterpret_cast<char *>(&right), sizeof(right));
                ++len2;
            }
        }
        while (series1 && len1 < series_len) {
            file.write(reinterpret_cast<char *>(&left), sizeof(left));
            series1.read(reinterpret_cast<char *>(&left), sizeof(left));
            ++len1;
        }
        while (series2 && len2 < series_len) {
            file.write(reinterpret_cast<char *>(&right), sizeof(right));
            series2.read(reinterpret_cast<char *>(&right), sizeof(right));
            ++len2;
        }
    }

    while (series1) {
        file.write(reinterpret_cast<char *>(&left), sizeof(left));
        series1.read(reinterpret_cast<char *>(&left), sizeof(left));
    }
    while (series2) {
        file.write(reinterpret_cast<char *>(&right), sizeof(right));
        series2.read(reinterpret_cast<char *>(&right), sizeof(right));
    }

    file.close();
    series1.close();
    series2.close();
}

void externalSort(const char *filename, const size_t max_memory)
{
    std::hash<std::thread::id> hasher;
    std::string series1("series1tmp_" + std::to_string(hasher(std::this_thread::get_id())));
    std::string series2("series2tmp_" + std::to_string(hasher(std::this_thread::get_id())));
    
    size_t file_len = 0;
    std::ifstream file(filename, std::ios::binary);
    auto start = file.tellg();
    file.seekg(0, std::ios::end);
    auto end = file.tellg();
    file_len = (end - start) / sizeof(uint64_t);
    file.close();
    
    size_t series_len = max_memory / 2;
    createSeries(filename, series1.c_str(), series2.c_str(), max_memory);
    while (series_len * 2 < file_len) {
        mergeSeries(filename, series1.c_str(), series2.c_str(), series_len);
        series_len *= 2;
        splitSeries(filename, series1.c_str(), series2.c_str(), series_len);
    }
    mergeSeries(filename, series1.c_str(), series2.c_str(), series_len);

    std::remove(series1.c_str());
    std::remove(series2.c_str());
}

void threadWork(std::ifstream &fin, const char *filename, const size_t max_memory)
{
    std::ofstream tmpfile(filename, std::ios::binary);

    uint64_t *buffer = new uint64_t[max_memory];
    const size_t buffer_size = max_memory * sizeof(uint64_t);
    fin.read(reinterpret_cast<char *>(buffer), buffer_size);
    while (fin.gcount()) {
        fin.seekg(fin.gcount(), std::ios::cur);
        tmpfile.write(reinterpret_cast<char *>(buffer), fin.gcount());
        fin.read(reinterpret_cast<char *>(buffer), buffer_size);
    }
    delete [] buffer;
    tmpfile.close();
    fin.close();

    externalSort(filename, max_memory);
}

void externalThreadSort(const char *file_in, const char *file_out, const size_t max_memory)
{
    const char *tmpfile1 = "tmpfile1.bin";
    const char *tmpfile2 = "tmpfile2.bin";
    const size_t thread_memory = max_memory / 2;
    std::ifstream fin1(file_in, std::ios::binary);
    std::ifstream fin2(file_in, std::ios::binary);
    fin2.seekg(thread_memory * sizeof(uint64_t));

    std::thread thread1(threadWork, std::ref(fin1), tmpfile1, thread_memory);
    std::thread thread2(threadWork, std::ref(fin2), tmpfile2, thread_memory);
    
    thread1.join();
    thread2.join();

    std::ifstream tmpleft(tmpfile1, std::ios::binary);
    std::ifstream tmpright(tmpfile2, std::ios::binary);
    std::ofstream output(file_out, std::ios::binary);

    uint64_t left = 0, right = 0, res;
    tmpleft.read(reinterpret_cast<char *>(&left), sizeof(left));
    tmpright.read(reinterpret_cast<char *>(&right), sizeof(right));
    while (tmpleft || tmpright) {
        if (!tmpleft) {
            res = right;
            tmpright.read(reinterpret_cast<char *>(&right), sizeof(right));
        } else if (!tmpright || left <= right) {
            res = left;
            tmpleft.read(reinterpret_cast<char *>(&left), sizeof(left));
        } else {
            res = right;
            tmpright.read(reinterpret_cast<char *>(&right), sizeof(right));
        }
        output.write(reinterpret_cast<const char *>(&res), sizeof(res));
    }

    tmpleft.close();
    tmpright.close();
    output.close();

    std::remove(tmpfile1);
    std::remove(tmpfile2);
}