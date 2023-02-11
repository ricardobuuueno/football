
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
	auto severity = log_config["severity"].value<int>();
	auto file = log_config["file"].value<std::string>();
	auto maxFileSize = log_config["maxFileSize"].value<size_t>();

	plog::init(static_cast<plog::Severity>(severity.value()), file.value().c_str(), maxFileSize.value(), 3);

	club sp{"São Paulo"};
	bool saved = sp.save();

    std::cout << "Club: " << sp.id() << " " << sp.name() << '\n';
}

