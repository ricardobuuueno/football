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

} // namespace football