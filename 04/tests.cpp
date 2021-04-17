#include <iostream>
#include <cassert>
#include <sstream>
#include <string>

#include "BigInt.hpp"

// обычное использование
void defaultUse()
{
    BigInt a = 1;
    assert(a == 1);
    BigInt b("123456789012345678901234567890");
    BigInt c = a * b + 2;
    BigInt d;
    d = std::move(c);
    a = d + b;
    std::cout << "d: " << d << std::endl;
}

// создание и вывод в поток
void testInOut()
{
    // from small int32_t
    BigInt big1 = 12345;
    std::cout << "from int32_t: " << big1 << std::endl;

    // from BIG int32_t (> 1e9)
    BigInt big2 = 2'000'000'000;
    std::cout << "from BIG int32_t: " << big2 << std::endl;

    // from string
    BigInt big3("12345678901234567890");
    std::cout << "from string: " << big3 << std::endl;

    // from string with leading zeros
    BigInt big4("00012345");
    std::cout << "leading zeros string: " << big4 << std::endl;

    // from string with sign (and leading zeros probably)
    BigInt big5("+0012345");
    BigInt big6("-12345");
    std::cout << "from string +: " << big5 << " from string -: " << big6 << std::endl;
}

// проверка работы конструкторов/операторов копирования/перемещения
void testCopyMove()
{
    std::string answer = "12345";
    std::stringstream ss;
    BigInt big7(answer);
    std::string big7str;
    ss << big7;
    ss >> big7str;
    assert(big7str == answer);
    ss.clear();
    // copy constructor
    BigInt big8 = big7;
    std::string big8str;
    ss << big8 << " " << big7;
    ss >> big8str >> big7str;
    assert(big8str == answer && big7str == answer);
    ss.clear();
    // copy assignment
    BigInt big9 = 1;
    std::string big9str;
    big9 = big7;
    ss << big9 << " " << big7;
    ss >> big9str >> big7str;
    assert(big9str == answer && big7str == answer);
    ss.clear();

    // перемещение оставляет объект в состоянии эквивалентым 0

    // move constructor
    BigInt big10 = std::move(big8);
    std::string big10str;
    ss << big10 << " " << big8;
    ss >> big10str >> big8str;
    assert(big10str == answer && big8str == "0");
    ss.clear();
    // move assignment
    BigInt big11 = 1;
    std::string big11str;
    big11 = std::move(big9);
    ss << big11 << " " << big9;
    ss >> big11str >> big9str;
    assert(big11str == answer && big9str == "0");
}

// арифметические операции
void arithmeticOps()
{
    std::stringstream ss;
    std::string result;
    BigInt big("9010101234");
    BigInt big_neg("-1234567890");

    ss << -big_neg; // unary - 
    ss >> result;
    assert(result == "1234567890");
    ss.clear();

    BigInt big12 = big + big; // + same sign
    ss << big12;
    ss >> result;
    assert(result == "18020202468");
    ss.clear();

    BigInt big13 = big + big_neg; // + diff sign
    ss << big13;
    ss >> result;
    assert(result == "7775533344");
    ss.clear();

    BigInt big13_1 = big;
    big13_1 += big_neg; // += equals +
    ss << big13_1;
    ss >> result;
    assert(result == "7775533344");
    ss.clear();

    BigInt big14 = big12 - big; // - same sign
    ss << big14;
    ss >> result;
    assert(result == "9010101234");
    ss.clear();

    BigInt big15 = big13 - big_neg; // - diff sign
    ss << big15;
    ss >> result;
    assert(result == "9010101234");
    ss.clear();

    BigInt big16 = big - big12; // - over zero
    ss << big16;
    ss >> result;
    assert(result == "-9010101234");
    ss.clear();

    BigInt big16_1 = big;
    big16_1 -= big12; // -= equals -
    ss << big16_1;
    ss >> result;
    assert(result == "-9010101234");
    ss.clear();

    BigInt big17 = big * big; // * same sign +
    ss << big17;
    ss >> result;
    assert(result == "81181924246928322756");
    ss.clear();

    BigInt big18 = big_neg * big_neg; // * same sign -
    ss << big18;
    ss >> result;
    assert(result == "1524157875019052100");
    ss.clear();

    BigInt big19 = big * big_neg; // * diff sign
    ss << big19;
    ss >> result;
    assert(result == "-11123581669145776260");
    ss.clear();

    BigInt big19_1 = big;
    big19_1 *= big_neg; // *= equals *=
    ss << big19_1;
    ss >> result;
    assert(result == "-11123581669145776260");
    ss.clear();
}

// логические операции
void logicalOps()
{
    std::stringstream ss;
    BigInt big("9010101234");
    BigInt small("12451");
    BigInt big_neg("-1234567890");
    BigInt small_neg("-12451");

    assert(-(-big_neg) == big_neg); // == true
    assert(!((-big_neg) == big)); // == false
    assert(!(small == big)); // == diff size
    assert(!(small == small_neg)); // == diff sign

    assert((-big_neg) != big); // != equals !(==)
    
    assert(-big_neg < big); // < true with sign +
    assert(-big < big_neg); // < true with sign -
    assert(!(big < big)); // < false
    assert(small_neg < small); // < true diff sign
    assert(!(small < big_neg)); // < false diff sign
    assert(small < big && big_neg < small_neg); // < true diff size
    assert(!(big < small) && !(small_neg < big_neg)); // < false diff size

    assert(big > small); // _1 > _2 equals (_2 < _1)
    assert(big >= big); // _1 >= _2 equals !(_1 < _2)
    assert(big_neg <= small); // _1 <= _2 equals !(_2 < _1)
}

// совместимость с int32_t
void int32Compability()
{
    BigInt big20("12345678");

    big20 = (big20 - 78) * 13 + 100; // арифметические операции совместимы справа
    assert(big20 == BigInt("160492900"));
    big20 = 100 + 3 * (1 - big20); // арифметические операции совместимы слева
    assert(big20 == BigInt("-481478597"));

    BigInt big21 = 12345;
    assert(big21 == 12345 && big21 < 12346); // арифметические операции совместимы справа
    assert(12345 == big21 && 12346 > big21); // арифметические операции совместимы слева
}

// использование нуля
void zeroBigInt()
{
    // способы создать объект равный 0
    BigInt big22_1;
    BigInt big22_2 = 0;
    BigInt big22_3("0");
    BigInt big22_4("+0"); // знак отбросится
    BigInt big22_5("-0"); // знак отбросится
    assert(big22_1 == 0 && big22_2 == 0 && big22_3 == 0 && big22_4 == 0 && big22_5 == 0);
    std::cout << "zeros: " << big22_4 << " "<< big22_5 << std::endl;

    BigInt big23("1234567890");
    assert(big23 + big22_1 == big23 && big22_1 + big23 == big23); // x + 0 = 0 + x = 0
    assert(big23 - big22_2 == big23 && big22_2 - big23 == -big23); // x - 0 = x; 0 - x = -x
    assert(big23 * big22_3 == big22_4 && big22_3 * big23 == big22_5); // x * 0 = 0 * x = 0

    BigInt big24("-1234567890");
    assert(big23 != 0 && big24 != 0 && big22_1 == big22_5); // x == 0 <=> x = 0
    assert(big23 > 0 && big24 < 0); // x > 0 <=> sgn(x) > 0, x < 0 <=> sgn(x) < 0
}

// действительно большие числа
void soBig()
{
    BigInt factorial = 1;
    for (auto i = 1; i <= 42; ++i) {
        factorial *= i;
    }
    std::cout << "42! = " << std::endl;
    std::cout << factorial << std::endl;
    std::cout << "42! * 42! = " << std::endl;
    std::cout << factorial * factorial << std::endl;
}

int main()
{
    defaultUse();
    testInOut();
    testCopyMove();
    logicalOps();
    arithmeticOps();
    int32Compability();
    zeroBigInt();
    soBig();
}