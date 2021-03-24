#include "Allocator.hpp"

Allocator::Allocator() : capacity_{0},  size_{0}, offset_{0}, data_{} {}

void Allocator::makeAllocator(size_t maxSize)
{
    if (maxSize > capacity_) {
        data_.reset(new char[maxSize]);
        capacity_ = maxSize;    
    }
    size_ = maxSize;
    offset_ = 0;
}

char* Allocator::alloc(size_t size)
{
    if (offset_ + size > size_ || size == 0) {
        return nullptr;
    }
    size_t old_offset = offset_;
    offset_ += size;
    return data_.get() + old_offset;
}

void Allocator::reset()
{
    offset_ = 0;
}

bool Allocator::isNull() const
{
    return data_.get() == nullptr;
}