
#include <iostream>
#include "classes/club.hpp"
#include "plog/Log.h"
#include "plog/Initializers/RollingFileInitializer.h"
#include "util/toml.hpp"

#include "constants.hpp"

int main()
{
	using namespace football;

	toml::table config = toml::parse_file(CONFIG_FILE);
	auto log_config = config["log"];
	auto log_file = log_config["file"].value<std::string>();

	plog::init(plog::debug, log_file.value().c_str(), 5000, 3);

	club sp{"São Paulo"};
	bool saved = sp.save();

    std::cout << "Club: " << sp.id() << " " << sp.name() << '\n';
}

