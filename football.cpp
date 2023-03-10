
#include "classes/club.hpp"
#include "net/football_server.hpp"
#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"
#include "util/globals.hpp"
#include "util/toml.hpp"

#include <filesystem>
#include <iostream>

int main(int argc, char *argv[]) // NOLINT
{
    using namespace football;

    auto app_path = std::filesystem::path(argv[0]);
    std::string config_filename{app_path.filename()};
    config_filename += ".toml";

    football::init(config_filename);

    club sp{"São Paulo", "BR"};
    bool saved = sp.save();

    std::cout << "Club: " << sp.id() << " " << sp.name() << '\n';

    net::football_server server(60000);
    server.start();

    while (true)
    {
        server.update(-1, true);
    }

    return 0;
}
