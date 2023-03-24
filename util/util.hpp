#pragma once

#include <memory>
#include <string>

namespace util
{

std::string random_string(const int len);

template <typename T, typename... A> auto New(A &&...args) -> std::unique_ptr<T>
{
    return std::unique_ptr<T>(new T(std::forward<A>(args)...));
}

template <typename T, typename... A> auto NewShared(A &&...args) -> std::shared_ptr<T>
{
    return std::shared_ptr<T>(new T(std::forward<A>(args)...));
}

} // namespace util