#include <cstdlib>

#include "BigInt.hpp"

void BigInt::reallocate(size_t new_size)
{
    if (new_size > size_) {
        int32_t *new_data =  new int32_t[new_size];
        std::copy(data_, data_ + size_, new_data);
        for (size_t i = size_; i < new_size; ++i) {
            new_data[i] = 0;
        }
        delete [] data_;
        data_ = new_data;
    } else if (new_size < size_) {
        int32_t *new_data =  new int32_t[new_size];
        std::copy(data_, data_ + new_size, new_data);
        delete [] data_;
        data_ = new_data;
    }
    size_ = new_size;
}

void BigInt::shrink()
{
    size_t new_size = size_;
    while (new_size && !data_[new_size - 1]) {
        --new_size;
    }
    if (!new_size) {
        sign_ = 0;
    }
    reallocate(new_size);
}

BigInt::BigInt(int32_t num)
{
    if (!num) {
        size_ = 0;
        sign_ = 0;
        data_ = new int32_t[0];
    } else {
        sign_ = num > 0 ? 1 : -1;
        int64_t abs_num = sign_ * ((int64_t) num);
        if (abs_num >= mod) {
            data_ = new int32_t[2];
            size_ = 2;
            data_[0] = abs_num % mod;
            data_[1] = abs_num / mod;
        } else {
            data_ = new int32_t[1];
            size_ = 1;
            data_[0] = abs_num;
        }
    }
}

BigInt::BigInt(const std::string& str)
{
    bool has_sign = str[0] == '-' || str[0] == '+';
    sign_ = str[0] == '-' ? -1 : 1;
    size_t shift = has_sign;
    while (shift < str.length() && str[shift] == '0') {
        ++shift;
    }
    size_ = (str.length() - shift) / base + ((str.length() - shift) % base != 0);
    data_ = new int32_t[size_];
    if (size_) {
        size_t j = str.length();
        for (size_t i = 0; i < size_ - 1 && j > base + shift; ++i) {
            char* eptr;
            auto res = strtol(str.substr(j-=base, base).c_str(), &eptr, 10);
            data_[i] = (int32_t) res;
        }
        char* eptr;
        auto res = strtol(str.substr(shift, j - shift).c_str(), &eptr, 10);
        data_[size_ - 1] = (int32_t) res;
    } else {
        sign_ = 0;
    }
}

BigInt::BigInt(const BigInt& other) : size_{other.size_}, sign_{other.sign_}
{
    data_ = new int32_t[other.size_];
    std::copy(other.data_, other.data_ + other.size_, data_);
}

BigInt& BigInt::operator= (const BigInt& other)
{
    if (this != &other) {
        delete[] data_;
        size_ = 0;
        sign_ = 0;

        data_ = new int32_t[other.size_];
        size_ = other.size_;
        sign_ = other.sign_;
        std::copy(other.data_, other.data_ + other.size_, data_);
    }

    return *this;
}
    
BigInt::BigInt(BigInt&& other) : size_{other.size_}, sign_{other.sign_}, data_{other.data_}
{
    other.size_ = 0;
    other.sign_ = 0;
    other.data_ = new int32_t[0];
}

BigInt& BigInt::operator= (BigInt&& other)
{
    if (this != &other) {
        delete [] data_;

        size_ = other.size_;
        sign_ = other.sign_;
        data_ = other.data_;

        other.size_ = 0;
        other.sign_ = 0;
        other.data_ = new int32_t[0];
    }

    return *this;
}

BigInt::~BigInt()
{
    delete [] data_;
}

BigInt BigInt::operator- () const
{
    BigInt tmp = BigInt(*this);
    tmp.sign_ = -tmp.sign_;
    return tmp;
}

BigInt& BigInt::operator+=(const BigInt& other)
{
    if (sign_ == 0) {
        *this = other;
        return *this;
    }
    if (other.sign_ == 0) {
        return *this;
    }
    if (sign_ != other.sign_) {
        return *this -= -other;
    }

    size_t new_size = (size_ > other.size_ ? size_ : other.size_) + 1;
    reallocate(new_size);
    int32_t carry  = 0;
    for (size_t i = 0; i < other.size_ || carry; ++i) {
        int32_t res = data_[i] + carry;
        if (i < other.size_) {
            res += other.data_[i];
        }
        data_[i] = res % mod;
        carry = res / mod;
    }
    shrink();
    return *this;
}

BigInt& BigInt::operator-=(const BigInt& other)
{
    if (sign_ == 0) {
        *this = -other;
        return *this;
    }
    if (other.sign_ == 0) {
        return *this;
    }
    if (sign_ != other.sign_) {
        return *this += -other;
    }
    if (other > *this) {
        *this = -(other - *this);
        return *this;
    }
    size_t new_size = (size_ > other.size_ ? size_ : other.size_);
    reallocate(new_size);
    for (size_t i = 0; i < other.size_; ++i) {
        int32_t res = data_[i] - other.data_[i];
        if (res < 0) {
            --data_[i + 1];
        }
        data_[i] = (mod + res) % mod;
    }
    shrink();
    return *this;
}

BigInt& BigInt::operator*=(const BigInt& other)
{
    if (sign_ == 0) {
        return *this;
    }
    if (other.sign_ == 0) {
        *this = other;
        return *this;
    }
    size_t new_size = size_ + other.size_;
    int32_t *new_data = new int32_t[new_size];
    for (size_t i = 0; i < new_size; ++i) {
        new_data[i] = 0;
    }
    for (size_t i = 0; i < size_; ++i) {
        int32_t carry = 0;
        for (size_t j = 0; j < other.size_ || carry; ++j) {
            int64_t res = new_data[i + j] + carry;
            if (j < other.size_) {
                res += ((int64_t) data_[i]) * other.data_[j];
            }
            new_data[i + j] = res % mod;
            carry = res / mod;
        }
    }
    delete [] data_;
    data_ = new_data;
    size_ = new_size;
    sign_ *= other.sign_;
    shrink();
    return *this;
}

std::ostream& operator<<(std::ostream& out, const BigInt& num)
{
    if (!num.sign_) {
        out << 0;
        return out;
    }
    if (num.sign_ < 0) {
        out << '-';
    }
    std::streamsize prev_width = out.width();
    char prev_fiil = out.fill();
    for (size_t i = num.size_ - 1; i; --i) {
        out << num.data_[i];
        out.width(num.base);
        out.fill('0');
    }
    out << num.data_[0];
    out.width(prev_width);
    out.fill(prev_fiil);
    return out;
}

BigInt operator+ (const BigInt& num1, const BigInt& num2)
{
    BigInt res = num1;
    res += num2;
    return res;
}

BigInt operator- (const BigInt& num1, const BigInt& num2)
{
    BigInt res = num1;
    res -= num2;
    return res;
}

BigInt operator* (const BigInt& num1, const BigInt& num2)
{
    BigInt res = num1;
    res *= num2;
    return res;
}

bool operator==(const BigInt& num1, const BigInt& num2)
{
    if (!num1.sign_ && !num2.sign_) {
        return true;
    }
    if (num1.sign_ != num2.sign_ || num1.size_ != num2.size_) {
        return false;
    }
    size_t i = num1.size_ - 1;
    while (i && num1.data_[i] == num2.data_[i]) {
        --i;
    }
    return num1.data_[i] == num2.data_[i];
}

bool operator!=(const BigInt& num1, const BigInt& num2)
{
    return !(num1 == num2);
}

bool operator<(const BigInt& num1, const BigInt& num2)
{
    if (num1.sign_ < num2.sign_) {
        return true;
    }
    if (num1.sign_ > num2.sign_ || !num1.sign_) {
        return false;
    }
    if ((num1.size_ < num2.size_ && num1.sign_ > 0)
            || (num1.size_ > num2.size_ && num1.sign_ < 0)) {
        return true;
    }
    if ((num1.size_ > num2.size_ && num1.sign_ > 0)
            || (num1.size_ < num2.size_ && num1.sign_ < 0)) {
        return false;
    }
    size_t i = num1.size_ - 1;
    while (i && num1.data_[i] == num2.data_[i]) {
        --i;
    }
    if (num1.sign_ > 0) {
        return num1.data_[i] < num2.data_[i];
    }
    else {
        return num1.data_[i] > num2.data_[i];
    }
}

bool operator>(const BigInt& num1, const BigInt& num2)
{
    return num2 < num1;
}

bool operator>=(const BigInt& num1, const BigInt& num2)
{
    return !(num1 < num2);
}

bool operator<=(const BigInt& num1, const BigInt& num2)
{
    return !(num2 < num1);
}



