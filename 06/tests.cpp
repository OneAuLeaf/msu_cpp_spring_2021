#include <cassert>
#include <cstring>
#include <ostream>

#include "Format.hpp"

struct CustomData
{
    int x;
    friend std::ostream& operator<<(std::ostream& out, CustomData data) {
        out << "CustomData " << data.x;
        return out;
    }
};

// обычное использование
void defaultUse()
{
    auto text1 = format("Today {0} is equal to {1}", 1, 0);
    assert(text1 == "Today 1 is equal to 0");

    auto text2 = format("{1}{0} is equal to {0}{1} today", 0, 1);
    assert(text2 == "10 is equal to 01 today");
}

// аргументы format могут быть как lvalue, так и rvalue
void testRLvalue()
{
    std::string arg = "five";

    auto text3 = format("{0} + {1} = {2}", 2, 3, arg); // string lvalue
    assert(text3 == "2 + 3 = five");

    auto text4 = format("{0} + {1} = {2}", 2, 3, std::move(arg)); // string rvalue
    assert(text4 == "2 + 3 = five");
}

// объекты могут быть разных типов, но должны быть выводимы в поток
void fewTypesUse()
{
    auto text5 = format("Few types: int {0}, double {1}, string {2}, custom {3}", 2, 3.5, "five", CustomData{7});
    assert(text5 == "Few types: int 2, double 3.5, string five, custom CustomData 7");
}

// тест исключения BadFormat
void testBadFormat()
{
    try {
        auto text6 = format("{ not closed", 1, 2, 3); // no close bracket
        assert(false);
    } catch (const BadFormat& e) {
        assert(!strcmp(e.what(), "{ not closed bad format at index: 0"));
    } catch (...) {
        assert(false);
    }

    try {
        auto text7 = format("not } opened", 1, 2, 3); // close bracket before open
        assert(false);
    } catch (const BadFormat& e) {
        assert(!strcmp(e.what(), "not } opened bad format at index: 4"));
    } catch (...) {
        assert(false);
    }
    
    try {
        auto text8 = format("{0} {} {2}", 1, 2, 3); // empty brackets
        assert(false);
    } catch (const BadFormat& e) {
        assert(!strcmp(e.what(), "{0} {} {2} bad format at index: 5"));
    } catch (...) {
        assert(false);
    }

    try {
        auto text9 = format("{0} {{}} {2}", 1, 2, 3); // not valid index
        assert(false);
    } catch (const BadFormat& e) {
        assert(!strcmp(e.what(), "{0} {{}} {2} bad format at index: 5"));
    } catch (...) {
        assert(false);
    }

    try {
        auto text10 = format("{0} {notvalid} {2}", 1, 2, 3); // not valid index
        assert(false);
    } catch (const BadFormat& e) {
        assert(!strcmp(e.what(), "{0} {notvalid} {2} bad format at index: 5"));
    } catch (...) {
        assert(false);
    }

    try {
        auto text11 = format("{0} {0valid} {2}", 1, 2, 3); // not valid index
        assert(false);
    } catch (const BadFormat& e) {
        assert(!strcmp(e.what(), "{0} {0valid} {2} bad format at index: 5"));
    } catch (...) {
        assert(false);
    }
}

// тест исключения OutOfRange
void testOutOfRange()
{
    try {
        auto text12 = format("{0} {1} {10}", 1, 2, 3);
        assert(false);
    } catch (const OutOfRange& e) {
        assert(!strcmp(e.what(), "{0} {1} {10} out of range: 10"));
    } catch (...) {
        assert(false);
    }
}

// пустые тесты
void emptyFormat()
{
    auto text13 = format("No format index", 1, 2, 3); // no index - valid
    assert(text13 == "No format index");

    auto text14 = format("", 1, 2, 3); // empty format string - valid
    assert(text14 == "");

    auto text15 = format(""); // all empty - valid
    assert(text15 == "");

    try {
        auto text16 = format("{0}"); // invalid
        assert(false);
    } catch (const OutOfRange& e) {
        assert(!strcmp(e.what(), "{0} out of range: 0"));
    } catch (...) {
        assert(false);
    }
}

// тест с одним элементом
void singleFormat()
{
    auto text17 = format("{0}", "Valid");
    assert(text17 == "Valid");
}

int main()
{
    defaultUse();
    testRLvalue();
    fewTypesUse();
    testBadFormat();
    testOutOfRange();
    emptyFormat();
    singleFormat();
}