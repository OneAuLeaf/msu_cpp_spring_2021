#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>
#include <iterator>

#include "Allocator.hpp"
#include "Iterator.hpp"

template <class T, class AllocatorT = std::allocator<T>>
class Vector
{
public:
    using value_type = T;
    using allocator_type = AllocatorT;
    using size_type = size_t;
    using difference_type  = ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<AllocatorT>::pointer;
    using const_pointer = typename std::allocator_traits<AllocatorT>::const_pointer;
    using iterator = Iterator<T>;
    using const_iterator = Iterator<const T>;
    using reverse_iterator = typename std::reverse_iterator<iterator>;
    using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;

    Vector();
    explicit Vector(const AllocatorT& alloc);
    explicit Vector(size_t size, const AllocatorT& alloc = AllocatorT());
    Vector(size_t size, const T& value, const AllocatorT& alloc = AllocatorT());
    template <class InputIt, class = typename std::iterator_traits<InputIt>::value_type>
    Vector(InputIt first, InputIt last, const AllocatorT& alloc = AllocatorT());
    Vector(std::initializer_list<T> init, const AllocatorT& alloc = AllocatorT());

    Vector(const Vector& other);
    Vector(Vector&& other);

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other);
    Vector& operator=(std::initializer_list<T> init);

    reference operator[](size_t pos);
    const_reference operator[](size_t pos) const;

    bool empty() const { return !size_; }
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }

    void resize(size_t size);
    void reserve(size_t new_cap);

    void clear();
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back();
    template <class... Args>
    reference emplace_back(Args&&... args);

    iterator begin() { return iterator(data_); }
    iterator end() { return iterator(data_ + size_); }
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }

    const_iterator begin() const { return const_iterator(data_); }
    const_iterator end() const { return const_iterator(data_ + size_); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

    ~Vector();
private:
    size_t size_, capacity_;
    AllocatorT allocator_;
    T* data_;
};

#include "Vector.tpp"

#endif
