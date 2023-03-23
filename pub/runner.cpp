#include "runner.hpp"

namespace pub
{

auto get_new_championship(publisher pub, const std::string &url) -> std::tuple<bool, std::string, std::string>
{
    std::tuple<bool, std::string, std::string> result;
    switch (pub)
    {
    case publisher::gazeta_esportiva: {
        auto publisher = gazeta_esportiva{};
        result = publisher.get_championship(url);
    }
    break;
    case publisher::none: {
    }
    }

    return result;
}

} // namespace pub
