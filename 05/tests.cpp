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

struct NotSerializableData
{
    uint64_t a;
    bool b;
    uint64_t c;
};

// обычное использование
void defaultUse()
{
    Data x { 1, true, 2 };

    std::stringstream stream;

    Serializer serializer(stream);
    Error err_save = serializer.save(x);
    assert(err_save == Error::NoError);

    Data y { 0, false, 0 };

    Deserializer deserializer(stream);
    Error err_load = deserializer.load(y);
    assert(err_load == Error::NoError);

    assert(x.a == y.a);
    assert(x.b == y.b);
    assert(x.c == y.c);
}

// несериализуемые структуры
void notSerializable()
{
    NotSerializableData x {1, true, 2};
    std::stringstream stream;
    
    Serializer serializer(stream);
    Error err_save = serializer.save(x);
    assert(err_save == Error::NotSerializable);

    stream << "1 true 2";

    NotSerializableData y { 0, false, 0 };
    Deserializer deserializer(stream);
    Error err_load = deserializer.load(y);
    assert(err_load == Error::NotSerializable);
}

int main()
{
    defaultUse();
    notSerializable();
}