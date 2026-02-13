#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>
#include <stdexcept>
#include <memory>
#include <initializer_list>
#include <algorithm>

template<typename T>
class Vector {
public:
    class ConstIterator;
    class Iterator;

    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = Vector::Iterator;
    using const_iterator = Vector::ConstIterator;

private:
    static constexpr size_type min_capacity = 5;
    size_type sz{0};
    size_type max_sz{min_capacity};
    std::unique_ptr<value_type[]> values{std::make_unique<value_type[]>(min_capacity)};

public:
    Vector() = default;

    explicit Vector(size_type n) : sz(0), max_sz(std::max(n, min_capacity)), values(std::make_unique<value_type[]>(max_sz)) {}

    Vector(std::initializer_list<value_type> init)
        : sz(init.size()), max_sz(std::max(init.size(), min_capacity)), values(std::make_unique<value_type[]>(max_sz)) {
        std::copy(init.begin(), init.end(), values.get());
    }

    Vector(const Vector& other)
        : sz(other.sz), max_sz(other.max_sz), values(std::make_unique<value_type[]>(other.max_sz)) {
        std::copy(other.values.get(), other.values.get() + other.sz, values.get());
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            sz = other.sz;
            max_sz = other.max_sz;
            values = std::make_unique<value_type[]>(other.max_sz);
            std::copy(other.values.get(), other.values.get() + other.sz, values.get());
        }
        return *this;
    }

    ~Vector() = default;

    size_type size() const { return sz; }
    size_type capacity() const { return max_sz; }
    bool empty() const { return sz == 0; }
    void clear() { sz = 0; }

    void reserve(size_type n) {
        if (n <= max_sz) return;
        auto new_values = std::make_unique<value_type[]>(n);
        for (size_type i = 0; i < sz; ++i) new_values[i] = values[i];
        values = std::move(new_values);
        max_sz = n;
    }

    void shrink_to_fit() {
        if (sz == max_sz) return;
        auto new_values = std::make_unique<value_type[]>(std::max(sz, min_capacity));
        for (size_type i = 0; i < sz; ++i) new_values[i] = values[i];
        values = std::move(new_values);
        max_sz = std::max(sz, min_capacity);
    }

    void push_back(value_type x) {
        if (sz == max_sz) reserve(max_sz * 2);
        values[sz++] = x;
    }

    void pop_back() {
        if (sz == 0) throw std::runtime_error("Cannot pop from empty vector");
        --sz;
    }

    reference operator[](size_type i) {
        if (i >= sz) throw std::runtime_error("Index out of bounds");
        return values[i];
    }

    const_reference operator[](size_type i) const {
        if (i >= sz) throw std::runtime_error("Index out of bounds");
        return values[i];
    }

    iterator begin() { return iterator(values.get()); }
    iterator end() { return iterator(values.get() + sz); }
    const_iterator begin() const { return const_iterator(values.get()); }
    const_iterator end() const { return const_iterator(values.get() + sz); }

    iterator insert(const_iterator pos, const_reference val) {
        auto diff = pos - begin();
        if (diff < 0 || static_cast<size_type>(diff) > sz)
            throw std::runtime_error("Iterator out of bounds");
        size_type current{static_cast<size_type>(diff)};
        if (sz >= max_sz)
            reserve(max_sz * 2); 
        for (auto i{sz}; i-- > current;)
            values[i + 1] = values[i];
        values[current] = val;
        ++sz;
        return iterator{values.get() + current};
    }

    iterator erase(const_iterator pos) {
        auto diff = pos - begin();
        if (diff < 0 || static_cast<size_type>(diff) >= sz)
            throw std::runtime_error("Iterator out of bounds");
        size_type current{static_cast<size_type>(diff)};
        for (auto i{current}; i < sz - 1; ++i)
            values[i] = values[i + 1];
        --sz;
        return iterator{values.get() + current};
    }

    class Iterator {
    public:
        using value_type = Vector::value_type;
        using reference = Vector::reference;
        using pointer = Vector::pointer;
        using difference_type = Vector::difference_type;
        using iterator_category = std::random_access_iterator_tag;

    private:
        pointer ptr{nullptr};

    public:
        Iterator() = default;
        explicit Iterator(pointer p) : ptr(p) {}

        reference operator*() const { return *ptr; } 
        pointer operator->() const { return ptr; }

        Iterator& operator++() { 
            ++ptr; 
            return *this; } 
        Iterator operator++(int) { 
            Iterator temp = *this; 
            ++ptr;
            return temp; } 
        
        bool operator==(const ConstIterator& other) const { return ptr == other.ptr; } //If they point to the same element, return true
        bool operator!=(const ConstIterator& other) const { return ptr != other.ptr; }

        //Implicit conversion of the Iterator type to a ConstIterator when passed to a function
        operator ConstIterator() const { return ConstIterator(ptr); }
    };

    class ConstIterator {
    public:
        using value_type = Vector::value_type;
        using reference = Vector::const_reference;
        using pointer = Vector::const_pointer;
        using difference_type = Vector::difference_type;
        using iterator_category = std::random_access_iterator_tag; 

    private:
        pointer ptr{nullptr};

    public:
        ConstIterator() = default;
        explicit ConstIterator(pointer p) : ptr(p) {}

        reference operator*() const { return *ptr; }
        pointer operator->() const { return ptr; }

        ConstIterator& operator++() { 
            ++ptr; 
            return *this; }
        ConstIterator operator++(int) { 
            ConstIterator temp = *this; 
            ++ptr; 
            return temp; }

        bool operator==(const ConstIterator& other) const { return ptr == other.ptr; }
        bool operator!=(const ConstIterator& other) const { return ptr != other.ptr; }

        friend difference_type operator-(const ConstIterator& lop, const ConstIterator& rop) {
            return lop.ptr - rop.ptr;
        }

        friend class Iterator;
    };
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& vec) {
    os << "[";
    for (std::size_t i = 0; i < vec.size(); ++i) {
        os << vec[i];
        if (i + 1 < vec.size()) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

#endif