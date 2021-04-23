#include <cassert>
#include <sstream>

#include "Serializer.hpp"

struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;
    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c);
    }
    template <class Deserializer>
    Error deserialize(Deserializer& deserializer)
    {
        return deserializer(a, b, c);
    }
};

struct Data2
{
    uint64_t a;
    bool b;
    uint64_t c;
    bool d;
    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c, d);
    }
    template <class Deserializer>
    Error deserialize(Deserializer& deserializer)
    {
        return deserializer(a, b, c, d);
    }
};

struct NotSerializableData
{
    uint64_t a;
    bool b;
    uint64_t c;
};

struct SingleUInt
{
    uint64_t a;
    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a);
    }
    template <class Deserializer>
    Error deserialize(Deserializer& deserializer)
    {
        return deserializer(a);
    }
};

struct SingleBool
{
    bool b;
    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(b);
    }
    template <class Deserializer>
    Error deserialize(Deserializer& deserializer)
    {
        return deserializer(b);
    }
};

// обычное использование
void defaultUse()
{
    Data x { 1, true, 2 };

    std::stringstream stream;
    Error err_save, err_load;

    Serializer serializer(stream);
    err_save = serializer.save(x);
    assert(err_save == Error::NoError);

    Data y { 0, false, 0 };

    Deserializer deserializer(stream);
    err_load = deserializer.load(y);
    assert(err_load == Error::NoError);

    assert(x.a == y.a);
    assert(x.b == y.b);
    assert(x.c == y.c);
}

// несколько последовательных сериализаций в 1 поток
void fewSerialization()
{
    Data x1 { 1, true, 2 };
    Data2 x2 { 3, false, 4 , false };

    std::stringstream stream;
    Error err_save, err_load;

    Serializer serializer(stream);
    err_save = serializer.save(x1);
    assert(err_save == Error::NoError);

    err_save = serializer.save(x2);
    assert(err_save == Error::NoError);

    Data y1 { 0, false, 0 };
    Data2 y2 { 0, false, 0, true };

    Deserializer deserializer(stream);
    err_load = deserializer.load(y1);
    assert(err_load == Error::NoError);
    assert(y1.a == x1.a && y1.b == x1.b && y1.c == x1.c);

    err_load = deserializer.load(y2);
    assert(err_load == Error::NoError);
    assert(y2.a == x2.a && y2.b == x2.b && y2.c == x2.c && y2.d == x2.d);
}

// несериализуемые структуры
void notSerializable()
{
    NotSerializableData x { 1, true, 2 };

    std::stringstream stream;
    Error err_save, err_load;

    Serializer serializer(stream);
    err_save = serializer.save(x);
    assert(err_save == Error::NotSerializable && stream.str().empty());

    stream << "1 true 2";

    NotSerializableData y { 0, false, 0 };

    Deserializer deserializer(stream);
    err_load = deserializer.load(y);
    assert(err_load == Error::NotSerializable);
}

// испорченная сериализация
void corruptedArchive()
{
    Data2 x { 1, true, 2, true};

    std::stringstream stream;
    Error err_save, err_load;

    Serializer serializer(stream);
    err_save = serializer.save(x); // save некогда не портит
    assert(err_save != Error::CorruptedArchive);
    stream.clear();

    Deserializer deserializer(stream);
    Data2 y { 0, false, 0 , false};

    stream.str("1 true 72int true"); // испорченный uint64_t (не полностью число)
    err_load = deserializer.load(y);
    assert(err_load == Error::CorruptedArchive);
    assert(y.a == 1 && y.b == true && y.c == 0 && y.d == true);
    stream.clear();

    y = { 0, false, 0 , false};
    stream.str("1 true int true"); // испорченный uint64_t (не число)
    err_load = deserializer.load(y);
    assert(err_load == Error::CorruptedArchive);
    assert(y.a == 1 && y.b == true && y.c == 0 && y.d == true);
    stream.clear();
    
    y = { 0, false, 0 , false};
    stream.str("1 true 2 12"); // испорченный bool (не строка)
    err_load = deserializer.load(y);
    assert(err_load == Error::CorruptedArchive);
    assert(y.a == 1 && y.b == true && y.c == 2 && y.d == false);
    stream.clear();

    y = { 0, false, 0 , false};
    stream.str("1 true 2 notbool"); // испорченный bool (строка не true/false)
    err_load = deserializer.load(y);
    assert(err_load == Error::CorruptedArchive);
    assert(y.a == 1 && y.b == true && y.c == 2 && y.d == false);
    stream.clear();

    y = { 0, false, 0 , false};
    stream.str("1 notbool notint true"); // испорченные bool и uint64_t
    err_load = deserializer.load(y);
    assert(err_load == Error::CorruptedArchive);
    assert(y.a == 1 && y.d == true); // при ошибке load в объекте изменяются все поля, которые успешно десериализировались
    assert(y.b == false && y.c == 0); // при ошибке load в объекте поля, для которых возвращалось CorruptedArchive, остаются в исходном состоянии
    stream.clear();

    y = { 0, false, 0 , false};
    stream.str("1 true 2"); // нехватка аргументов
    err_load = deserializer.load(y);
    assert(err_load == Error::CorruptedArchive);
    assert(y.a == 1 && y.b == true && y.c == 2 && y.d == false);
    stream.clear();

    y = { 0, false, 0 , false};
    stream.str("1 true"); // большая нехватка аргументов
    err_load = deserializer.load(y);
    assert(err_load == Error::CorruptedArchive);
    assert(y.a == 1 && y.b == true && y.c == 0 && y.d == false);
    stream.clear();

    y = { 0, false, 0 , false};
    stream.str(""); // полная нехватка аргументов
    err_load = deserializer.load(y);
    assert(err_load == Error::CorruptedArchive);
    assert(y.a == 0 && y.b == false && y.c == 0 && y.d == false);
    stream.clear();
}

// сериализация структур с 1 полем
void singleSerialization()
{
    SingleBool x1 {true};
    SingleUInt x2 {1};

    std::stringstream stream;
    Error err_save, err_load;

    Serializer serializer(stream);
    err_save = serializer.save(x1);
    assert(err_save == Error::NoError);
    
    err_save = serializer.save(x2);
    assert(err_save == Error::NoError);

    SingleBool y1 {false};
    SingleUInt y2 {0};

    Deserializer deserializer(stream);
    err_load = deserializer.load(y1);
    assert(err_load == Error::NoError);
    err_load = deserializer.load(y2);
    assert(err_load == Error::NoError);

    assert(x1.b == y1.b);
    assert(x2.a == y2.a);
}

int main()
{
    defaultUse();
    fewSerialization();
    notSerializable();
    corruptedArchive();
    singleSerialization();
}