#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>

template <class T>
class Iterator
{
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    explicit Iterator(pointer ptr) : ptr_(ptr) {}

    Iterator(const Iterator&) = default;
    Iterator(Iterator&&) = default;
    Iterator& operator=(const Iterator&) = default;
    Iterator& operator=(Iterator&&) = default;

    reference operator*() const { return *ptr_; }
    pointer operator->() const { return ptr_; }
    reference operator[](size_t i) const { return ptr_[i]; }
    
    Iterator& operator++() { ++ptr_; return *this; }
    Iterator operator++(int) { Iterator retval = *this; ++(*this); return retval; }
    Iterator& operator--() { --ptr_; return *this; }
    Iterator operator--(int) { Iterator retval = *this; --(*this); return retval; }

    Iterator& operator+=(difference_type n) { ptr_ += n; return *this; }
    Iterator operator+ (difference_type n) const { Iterator tmp = *this; return tmp += n;}
    
    friend Iterator operator+(difference_type n, const Iterator& self) { Iterator tmp = self; return tmp + n; }

    Iterator& operator-=(difference_type n) { ptr_ -= n; return *this; }
    Iterator operator- (difference_type n) const { Iterator tmp = *this; return tmp -= n;}

    difference_type operator-(const Iterator& other) const { return std::distance(other.ptr_, ptr_); }

    bool operator==(const Iterator& other) const { return ptr_ == other.ptr_; }
    bool operator!=(const Iterator& other) const { return !(*this == other); }
    bool operator< (const Iterator& other) const { return ptr_ < other.ptr_; }
    bool operator> (const Iterator& other) const { return other < *this; }
    bool operator<=(const Iterator& other) const { return !(other < *this); }
    bool operator>=(const Iterator& other) const { return !(*this < other); }
private:
    pointer ptr_;
};

#endif