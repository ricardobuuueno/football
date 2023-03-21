#pragma once

#include "gazeta_esportiva.hpp"
#include "publisher_base.hpp"

namespace pub
{

auto get_new_championship(publisher pub, const std::string &url) -> std::list<std::string>
{
    std::list<std::string> res{};
    switch (pub)
    {
    case publisher::gazeta_esportiva: {
        res = gazeta_esportiva gaz{};
    }
    break;
    case publisher::none: {
    }
    }

    return res;
}

} // namespace pub