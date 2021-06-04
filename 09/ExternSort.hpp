#ifndef EXTERNSORT_HPP
#define EXTERNSORT_HPP

#include <fstream>

// алгоритм внешней сортировки
void splitSeries(const char *filename, const char *line1_name, const char *line2_name, size_t series_len);
void createSeries(const char *filename, const char *line1_name, const char *line2_name, const size_t max_memory);
void mergeSeries(const char *filename, const char *line1_name, const char *line2_name, size_t series_len);
void externalSort(const char *path, const size_t max_memory);

// сортировка в 2 потока
void threadWork(std::ifstream &fin, const char *filename, const size_t max_memory);
void externalThreadSort(const char *file_in, const char *file_out, const size_t max_memory);

#endif