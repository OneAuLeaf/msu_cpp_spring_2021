#include <iostream>
#include <cassert>
#include <vector>

#include "TokenParser.hpp"

void startCallback(bool& iscalled)
{
    iscalled = true;
}

void endCallback(bool& iscalled)
{
    iscalled = true;
}

void digitCallback(uint64_t digit, std::vector<uint64_t>& vec)
{
    vec.push_back(digit);
}

void stringCallback(const std::string& string, std::vector<std::string>& vec)
{
    vec.push_back(string);
}

// обычное использование парсера
void defaultUse()
{
    parse::TokenParser parser;
    std::vector<uint64_t> digit_test, digit_true;
    std::vector<std::string> string_test, string_true;
    bool start_called = false, end_called = false;
    // настраиваем callback 
    parser.SetStartCallback(std::bind(startCallback, std::ref(start_called)));
    parser.SetEndCallback(std::bind(endCallback, std::ref(end_called)));
    parser.SetDigitTokenCallback(std::bind(digitCallback, std::placeholders::_1, std::ref(digit_test)));
    parser.SetStringTokenCallback(std::bind(stringCallback, std::placeholders::_1, std::ref(string_test)));

    // используем парсер
    std::string test1 = "There are more than 5 words And 0nly 2 d1g1ts";
    digit_true = {5, 2};
    string_true = {"There", "are", "more", "than", "words", "And", "0nly", "d1g1ts"};
    parser.Parse(test1);
    assert(digit_test == digit_true && string_test == string_true && start_called && end_called);

    start_called = false;
    end_called = false;
    digit_test.clear();
    string_test.clear();

    // можем использовать парсер несколько раз
    std::string test2 = "This is second test";
    digit_true = {};
    string_true = {"This", "is", "second", "test"};
    parser.Parse(test2);
    assert(digit_test == digit_true && string_test == string_true && start_called && end_called);
}

// использование только некоторых callback'ов
void partialSetCallbacks()
{
    parse::TokenParser parser;
    std::vector<uint64_t> digit_test, digit_true;
    std::vector<std::string> string_test, string_true;
    bool start_called = false, end_called = false;
    
    // только start
    parser.SetStartCallback(std::bind(startCallback, std::ref(start_called)));
    std::string test3 = "1 start";
    parser.Parse(test3);
    assert(start_called);

    start_called = false;

    // только end
    parser.SetStartCallback(nullptr); // мы можем отвязывать callback'и
    parser.SetEndCallback(std::bind(startCallback, std::ref(end_called)));
    std::string test4 = "2 end";
    parser.Parse(test4);
    assert(!start_called && end_called);

    end_called = false;

    // только digit
    parser.SetEndCallback(nullptr);
    parser.SetDigitTokenCallback(std::bind(digitCallback, std::placeholders::_1, std::ref(digit_test)));
    std::string test5 = "3 digit";
    digit_true = {3};
    parser.Parse(test5);
    assert(digit_test == digit_true && !end_called);

    digit_test.clear();

    // только string
    parser.SetDigitTokenCallback(nullptr);
    parser.SetStringTokenCallback(std::bind(stringCallback, std::placeholders::_1, std::ref(string_test)));
    std::string test6 = "4 string";
    string_true = {"string"};
    parser.Parse(test6);
    assert(digit_test.empty() && string_test == string_true);

    string_test.clear();

    // ничего
    parser.SetStringTokenCallback(nullptr);
    std::string test7 = "5 void";
    parser.Parse(test7);
    assert(string_test.empty() && digit_test.empty() && !start_called && !end_called);
}

// обработка комбинаций пробельных символов
// пробельные символы это символы x для которых std::isspace(x) == true
// например обычные пробелы, табы, переводы строки
void differentSpaces()
{
    parse::TokenParser parser;
    std::vector<uint64_t> digit_test, digit_true;
    std::vector<std::string> string_test, string_true;
    parser.SetDigitTokenCallback(std::bind(digitCallback, std::placeholders::_1, std::ref(digit_test)));
    parser.SetStringTokenCallback(std::bind(stringCallback, std::placeholders::_1, std::ref(string_test)));

    // множественные пробельные символы
    std::string test8 = "500   days\n\t of\tSummer\n";
    digit_true = {500};
    string_true = {"days", "of", "Summer"};
    parser.Parse(test8);
    assert(digit_test == digit_true && string_test == string_true);

    digit_test.clear();
    string_test.clear();

    // пробелы в начале текста
    std::string test9 = "\t  21";
    digit_true = {21};
    string_true = {};
    parser.Parse(test9);
    assert(digit_test == digit_true && string_test == string_true);

    digit_test.clear();
    string_test.clear();

    // пробелы в конце
    std::string test10 = "Ocean's_11\t    \t";
    digit_true = {};
    string_true = {"Ocean's_11"};
    parser.Parse(test10);
    assert(digit_test == digit_true && string_test == string_true);

    digit_test.clear();
    string_test.clear();

    // строка вся из пробелов
    std::string test11 = "    \n \t\t\n   ";
    digit_true = {};
    string_true = {};
    parser.Parse(test11);
    assert(digit_test == digit_true && string_test == string_true);

    digit_test.clear();
    string_test.clear();

    // строка без пробельных символов
    std::string test12 = "LockStock&2SmokingBarrels";
    digit_true = {};
    string_true = {"LockStock&2SmokingBarrels"};
    parser.Parse(test12);
    assert(digit_test == digit_true && string_test == string_true);
}

// обработка строк и чисел
// число - такая последовательность символов [0-9]+, что помещается в uint64_t
// строкой считается всё, что не является числом
void someStringsAndDigits()
{
    parse::TokenParser parser;
    std::vector<uint64_t> digit_test, digit_true;
    std::vector<std::string> string_test, string_true;
    parser.SetDigitTokenCallback(std::bind(digitCallback, std::placeholders::_1, std::ref(digit_test)));
    parser.SetStringTokenCallback(std::bind(stringCallback, std::placeholders::_1, std::ref(string_test)));

    // примеры чисел
    std::string test13 = "42 1234567890 000123456789";
    digit_true = {42, 1234567890, 123456789};
    parser.Parse(test13);
    assert(digit_test == digit_true);

    // примеры строк
    // заметим, что по условию, строковое представление числа, начинающееся с +/- числом не считается
    // равно как и число не помещающееся в uint64_t
    std::string test14 = "строка string str1n9 57ring _$tr!#g_ +42 -42 1000000000000000000000";
    string_true = {"строка", "string", "str1n9", "57ring", "_$tr!#g_", "+42", "-42", "1000000000000000000000"};
    parser.Parse(test14);
    assert(string_test == string_true);
}

//тест на экстремальные случаи
void extraTest()
{
    parse::TokenParser parser;
    std::vector<uint64_t> digit_test, digit_true;
    std::vector<std::string> string_test, string_true;
    parser.SetDigitTokenCallback(std::bind(digitCallback, std::placeholders::_1, std::ref(digit_test)));
    parser.SetStringTokenCallback(std::bind(stringCallback, std::placeholders::_1, std::ref(string_test)));

    // пустая строка
    std::string test14;
    digit_true = {};
    string_true = {};
    parser.Parse(test14);
    assert(digit_test == digit_true && string_test == string_true);

    // строки минимальной длины
    std::string test15 = "1";
    digit_true = {1};
    string_true = {};
    parser.Parse(test15);
    assert(digit_test == digit_true && string_test == string_true);

    digit_test.clear();

    // строки минимальной длины
    std::string test16 = "a";
    digit_true = {};
    string_true = {"a"};
    parser.Parse(test16);
    assert(digit_test == digit_true && string_test == string_true);
}

int main()
{
    defaultUse();
    partialSetCallbacks();
    differentSpaces();
    someStringsAndDigits();
    extraTest();
}