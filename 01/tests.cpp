#include <iostream>
#include <cstring>
#include <cassert>

#include "Allocator.hpp"


// обычное использование аллокатора
void defaultUse()
{
    Allocator a;
    a.makeAllocator(10);
    assert(a.isNull() == false && a.capacity() == 10 &&
            a.size() == 10 && a.offset() == 0);

    char *test1 = a.alloc(4); // выделили кусочек
    char *test2 = a.alloc(3); // и ещё один
    const char *word1 = "yes", *word2 = "no", *word3 = "pozalusta";
    assert(test1 != nullptr && test2 != nullptr && a.offset() == 7);

    strcpy(test1, word1);
    strcpy(test2, word2);
    assert(!strcmp(test1, word1));
    assert(!strcmp(test2, word2));

    a.reset();
    assert(a.isNull() == false && a.capacity() == 10 &&
            a.size() == 10 && a.offset() == 0);
    
    char *test3 = a.alloc(10); // выделили после reset, заполнили всю память
    assert(test3 != nullptr && a.offset() == 10);

    strcpy(test3, word3);
    assert(!strcmp(test3, word3)); 
}

// использование аллокатора до первого вызова makeAllocator
void useBeforeMakeAllocator()
{
    Allocator a; // аллокатор с невыделенной памятью (null-алокатор)
    assert(a.isNull() == true && a.capacity() == 0 &&
            a.size() == 0 && a.offset() == 0);
    
    char *test4 = a.alloc(2); // для null-аллокатора alloc(size) возвращает nullptr
    assert(test4 == nullptr);
    assert(a.isNull() == true && a.capacity() == 0 &&
            a.size() == 0 && a.offset() == 0);

    a.reset(); // для null-аллокатора reset вызвать можно, состояние аллокатора не меняется
    assert(a.isNull() == true && a.capacity() == 0 &&
            a.size() == 0 && a.offset() == 0);
}

// вызов нескольких makeAllocator
void fewMakeAllocator()
{
    Allocator a;
    a.makeAllocator(10);

    const char *word5 = "meow", *word6 = "bark";

    char *test5 = a.alloc(5);
    strcpy(test5, word5);

    a.makeAllocator(20); // после этого test5 перестает быть валидным, а память перевыделяется (возможно в другом месте)
    assert(a.isNull() == false && a.capacity() == 20 &&
            a.size() == 20 && a.offset() == 0);

    char *test6 = a.alloc(5);
    strcpy(test6, word6);

    a.makeAllocator(5); // после этого test6 перестает быть валидным, но в силу реализации перевыделения памяти не происходит
    assert(a.isNull() == false && a.capacity() == 20 &&
            a.size() == 5 && a.offset() == 0);
    
    assert(!strcmp(a.alloc(5), word6)); // word6 всё еще в этом месте памяти

    a.makeAllocator(10);
    assert(a.isNull() == false && a.capacity() == 20 &&
            a.size() == 10 && a.offset() == 0);

    assert(!strcmp(a.alloc(5), word6)); // и тут тоже

    a.makeAllocator(30); // после этого память действительно перевыделяется, так что тут word6 может не быть
    assert(a.isNull() == false && a.capacity() == 30 &&
            a.size() == 30 && a.offset() == 0);
}

// нехватка места для alloc
void tryOverflowAlloc()
{
    Allocator a;
    a.makeAllocator(10);

    char *test7 = a.alloc(11);
    assert(test7 == nullptr);
}

// особенности выделения памяти 0 размера
// при вызове alloc(0) гарантируется возвращение nullptr
// при вызове makeAllocator(0) гарантируется, что выделение/перевыделение памяти не происходит
void useZeroSize()
{
    Allocator a;
    a.makeAllocator(0); // выделение памяти не происходит
    assert(a.isNull() == true && a.capacity() == 0 && 
            a.size() == 0 && a.offset() == 0);

    char *test8 = a.alloc(0); // для null-аллокатора alloc(0) возвращает nullptr
    assert(test8 == nullptr); 

    a.makeAllocator(5);
    assert(a.isNull() == false && a.capacity() == 5 &&
            a.size() == 5 && a.offset() == 0);

    char *test10 = a.alloc(0); // для не null-аллокатора со свободным местом alloc(0) возвращает nullptr
    assert(test10 == nullptr);

    a.makeAllocator(0); // перевыделение памяти не происходит
    assert(a.isNull() == false && a.capacity() == 5 &&
            a.size() == 0 && a.offset() == 0);

    char *test11 = a.alloc(0); // для не null-аллокатора без свободного места alloc(0) возвращает nullptr
    assert(test11 == nullptr);
}

int main()
{
    defaultUse();
    useBeforeMakeAllocator();
    fewMakeAllocator();
    tryOverflowAlloc();
    useZeroSize();
    return 0;
}