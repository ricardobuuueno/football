#include <iostream>

#include "./pub/gazeta_esportiva.hpp"
#include "./util/globals.hpp"

int main(int argc, char *argv[])
{
    auto app_path = std::filesystem::path(argv[0]);
    std::string config_filename{app_path.filename()};
    config_filename += ".toml";

    football::init(config_filename);

    pub::gazeta_esportiva gazeta{};
    auto championship = gazeta.get_championship("https://www.gazetaesportiva.com/campeonatos/paulista/");
    std::cout << std::get<2>(championship) << '\n';

    auto clubs = gazeta.get_championship_clubs("https://www.gazetaesportiva.com/campeonatos/paulista/");

    for (auto const &club : std::get<2>(clubs))
    {
        std::cout << club << '\n';
    }

    return 0;
}