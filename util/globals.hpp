#pragma once

#include "../plog/Initializers/RollingFileInitializer.h"
#include "../plog/Log.h"
#include "toml.hpp"

#include <memory>
#include <string>
#include <utility>

namespace football
{

auto init(const std::string &config_filename) -> void;

template <typename T, typename... A> auto New(A &&...args) -> std::unique_ptr<T>
{
    return std::unique_ptr<T>(new T(std::forward<A>(args)...));
}

template <typename T, typename... A> auto NewShared(A &&...args) -> std::shared_ptr<T>
{
    return std::shared_ptr<T>(new T(std::forward<A>(args)...));
}

} // namespace football