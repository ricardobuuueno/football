#include "runner.hpp"

namespace pub
{

auto get_new_championship(publisher pub, const std::string &url) -> std::string
{
    std::string res{};
    switch (pub)
    {
    case publisher::gazeta_esportiva: {
        auto publisher = gazeta_esportiva{};
        res = publisher.get_championship(url);
    }
    break;
    case publisher::none: {
    }
    }

    return res;
}

} // namespace pub
