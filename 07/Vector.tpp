#include <algorithm>
#include <stdexcept>

template <class T, class AllocatorT>
Vector<T, AllocatorT>::Vector()
        : size_{0}, capacity_{0}, allocator_{AllocatorT()}, data_{nullptr} {}

template <class T, class AllocatorT>
Vector<T, AllocatorT>::Vector(const AllocatorT& alloc)
        : size_{0}, capacity_{0}, allocator_{alloc}, data_{nullptr} {}

template <class T, class AllocatorT>
Vector<T, AllocatorT>::Vector(size_t size, const AllocatorT& alloc)
        : size_{size}, capacity_{size}, allocator_{alloc}
{
    data_ = allocator_.allocate(size);
    for (size_t i = 0; i < size_; ++i) {
        new (data_ + i) T();
    }
}

template <class T, class AllocatorT>
Vector<T, AllocatorT>::Vector(size_t size, const T& value, const AllocatorT& alloc)
        : size_{size}, capacity_{size}, allocator_{alloc}
{
    data_ = allocator_.allocate(size);
    for (size_t i = 0; i < size_; ++i) {
        new (data_ + i) T(value);
    }
}

template <class T, class AllocatorT>
template <class InputIt, class>
Vector<T, AllocatorT>::Vector(InputIt first, InputIt last, const AllocatorT& alloc)
        : size_{static_cast<Vector<T, AllocatorT>::size_type>(std::distance(first, last))}, 
          capacity_{static_cast<Vector<T, AllocatorT>::size_type>(std::distance(first, last))}, allocator_{alloc}
{
    data_ = allocator_.allocate(size_);
    auto it = first;
    for (size_t i = 0; i < size_; ++i) {
        new (data_ + i) T(*it++);
    }
}

template <class T, class AllocatorT>
Vector<T, AllocatorT>::Vector(std::initializer_list<T> init, const AllocatorT& alloc)
        : size_{init.size()}, capacity_{init.size()}, allocator_{alloc}
{
    data_ = allocator_.allocate(size_);
    auto it = init.begin();
    for (size_t i = 0; i < size_; ++i) {
        new (data_ + i) T(*it++);
    }
}
template <class T, class AllocatorT>
Vector<T, AllocatorT>::Vector(const Vector& other)
        : size_{other.size_}, capacity_{other.capacity_}, allocator_{other.allocator_}
{
    data_ = allocator_.allocate(capacity_);
    for (size_t i = 0; i < size_; ++i) {
        new (data_ + i) T(other.data_[i]);
    }
}

template <class T, class AllocatorT>
Vector<T, AllocatorT>::Vector(Vector&& other)
        : size_{other.size_}, capacity_{other.capacity_}, allocator_{other.allocator_}, data_{other.data_}
{
    other.size_ = 0;
    other.capacity_ = 0;
    other.data_ = nullptr;
}

template <class T, class AllocatorT>
Vector<T, AllocatorT>& Vector<T, AllocatorT>::operator=(const Vector& other)
{
    if (this == &other) {
        return *this;
    }

    reserve(other.capacity_);
    resize(other.size_);
    std::copy(other.data_, other.data_ + other.size_, data_);

    return *this;
}

template <class T, class AllocatorT>
Vector<T, AllocatorT>& Vector<T, AllocatorT>::operator=(Vector&& other)
{
    if (this == &other) {
        return *this;
    }

    reserve(other.capacity_);
    resize(other.size_);
    std::move(other.data_, other.data_ + other.size_, data_);

    return *this;
}

template <class T, class AllocatorT>
Vector<T, AllocatorT>& Vector<T, AllocatorT>::operator=(std::initializer_list<T> init)
{
    resize(init.size());
    std::copy(init.begin(), init.end(), data_);
    return *this;
}

template <class T, class AllocatorT>
typename Vector<T, AllocatorT>::reference Vector<T, AllocatorT>::operator[](size_t pos)
{
    if (pos >= size_) {
        throw std::out_of_range("Index " + std::to_string(pos) + " out of range " + std::to_string(size_));
    }
    return data_[pos];
}

template <class T, class AllocatorT>
typename Vector<T, AllocatorT>::const_reference Vector<T, AllocatorT>::operator[](size_t pos) const
{
    if (pos >= size_) {
        throw std::out_of_range("Index " + std::to_string(pos) + " out of range " + std::to_string(size_));
    }
    return data_[pos];
}

template <class T, class AllocatorT>
void Vector<T, AllocatorT>::resize(size_t size)
{
    if (size > capacity_) {
        reserve(size);
    }

    if (size > size_) {
        for (size_t i = size_; i < size; ++i) {
            new (data_ + i) T();
        }
    } else if (size < size_) {
        std::destroy(data_ + size, data_ + size_);
    }
    size_ = size;
}

template <class T, class AllocatorT>
void Vector<T, AllocatorT>::reserve(size_t new_cap)
{
    if (new_cap <= capacity_) {
        return;
    }
    auto tmp = allocator_.allocate(new_cap);
    for (size_t i = 0; i < size_; ++i) {
        new (tmp + i) T(std::move(data_[i]));
    }
    std::destroy(data_, data_ + size_);
    allocator_.deallocate(data_, capacity_);
    capacity_ = new_cap;
    data_ = tmp;
}

template <class T, class AllocatorT>
void Vector<T, AllocatorT>::clear()
{
    std::destroy(data_, data_ + size_);
    size_ = 0;
}

template <class T, class AllocatorT>
void Vector<T, AllocatorT>::push_back(const T& value)
{
    if (size_ + 1 > capacity_) {
        reserve(capacity_ * 2 + 1);
    }
    new (data_ + size_) T(value);
    ++size_;
}

template <class T, class AllocatorT>
void Vector<T, AllocatorT>::push_back(T&& value)
{
    if (size_ + 1 > capacity_) {
        reserve(capacity_ * 2 + 1);
    }
    new (data_ + size_) T(std::move(value));
    ++size_;
}

template <class T, class AllocatorT>
void Vector<T, AllocatorT>::pop_back()
{
    if (!size_) {
        throw std::out_of_range("Vector is empty");
    }
    data_[size_ - 1].~T();
    --size_;
}

template <class T, class AllocatorT>
template <class... Args>
typename Vector<T, AllocatorT>::reference Vector<T, AllocatorT>::emplace_back(Args&&... args)
{
    if (size_ + 1 > capacity_) {
        reserve(capacity_ * 2 + 1);
    }
    new (data_ + size_) T(std::forward<Args>(args)...);
    return data_[size_++];
}

template <class T, class AllocatorT>
Vector<T, AllocatorT>::~Vector()
{
    std::destroy(data_, data_ + size_);
    size_ = 0;
    allocator_.deallocate(data_, capacity_);
}