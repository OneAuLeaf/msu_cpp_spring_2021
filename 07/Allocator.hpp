#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include <cstdlib>

template<class T>
class Allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;

    Allocator() = default;
    Allocator(const Allocator&) = default;
    Allocator(Allocator&&) = default;
    Allocator& operator=(const Allocator&) = default;
    Allocator& operator=(Allocator&&) = default;

    pointer allocate(size_t n) { return static_cast<pointer>(malloc(n * sizeof(value_type))); };
    void deallocate(pointer ptr, size_t n) { free(ptr); };

    ~Allocator() = default;
};

#endif