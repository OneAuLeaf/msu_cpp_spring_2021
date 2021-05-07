#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>

template <class T>
class Iterator
    : public std::iterator<std::random_access_iterator_tag, T>
{
public:
    explicit Iterator(T* ptr) : ptr_(ptr) {}

    T& operator*() { return *ptr_; }
    T& operator[](size_t i) { return ptr_[i]; }
    
    Iterator& operator++() { ++ptr_; return *this; }
    Iterator operator++(int) { Iterator retval = *this; ++(*this); return retval; }
    Iterator& operator--() { --ptr_; return *this; }
    Iterator operator--(int) { Iterator retval = *this; --(*this); return retval; }

    Iterator& operator+=(ptrdiff_t n) { ptr_ += n; return *this; }
    Iterator operator+ (ptrdiff_t n) const { Iterator tmp = *this; return tmp += n;}
    Iterator& operator-=(ptrdiff_t n) { ptr_ -= n; return *this; }
    Iterator operator- (ptrdiff_t n) const { Iterator tmp = *this; return tmp -= n;}

    ptrdiff_t operator-(const Iterator& other) const { return std::distance(ptr_, other.ptr_); }

    bool operator==(const Iterator& other) const { return ptr_ == other.ptr_; }
    bool operator!=(const Iterator& other) const { return !(*this == other); }
    bool operator< (const Iterator& other) const { return ptr_ < other.ptr_; }
    bool operator> (const Iterator& other) const { return other < *this; }
    bool operator<=(const Iterator& other) const { return !(other < *this); }
    bool operator>=(const Iterator& other) const { return !(*this < other); }
private:
    T* ptr_;
};

#endif