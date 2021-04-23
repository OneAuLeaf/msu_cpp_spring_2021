template <class T, std::enable_if_t<Serializer::ISerializable<T>::value, bool>>
Error Serializer::save(T&& object)
{
    return object.serialize(*this);
}

template <class T, std::enable_if_t<!Serializer::ISerializable<T>::value, bool>>
Error Serializer::save(T&& object)
{
    return Error::NotSerializable;
}

template <class... ArgsT>
Error Serializer::operator()(ArgsT... args)
{
    return process(args...);
}

template <class First, class... ArgsT>
Error Serializer::process(First obj, ArgsT... args)
{
    Error res = process(obj);
    return res | process(args...);
}

template <class T, std::enable_if_t<Deserializer::IDeserializable<T>::value, bool>>
Error Deserializer::load(T& object)
{
    return object.deserialize(*this);
}

template <class T, std::enable_if_t<!Deserializer::IDeserializable<T>::value, bool>>
Error Deserializer::load(T& object)
{
    return Error::NotSerializable;
}

template <class... ArgsT>
Error Deserializer::operator()(ArgsT&... args)
{
    return process(args...);
}

template <class First, class... ArgsT>
Error Deserializer::process(First& obj, ArgsT&... args)
{
    Error res = process(obj);
    return res | process(args...);
}