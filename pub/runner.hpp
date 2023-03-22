#pragma once

#include "gazeta_esportiva.hpp"
#include "publisher_base.hpp"

namespace pub
{

auto get_new_championship(publisher pub, const std::string &url) -> std::string;

} // namespace pub