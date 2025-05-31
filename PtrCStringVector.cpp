#include <functional>
#include <algorithm>
#include <string>
#include <cstring>
#include <stdexcept>
#include <utility> // std::exchange
#include "PtrCStringVector.h"

#include <iostream>
using namespace std;


PtrCStringVector::PtrCStringVector(): size_(0), capacity_(0), data_(nullptr) {}


PtrCStringVector::PtrCStringVector(const PtrCStringVector& other): size_(other.size_), capacity_(other.capacity_)
                                                                   , data_(nullptr) {
    if (other.data_) {
        data_ = new char *[capacity_];
        for (std::size_t i = 0; i < size_; ++i) {
            std::size_t stringLen = strlen(other.data_[i]) + 1;
            data_[i] = new char[stringLen];
            memcpy(data_[i], other.data_[i], stringLen);
        }
    }
}

PtrCStringVector::~PtrCStringVector() {
    free();
}

PtrCStringVector& PtrCStringVector::operator=(const PtrCStringVector& other) {
    if (this == &other)
        return *this;

    free();
    if (other.data_) {
        data_ = new char *[other.capacity_];
        for (std::size_t i = 0; i < other.size_; ++i) {
            std::size_t stringLen = strlen(other.data_[i]) + 1;
            data_[i] = new char[stringLen];
            memcpy(data_[i], other.data_[i], stringLen);
        }
        size_ = other.size_;
        capacity_ = other.capacity_;
    }
    return *this;
}


PtrCStringVector& PtrCStringVector::operator=(PtrCStringVector&& other) noexcept {
    if (this == &other)
        return *this;

    free();

    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;

    other.data_ = nullptr;
    other.capacity_ = other.size_ = 0;

    return *this;
}

void PtrCStringVector::push_back(const char* text2Add) {
    if (!text2Add)
        return;

    try {
        if (size_ >= capacity_)
            reserve(capacity_ ? capacity_ * 2 : 1);

        std::size_t stringLen = strlen(text2Add) + 1;
        data_[size_] = new char[stringLen];
        memcpy(data_[size_], text2Add, stringLen);
        ++size_;
    } catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocate error: " << e.what() << std::endl;
    }
}

PtrCStringVector PtrCStringVector::operator+(const PtrCStringVector& other) const {
    PtrCStringVector res;
    res.reserve(size_ + other.size_);

    for (std::size_t i = 0; i < size_; ++i)
        res.push_back(data_[i]);

    for (std::size_t j = 0; j < other.size_; ++j)
        res.push_back(other.data_[j]);

    return res;
}

char* PtrCStringVector::operator[](std::size_t index) {
    if (index >= size_)
        throw std::out_of_range("PtrCStringVector::operator[]: invalid index");
    return data_[index];
}

const char* PtrCStringVector::operator[](std::size_t index) const {
    if (index >= size_)
        throw std::out_of_range("PtrCStringVector::operator[](const): invalid index");
    return data_[index];
}

PtrCStringVector PtrCStringVector::operator&(const PtrCStringVector& rhs) const {
    /// @todo zaimplementuj, szczegoly w pliku naglowkowym
    return {};
}

void PtrCStringVector::free() {
    if (data_) {
        for (std::size_t i = 0; i < size_; ++i)
            delete[] data_[i];

        delete[] data_;
        data_ = nullptr;
        size_ = capacity_ = 0;
    }
}

void PtrCStringVector::reserve(std::size_t newCapacity) {
    if (newCapacity < capacity_)
        return;

    char** newData = new char *[newCapacity];
    for (std::size_t i = 0; i < size_; ++i)
        newData[i] = data_[i];

    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
}
