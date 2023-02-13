#pragma once

#include "plog/Log.h"
#include "plog/Initializers/RollingFileInitializer.h"
#include "util/toml.hpp"

#include <string>

namespace football
{

    auto init(const std::string& config_filename) -> void;

}