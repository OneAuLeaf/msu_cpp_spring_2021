#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <iostream>

enum class Error: uint32_t
{
    NoError,
    CorruptedArchive,
    NotSerializable
};

inline Error operator|(Error a, Error b)
{
    return static_cast<Error>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

class Serializer
{
    static constexpr char sep = ' ';

    template <class T>
    struct ISerializable
    {
    private:
        struct WrongRetType {};
        static WrongRetType check(...);
        template <class U>
        static auto check(U u) -> decltype(u.serialize(std::declval<Serializer&>()));
    public:
        using ExpectedRetType = Error;
        static constexpr bool value = std::is_same<ExpectedRetType, decltype(ISerializable<T>::check(std::declval<T>()))>::value;
    };
public:
    explicit Serializer(std::ostream& out);

    template <class T, std::enable_if_t<ISerializable<T>::value, bool> = true>
    Error save(T&& object);
    template <class T, std::enable_if_t<!ISerializable<T>::value, bool> = true>
    Error save(T&& object);

    template <class... ArgsT>
    Error operator()(ArgsT... args);
private:
    std::ostream& out_;

    template <class First, class... ArgsT>
    Error process(First obj, ArgsT... args);

    Error process(bool);
    Error process(uint64_t);
};

class Deserializer
{
    static constexpr char sep = ' ';

    template <class T>
    struct IDeserializable
    {
    private:
        struct WrongRetType {};
        static WrongRetType check(...);
        template <class U>
        static auto check(U u) -> decltype(u.deserialize(std::declval<Deserializer&>()));
    public:
        using ExpectedRetType = Error;
        static constexpr bool value = std::is_same<ExpectedRetType, decltype(IDeserializable<T>::check(std::declval<T>()))>::value;
    };
public:
    explicit Deserializer(std::istream& in);

    template <class T, std::enable_if_t<IDeserializable<T>::value, bool> = true>
    Error load(T& object);
    template <class T, std::enable_if_t<!IDeserializable<T>::value, bool> = true>
    Error load(T& object);

    template <class... ArgsT>
    Error operator()(ArgsT&... args);
private:
    std::istream& in_;

    template <class First, class... ArgsT>
    Error process(First& obj, ArgsT&... args);

    Error process(bool&);
    Error process(uint64_t&);
};

#include "Serializer.tpp"

#endif