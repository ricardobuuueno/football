#include "mysql.hpp"

#include <cppconn/driver.h>
#include <fmt/core.h>
#include <optional>

#include "../util/toml.hpp"

namespace mysql
{

//SERVER ----------------------------------------------------------------------------------------

	void server::connect()
	{
		if (!driver) {
			driver = get_driver_instance();
		}

		if (!connection) {

			toml::table config = toml::parse_file("football.toml");
			auto my_config = config["mysql"];

			auto server = my_config["server"].value<std::string>();
			auto port = my_config["port"].value<std::string>();
			auto username = my_config["username"].value<std::string>();
			auto password = my_config["password"].value<std::string>();
			auto database = my_config["database"].value<std::string>();

			connection = driver->connect(server.value(), username.value(), password.value());

			connection->setSchema(database.value());

		}
		else if (connection->isClosed()) {
			connection->reconnect();
		}

	}

	void server::prepare(const std::string& stmt)
	{
		pstmt = connection->prepareStatement(stmt);
	}

	void server::execute()
	{
		result = pstmt->executeQuery();
	}

//TABLE ----------------------------------------------------------------------------------------

	bool table::select(const std::string& field_name, const std::string& value)
	{
		set(field_name, value);

		std::string stmt = fmt::format("SELECT {} FROM {} WHERE {} = '{}';", fields(), tablename, field_name, value);

		srv.connect();
		srv.prepare(stmt);

		return false;
	}

	std::string table::get(const std::string& field_name) const
	{
		return {};
	}

	void table::set(const std::string& field_name, const std::string& value)
	{
		fields_values[field_name] = value;
	}

	bool table::execute()
	{
		return false;
	}

	std::string table::fields() const
	{
		std::string result{};

		for (const auto& [field, value] : fields_values) {
			result += (field + ", ");
		}

		return result;
	}

}
