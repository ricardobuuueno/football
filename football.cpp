
#include "classes/club.hpp"
#include "plog/Log.h"
#include "plog/Initializers/RollingFileInitializer.h"
#include "util/toml.hpp"
#include "globals.hpp"

#include <iostream>
#include <filesystem>

int main(int argc, char* argv[])
{
	using namespace football;

	auto app_path = std::filesystem::path(argv[0]);
	std::string config_filename{app_path.filename()};
	config_filename += ".toml";

	football::init(config_filename);

	club sp{"São Paulo"};
	bool saved = sp.save();

    std::cout << "Club: " << sp.id() << " " << sp.name() << '\n';
}
