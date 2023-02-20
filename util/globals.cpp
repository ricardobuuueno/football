#include "globals.hpp"
#include "../db/mysql.hpp"

namespace football
{

auto init(const std::string &config_filename) -> void
{
    toml::table config = toml::parse_file(config_filename);
    auto log_config = config["log"];
    auto severity = log_config["severity"].value<int>().value();
    auto file = log_config["file"].value<std::string>().value();
    auto maxFileSize = log_config["maxFileSize"].value<size_t>().value();
    auto maxFiles = log_config["maxFiles"].value<int>().value();

    plog::init(static_cast<plog::Severity>(severity), file.c_str(), maxFileSize, maxFiles);
    PLOG_DEBUG << "Log engine initialized [severity: " << severity << ", file: " << file
               << ", maxFileSize: " << maxFileSize << ", maxFiles: " << maxFiles << "]";

    mysql::init(config_filename);
    PLOG_DEBUG << "MySQL engine initialized ";
}

} // namespace football