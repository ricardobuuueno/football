#include "mysql.hpp"

#include <cppconn/driver.h>

#include <fmt/core.h>


namespace mysql
{

//SERVER ----------------------------------------------------------------------------------------

	void server::connect()
	{
		if (!driver) {
			driver = get_driver_instance();
		}

		if (!connection) {
			connection = driver->connect(server_url, username, password);
		}
		else if (connection->isClosed()) {
			connection->reconnect();
		}

		connection->setSchema(database);

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

		std::string stmt = fmt::format("SELECT {} FROM {} WHERE {} = {};", fields(), tablename, field_name, value);

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
