#include <cassert>
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>

#include "Vector.hpp"
#include "Allocator.hpp"

// сравнение состояний STL вектора и кастомного
template<class T, class AllocatorTL, class AllocatorTR>
bool operator==(const std::vector<T, AllocatorTL>& stl_vec, const Vector<T, AllocatorTR>& vec)
{
    if (stl_vec.size() != vec.size()) {
        return false;
    }

    return std::equal(stl_vec.begin(), stl_vec.end(), vec.begin());;
}

// дефолтное использование
void defaultUse()
{
    Vector<int, Allocator<int>> vec0 = {1, 2, 3, 4};
    std::vector<int> stl_vec0 = {1, 2, 3, 4};
    assert(stl_vec0 == vec0);

    vec0.push_back(42);
    stl_vec0.push_back(42);
    assert(stl_vec0 == vec0);

    vec0.resize(7);
    stl_vec0.resize(7);
    assert(stl_vec0 == vec0);

    vec0.pop_back();
    stl_vec0.pop_back();
    assert(stl_vec0 == vec0);

    vec0[0] = 1000 - 7;
    stl_vec0[0] = 1000 - 7;
    assert(stl_vec0 == vec0);

    std::sort(vec0.begin(), vec0.end());
    std::sort(stl_vec0.begin(), stl_vec0.end());
    assert(stl_vec0 == vec0);
}

// использование дефолтного и кастомного аллокатора
void allocatorUse()
{
    Vector<int> default_vec1 = {1, 2, 3, 4}; // std::allocator<int> используется
    Vector<int, Allocator<int>> custom_vec1 = {1, 2, 3, 4};
    std::vector<int, Allocator<int>> stl_vec1 = {1, 2, 3, 4}; // используем кастомный аллокатор для stl вектора
    assert(stl_vec1 == default_vec1);
    assert(stl_vec1 == custom_vec1);
    assert(default_vec1.capacity() == custom_vec1.capacity());

    default_vec1.push_back(42);
    custom_vec1.push_back(42);
    stl_vec1.push_back(42);
    assert(stl_vec1 == default_vec1);
    assert(stl_vec1 == custom_vec1);
    assert(default_vec1.capacity() == custom_vec1.capacity());

    default_vec1.emplace_back(1000 - 7);
    custom_vec1.emplace_back(1000 - 7);
    stl_vec1.emplace_back(1000 - 7);
    assert(stl_vec1 == default_vec1);
    assert(stl_vec1 == custom_vec1);
    assert(default_vec1.capacity() == custom_vec1.capacity());

    default_vec1.resize(7);
    custom_vec1.resize(7);
    stl_vec1.resize(7);
    assert(stl_vec1 == default_vec1);
    assert(stl_vec1 == custom_vec1);
    assert(default_vec1.capacity() == custom_vec1.capacity());

    default_vec1.reserve(13);
    custom_vec1.reserve(13);
    stl_vec1.reserve(13);
    assert(stl_vec1 == default_vec1);
    assert(stl_vec1 == custom_vec1);
    assert(default_vec1.capacity() == custom_vec1.capacity());    
}

// проверка конструкторов
void constructorTest()
{
    // по умолчанию
    Vector<int, Allocator<int>> vec2;
    std::vector<int> stl_vec2;
    assert(stl_vec2 == vec2);

    // от аллокатора
    Allocator<int> allocator;
    Vector<int, Allocator<int>> vec3(allocator);
    std::vector<int, Allocator<int>> stl_vec3(allocator);
    assert(stl_vec3 == vec3);

    // от размера
    Vector<int, Allocator<int>> vec4(5);
    std::vector<int> stl_vec4(5);
    assert(stl_vec4 == vec4);

    // от размера и значения для заполнения
    Vector<int, Allocator<int>> vec5(3, 13);
    std::vector<int> stl_vec5(3, 13);
    assert(stl_vec5 == vec5);

    // от диапазона
    int arr[] = {1, 2, 4, 8, 16, 32, 64};
    Vector<int, Allocator<int>> vec6(std::begin(arr), std::end(arr));
    std::vector<int> stl_vec6(std::begin(arr), std::end(arr));
    assert(stl_vec6 == vec6);

    // от initializer list
    Vector<int, Allocator<int>> vec7 = {0, 3, 5, 6, 9, 10};
    std::vector<int> stl_vec7 = {0, 3, 5, 6, 9, 10};
    assert(stl_vec7 == vec7);

    // конструктор копирования
    Vector<int, Allocator<int>> vec8 = vec7;
    std::vector<int> stl_vec8 = stl_vec7;
    assert(stl_vec8 == vec8);
    assert(stl_vec8 == vec7); // по транзитивности vec8 == vec7
    assert(vec7.capacity() == vec8.capacity());
    
    // изменение vec8 не отражается в vec7 и наоборот
    vec8[0] = 15;
    stl_vec8[0] = 15;
    assert(vec8[0] != vec7[0]);
    vec7.push_back(12);
    assert(vec8.size() + 1 == vec7.size());

    // конструктор перемещения
    Vector<int, Allocator<int>> vec9(std::move(vec8));
    std::vector<int> stl_vec9(std::move(stl_vec8));
    assert(stl_vec9 == vec9);
    assert(vec8.empty()); // после перемещение старый объект остается пустым
}

// проверка операторов присваивания
void assignmentTest()
{
    Vector<int, Allocator<int>> vec10 = {1, 2, 3}, vec11 = {4, 5, 6, 7};
    std::vector<int> stl_vec10 = {1, 2, 3}, stl_vec11 = {4, 5, 6, 7};
    assert(stl_vec10 == vec10 && stl_vec11 == vec11);

    // копирующий оператор присваивания
    vec10 = vec11;
    stl_vec10 = stl_vec11;
    assert(stl_vec10 == vec10);
    assert(stl_vec10 == vec11); // по транзитивности vec10 == vec11

    // изменение vec10 не отражается в vec11
    vec10[0] = 15;
    stl_vec10[0] = 15;
    assert(vec10[0] != vec11[0]);
    vec11.pop_back();
    stl_vec11.pop_back();
    assert(vec10.size() - 1 == vec11.size());

    // оператор перемещения
    Vector<int, Allocator<int>> vec12;
    std::vector<int> stl_vec12;
    vec12 = std::move(vec11);
    stl_vec12 = std::move(stl_vec11);
    assert(stl_vec12 == vec12);

    // оператор присваивания от initializer list
    Vector<int, Allocator<int>> vec13(5, 5);
    std::vector<int> stl_vec13(5, 5);
    vec13 = {-5, -1, 1, 5, 7, 9};
    stl_vec13 = {-5, -1, 1, 5, 7, 9};
    assert(stl_vec13 == vec13);
}

struct Simple
{
    int a;

    bool operator==(const Simple& other) const { return a == other.a; }
};

// проверка операций с итераторами
void iteratorTest()
{
    Vector<int, Allocator<int>> vec14(5);
    std::vector<int> stl_vec14(5);
    // практическое использование итераторов
    // begin, end
    int k = 0;
    for (auto it = vec14.begin(); it != vec14.end(); ++it) {
        *it = k++;
    }
    k = 0;
    for (auto it = stl_vec14.begin(); it != stl_vec14.end(); ++it) {
        *it = k++;
    }
    assert(stl_vec14 == vec14);
    // rbegin, rend
    k = 0;
    for (auto it = vec14.rbegin(); it != vec14.rend(); ++it) {
        *it = k++;
    }
    k = 0;
    for (auto it = stl_vec14.rbegin(); it != stl_vec14.rend(); ++it) {
        *it = k++;
    }
    assert(stl_vec14 == vec14);

    auto stl_it = stl_vec14.begin();
    // или так
    for (auto x : vec14) {
        assert(x == *stl_it++);
    }
    // алгоритмы на примере std::sort
    // begin, end
    Vector<int, Allocator<int>> vec15 = {2, 3, 1, 5, 4};
    std::vector<int> stl_vec15 = {2, 3, 1, 5, 4};
    std::sort(vec15.begin(), vec15.end());
    std::sort(stl_vec15.begin(), stl_vec15.end());
    assert(stl_vec15 == vec15);
    // rbegin, rend
    Vector<int, Allocator<int>> vec16 = {2, 3, 1, 5, 4};
    std::vector<int> stl_vec16 = {2, 3, 1, 5, 4};
    std::sort(vec16.rbegin(), vec16.rend());
    std::sort(stl_vec16.rbegin(), stl_vec16.rend());
    assert(stl_vec16 == vec16);

    // тест операций итератора

    Vector<int, Allocator<int>> vec17 = {7, 2, 3, 4, 5};
    // operator*
    auto it = vec17.begin();
    assert((*it) == 7);
    *it = 1;
    assert((*it) == 1);
    // operator[]
    for (int i = 0; i < 5; ++i) {
        assert(it[i] == i + 1);
    }
    // operator->
    Vector<Simple, Allocator<Simple>> vec18(5);
    std::vector<Simple> stl_vec18(5);
    k = 0;
    for(auto it = vec18.begin(); it != vec18.end(); ++it){
        it->a = k++;
    }
    k = 0;
    for(auto it = stl_vec18.begin(); it != stl_vec18.end(); ++it){
        it->a = k++;
    }
    assert(stl_vec18 == vec18);
    k = 0;
    for(auto it = vec18.begin(); it != vec18.end(); ++it){
        assert(it->a == k++);
    }

    Vector<int, Allocator<int>> vec19 = {1, 2, 3, 4};
    // operator++
    auto it1 = vec19.begin();
    auto it2 = ++it1;
    assert(it2 == vec19.begin() + 1 && it1 == vec19.begin() + 1);
    // operator++(int)
    it1 = vec19.begin();
    it2 = it1++;
    assert(it2 == vec19.begin() && it1 == vec19.begin() + 1);
    // operator--
    it1 = vec19.end();
    it2 = --it1;
    assert(it2 == vec19.end() - 1 && it1 == vec19.end() - 1);
    // operator--(int)
    it1 = vec19.end();
    it2 = it1--;
    assert(it2 == vec19.end() && it1 == vec19.end() - 1);

    Vector<int, Allocator<int>> vec20 = {1, 2, 3, 4, 5, 6, 7};
    // operator+=
    it = vec20.begin();
    assert(*it == vec20[0]);
    it += 3;
    assert(*it == vec20[3]);
    // operator+
    assert(*(it + 2) == vec20[3 + 2]); // it + diff_type
    assert(*(2 + it) == vec20[2 + 3]); // diff_type + it
    // operator-=
    it = vec20.end();
    it -= 3;
    assert(*it == vec20[vec20.size() - 3]);
    // operator-(diff_type)
    assert(*(it - 2) == vec20[vec20.size() - 3 - 2]);
    // operator-(iterator)
    assert(vec20.end() - vec20.begin() == 7 && 7 == vec20.size());

    Vector<int, Allocator<int>> vec21 = {12, 5, 1};
    // operator==
    assert(vec21.begin() + vec21.size() == vec21.end());
    assert(!(vec21.begin() == vec21.end()));
    // operator!=
    assert(!(vec21.begin() + vec21.size() != vec21.end()));
    assert(vec21.begin() != vec21.end());
    // operator<
    assert(vec21.begin() < vec21.end());
    assert(!(vec21.end() < vec21.begin()));
    // operator>
    assert(!(vec21.begin() > vec21.end()));
    assert(vec21.end() > vec21.begin());
    // operator>=
    assert(!(vec21.begin() >= vec21.end()));
    assert(vec21.end() >= vec21.begin());
    assert(vec21.begin() >= vec21.begin());
    // operator<=
    assert(vec21.begin() <= vec21.end());
    assert(!(vec21.end() <= vec21.begin()));
    assert(vec21.begin() <= vec21.begin());
}

// проверка функциональности методов empty, size, capacity и оператора []
void functionalityUse()
{
    Vector<int, Allocator<int>> vec21 = {12, 5, 1, 42};
    std::vector<int> stl_vec21 = {12, 5, 1, 42};
    // size
    assert(vec21.size() == 4 && vec21.size() == stl_vec21.size());
    // capacity
    assert(vec21.capacity() == 4 && vec21.capacity() == stl_vec21.capacity());

    vec21.pop_back();
    stl_vec21.pop_back();
    // size
    assert(vec21.size() == 3 && vec21.size() == stl_vec21.size());
    // capacity
    assert(vec21.capacity() == 4 && vec21.capacity() == stl_vec21.capacity());

    // empty
    Vector<int, Allocator<int>> vec22;
    assert(!vec21.empty() && vec22.empty());

    // operator[]
    Vector<int, Allocator<int>> vec23 = {1, 2, 3, 4};
    int k = 0;
    for (size_t i = 0; i < vec23.size(); ++i) {
        assert(vec23[i] == ++k);
    }

    // выход за границы индексов
    try {
        vec23[vec23.size()] += 1;
        assert(false);
    } catch (const std::out_of_range& e) {
        assert(std::string(e.what()) == "Index " + std::to_string(vec23.size()) + " out of range " + std::to_string(vec23.size()));
    } catch (...) {
        assert(false);
    }
    // выход за границы индексов
    try {
        vec23[0] += vec23[vec23.size()];
        assert(false);
    } catch (const std::out_of_range& e) {
        assert(std::string(e.what()) == "Index " + std::to_string(vec23.size()) + " out of range " + std::to_string(vec23.size()));
    } catch (...) {
        assert(false);
    }
}

// тестирование resize/reserve
void memoryUse()
{
    Vector<int, Allocator<int>> vec24 = {1, 2, 3, 4};
    assert(vec24.size() == 4 && vec24.capacity() == 4);

    // reserve <= текущего capacity
    vec24.reserve(0);
    assert(vec24.size() == 4 && vec24.capacity() == 4);
    vec24.reserve(3);
    assert(vec24.size() == 4 && vec24.capacity() == 4);
    vec24.reserve(4);
    assert(vec24.size() == 4 && vec24.capacity() == 4);
    // reserve > текущего capacity
    vec24.reserve(5);
    assert(vec24.size() == 4 && vec24.capacity() == 5);
    vec24.reserve(10);
    assert(vec24.size() == 4 && vec24.capacity() == 10);

    vec24.reserve(7);
    assert(vec24.size() == 4 && vec24.capacity() == 10); // ничего не происходит

    Vector<int, Allocator<int>> vec25 = {1, 2, 3, 4};
    std::vector<int> stl_vec25 = {1, 2, 3, 4};
    assert(vec25.size() == 4 && vec25.capacity() == 4);
    assert(stl_vec25.size() == 4 && stl_vec25.capacity() == 4);
    
    // resize new_size < size
    vec25.resize(2);
    stl_vec25.resize(2);
    assert(vec25.size() == 2 && vec25.capacity() == 4);
    assert(stl_vec25 == vec25); // как в оригинальном векторе, удаленные объекты уничтожаются.

    // resize new_size == size
    vec25.resize(2);
    stl_vec25.resize(2);
    assert(vec25.size() == 2 && vec25.capacity() == 4);
    assert(stl_vec25 == vec25); // ничего не происходит

    // resize capacity >= new_size > size
    vec25.resize(4);
    stl_vec25.resize(4);
    assert(vec25.size() == 4 && vec25.capacity() == 4);
    assert(stl_vec25 == vec25); // как в оригинальном векторе, расширенные элементы конструируются по умолчанию.

    // resize new_size > capacity
    vec25.resize(7);
    stl_vec25.resize(7);
    assert(vec25.size() == 7 && vec25.capacity() == 7);
    assert(stl_vec25 == vec25); // сначала расширяемся до необходимого размера потом resize
    
}

struct MoveOnly
{
    int a;
    double b;

    MoveOnly(int a, double b) : a{a}, b{b} {}
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(const MoveOnly&) = delete;
    MoveOnly& operator=(MoveOnly&&) = default;
    ~MoveOnly() = default;

    bool operator==(const MoveOnly& other) const { return a == other.a; }
};

// проверка мметодов push_back, emplace_back, pop_back, clear
void changingUse()
{
    Vector<int, Allocator<int>> vec26;
    std::vector<int> stl_vec26;
    assert(vec26.empty() && stl_vec26.empty());

    // push_back и расширения
    vec26.push_back(1000 - 7);
    stl_vec26.push_back(1000 - 7);
    assert(stl_vec26 == vec26 && vec26.size() == 1 && vec26.capacity() == 1); // расширение

    vec26.reserve(3);
    assert(vec26.capacity() == 3);

    vec26.push_back(12);
    stl_vec26.push_back(12);
    assert(stl_vec26 == vec26 && vec26.size() == 2 && vec26.capacity() == 3); // расширения нет
    vec26.push_back(5);
    stl_vec26.push_back(5);
    assert(stl_vec26 == vec26 && vec26.size() == 3 && vec26.capacity() == 3); // расширения нет
    vec26.push_back(5);
    stl_vec26.push_back(5);
    assert(stl_vec26 == vec26 && vec26.size() == 4 && vec26.capacity() == 7); // расширение cap = 2 * cap + 1

    // pop_back
    vec26.pop_back();
    stl_vec26.pop_back();
    assert(stl_vec26 == vec26 && vec26.size() == 3 && vec26.capacity() == 7);

    // clear
    vec26.clear();
    stl_vec26.clear();
    assert(stl_vec26 == vec26 && vec26.size() == 0 && vec26.capacity() == 7);
    assert(vec26.empty() && stl_vec26.empty());

    // pop_back from empty
    try {
        vec26.pop_back();
        assert(false);
    } catch (const std::out_of_range& e) {
        assert(std::string(e.what()) == "Vector is empty");
    } catch (...) {
        assert(false);
    }

    // emplace_back
    vec26.emplace_back(1000 - 7);
    stl_vec26.emplace_back(1000 - 7);
    assert(stl_vec26 == vec26 && vec26.size() == 1 && vec26.capacity() == 7);
}

// операции для объектов с запрещенным копированием
void notCopyableUse()
{
    Vector<MoveOnly, Allocator<MoveOnly>> vec27;
    std::vector<MoveOnly> stl_vec27;
    // push_back(T&&)
    MoveOnly obj1(3, 1.6);
    MoveOnly stl_obj1(3, 1.6);
    vec27.push_back(std::move(obj1));
    stl_vec27.push_back(std::move(stl_obj1));
    assert(stl_vec27 == vec27 && vec27.size() == 1 && vec27.capacity() == 1); // расширение

    MoveOnly obj2(5, 2.5);
    MoveOnly stl_obj2(5, 2.5);
    vec27.push_back(std::move(obj2));
    stl_vec27.push_back(std::move(stl_obj2));
    assert(stl_vec27 == vec27 && vec27.size() == 2 && vec27.capacity() == 3);

    // emplace_back
    vec27.emplace_back(7, 3.6);
    stl_vec27.emplace_back(7, 3.6);
    assert(stl_vec27 == vec27 && vec27.size() == 3 && vec27.capacity() == 3);

    vec27.emplace_back(11, 4.9);
    stl_vec27.emplace_back(11, 4.9);
    assert(stl_vec27 == vec27 && vec27.size() == 4 && vec27.capacity() == 7); // расширение

    //pop_back
    vec27.pop_back();
    stl_vec27.pop_back();
    assert(stl_vec27 == vec27 && vec27.size() == 3 && vec27.capacity() == 7);

    //clear
    vec27.clear();
    stl_vec27.clear();
    assert(stl_vec27 == vec27 && vec27.size() == 0 && vec27.capacity() == 7);
    assert(stl_vec27.empty() && vec27.empty());
}

// пустой вектор и его особенности
void emptyCase()
{
    Vector<int, Allocator<int>> vec28;
    assert(vec28.empty()); // он пустой
    assert(vec28.size() == 0 && vec28.capacity() == 0); // память под данные не выделяется
    
    // выход за границы индексов
    try {
        vec28[0] += 1;
        assert(false);
    } catch (const std::out_of_range& e) {
        assert(std::string(e.what()) == "Index 0 out of range 0");
    } catch (...) {
        assert(false);
    }

    // pop_back не работает
    try {
        vec28.pop_back();
        assert(false);
    } catch (const std::out_of_range& e) {
        assert(std::string(e.what()) == "Vector is empty");
    } catch (...) {
        assert(false);
    }

    // clear ничего не делает
    vec28.clear();
    assert(vec28.empty() && vec28.size() == 0 && vec28.capacity() == 0);

    // итераторы начала и конца совпадают
    assert(vec28.begin() == vec28.end());
    assert(vec28.rbegin() == vec28.rend());
}

int main(){
    defaultUse();
    allocatorUse();
    constructorTest();
    assignmentTest();
    iteratorTest();
    functionalityUse();
    memoryUse();
    changingUse();
    notCopyableUse();
    emptyCase();
}