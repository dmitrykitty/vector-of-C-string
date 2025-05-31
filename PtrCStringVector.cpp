#include <functional>
#include <algorithm>
#include <string>
#include <cstring>
#include <stdexcept>
#include <utility> // std::exchange
#include "PtrCStringVector.h"

#include <iostream>

char* sumCString(const char* str1, const char* str2) {
    if (!str1)
        str1 = "";
    if (!str2)
        str2 = "";
    //sprawdzenie na nullptr

    std::size_t lenStr1 = strlen(str1);
    std::size_t lenStr2 = strlen(str2);

    char* res = new char[lenStr1 + lenStr2 + 1];
    memcpy(res, str1, lenStr1);
    memcpy(res + lenStr1, str2, lenStr2 + 1);

    return res;
}

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

PtrCStringVector PtrCStringVector::operator&(const PtrCStringVector& other) const {
    PtrCStringVector res;
    res.reserve(std::max(size_, other.size_));

    std::size_t index = 0;

    while (index < size_ && index < other.size_) {
        try {
            char* stringSum = sumCString(data_[index], other.data_[index]);
            res.push_back(stringSum);
            delete[] stringSum;
        } catch (const std::bad_alloc& e) {
            std::cerr << "operator&: memory allocate error at index " << index << ": " << e.what() << "\n";
            return {};
        }
        index++;
    }

    while (index < size_) {
        res.push_back(data_[index]);
        ++index;
    }

    while (index < other.size()) {
        res.push_back(other.data_[index]);
        ++index;
    }
    return res;
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
