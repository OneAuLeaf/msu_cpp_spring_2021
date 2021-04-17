#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <string>
#include <ostream>

class BigInt
{
    static constexpr int64_t mod = 1'000'000'000;
    static constexpr int32_t base = 9;

    void reallocate(size_t);
    void shrink();
public:
    BigInt(int32_t _=0);
    explicit BigInt(const std::string&);
    BigInt(const BigInt&);
    BigInt& operator= (const BigInt&);
    BigInt(BigInt&&);
    BigInt& operator= (BigInt&&);

    BigInt operator- () const;

    BigInt& operator+=(const BigInt&);
    BigInt& operator-=(const BigInt&);
    BigInt& operator*=(const BigInt&);
    
    friend BigInt operator+ (const BigInt&, const BigInt&);
    friend BigInt operator- (const BigInt&, const BigInt&);
    friend BigInt operator* (const BigInt&, const BigInt&);
    
    friend bool operator==(const BigInt&, const BigInt&);
    friend bool operator!=(const BigInt&, const BigInt&);
    friend bool operator<(const BigInt&, const BigInt&);
    friend bool operator>(const BigInt&, const BigInt&);
    friend bool operator>=(const BigInt&, const BigInt&);
    friend bool operator<=(const BigInt&, const BigInt&);

    friend std::ostream& operator<<(std::ostream&, const BigInt&);

    ~BigInt();
private:
    size_t size_;
    int32_t sign_;
    int32_t *data_;
};

#endif