#include "mysql.hpp"

#include <cppconn/driver.h>
#include <fmt/core.h>
#include <optional>
#include <ctime>

#include "../plog/Log.h"
#include "../util/toml.hpp"
#include "../constants.hpp"

namespace mysql
{

	std::string random_string(const int len) 
	{
		static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

		srand((unsigned)time(NULL) * getpid()); 
		std::string tmp_s;
		tmp_s.reserve(len);

		for (int i = 0; i < len; ++i) {
			tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
		}
		
		return tmp_s;
	}

//SERVER ----------------------------------------------------------------------------------------

	void server::connect()
	{
		if (!driver) {
			driver = get_driver_instance();
		}

		if (!connection) {

			toml::table config = toml::parse_file(CONFIG_FILE);
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

	bool server::has_rows() const
	{
		PLOG_DEBUG << result->rowsCount();
		return (result != nullptr && result->rowsCount() > 0);
	}

	void server::get_values(std::map<std::string, std::string>& field_values) const
	{
		auto meta = result->getMetaData();
		PLOG_DEBUG << "Column count: " << meta->getColumnCount();

		while (result->next()) 
		{
			for (int i{1}; i <= meta->getColumnCount(); ++i) {
				auto field = meta->getColumnName(i);
				auto value = result->getString(i);
				field_values[field] = value;
			}
			break;
		}
	}


//TABLE ----------------------------------------------------------------------------------------

	bool table::start(const std::string& field_name, const std::string& value)
	{		
		std::string stmt = fmt::format("SELECT {} FROM {} WHERE {} = '{}' LIMIT 1;", fields(), tablename, field_name, value);
		
		if (select(stmt)) {
			return true;
		}
		
		set(field_name, value);

		return false;
	}

	std::string table::get(const std::string& field_name) const
	{
		return fields_values.at(field_name);
	}

	void table::set(const std::string& field_name, const std::string& value)
	{
		if (!value.empty()) {
			fields_values[field_name] = value;
		}
	}

	bool table::save()
	{
		populate();

		if (insert()) {
			if (reload()) {
				reset();
				return true;
			}
		}

		return false;
	}

	bool table::select(const std::string& stmt)
	{
		PLOG_DEBUG << stmt;

		srv.connect();
		srv.prepare(stmt);
		srv.execute();

		if (srv.has_rows())
		{
			srv.get_values(fields_values);
			return true;
		}

		return false;
	}

	bool table::insert()
	{
		std::string stmt = fmt::format("REPLACE INTO {} ({}) VALUES ({});", tablename, fields(), values());
		PLOG_DEBUG << stmt;

		srv.connect();
		srv.prepare(stmt);
		srv.execute();

		return true;
	}

	bool table::reload()
	{
		std::string stmt = fmt::format("SELECT * FROM {} WHERE {} = '{}' LIMIT 1;", tablename, key_field, fields_values[key_field]);
		return select(stmt);
	}

	std::string table::fields() const
	{
		if (fields_values.empty()) {
			return {"*"};
		}

		std::string result{};

		size_t count{0};
		for (const auto& [field, value] : fields_values) {
			result += field;
			if (++count < fields_values.size()) {
				result += ", ";
			}
		}

		return result;
	}

	std::string table::values() const
	{
		std::string result{};

		size_t count{0};
		for (const auto& [field, value] : fields_values) {
			result += ("'" + value + "'");
			if (++count < fields_values.size()) {
				result += ", ";
			}
		}

		return result;
	}

}
