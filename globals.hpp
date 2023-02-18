#pragma once

#include "plog/Log.h"
#include "plog/Initializers/RollingFileInitializer.h"
#include "util/toml.hpp"

#include <string>
#include <memory>
#include <utility>

namespace football
{

    auto init(const std::string& config_filename) -> void;

    template <typename T, typename... A>
	auto New(A&& ... args) -> std::unique_ptr<T> 
	{ 
		return std::unique_ptr<T>(new T(std::forward<A>(args)...)); 
	}

}