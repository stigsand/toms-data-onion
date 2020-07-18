#ifndef ENFORCE_HPP
#define ENFORCE_HPP

#include <stdexcept>
#include <utility>

template <typename T, typename Arg>
decltype(auto) enforce(T && t, Arg && arg)
{
    if (!t)
        throw std::logic_error(std::forward<Arg>(arg));
    return std::forward<T>(t);
}

template <typename T>
decltype(auto) enforce(T && t)
{
    return enforce(std::forward<T>(t), "logic error");
}

#endif
