#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include <cstdlib>
#include <memory>

class Allocator
{
public:
    Allocator();
    Allocator(const Allocator&) = delete;
    Allocator& operator= (const Allocator&) = delete; 

    size_t capacity() const { return capacity_; }
    size_t size() const { return size_; }
    size_t offset() const { return offset_; }
    
    void makeAllocator(size_t maxSize);
    char* alloc(size_t size);
    void reset();
    bool isNull() const;

    ~Allocator() = default;
private:
    size_t capacity_;
    size_t size_;
    size_t offset_;
    std::unique_ptr<char[]> data_;
};

#endif