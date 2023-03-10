#include <iostream>

#include "./pub/gazeta_esportiva.hpp"

int main(int argc, char *argv[])
{
    pub::gazeta_esportiva gazeta{};
    auto clubs = gazeta.get_championship_clubs("paulista");

    for (auto const &club : clubs)
    {
        std::cout << club << '\n';
    }

    return 0;
}