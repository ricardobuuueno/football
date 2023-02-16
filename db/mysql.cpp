#include "mysql.hpp"

#include <cppconn/driver.h>
#include <optional>

#include "../util/toml.hpp"

namespace mysql
{
	std::unique_ptr<environment> env = nullptr;

	auto init(const std::string& config_filename) -> void
	{
		env = std::make_unique<environment>();

		toml::table config = toml::parse_file(config_filename);
		auto my_config = config["mysql"];

		env->server = my_config["server"].value<std::string>().value();
		env->port = my_config["port"].value<std::string>().value();
		env->username = my_config["username"].value<std::string>().value();
		env->password = my_config["password"].value<std::string>().value();
		env->database = my_config["database"].value<std::string>().value();
	}

//SERVER ----------------------------------------------------------------------------------------

	void server::connect()
	{
		if (!driver) {
			driver = get_driver_instance();
		}

		if (!connection) {

			connection = driver->connect(env->server, env->username, env->password);

			connection->setSchema(env->database);

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

	auto server::has_rows() const -> bool
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

	auto table::start(const std::string& field_name, const std::string& value) -> bool
	{		
		if (value.empty()) {
			return false;
		}

		std::string stmt = fmt::format("SELECT {} FROM {} WHERE {} = '{}' LIMIT 1;", fields(), tablename, field_name, value);
		
		if (select(stmt)) {
			return true;
		}
		
		set(field_name, value);

		return false;
	}

	auto table::get(const std::string& field_name) const -> std::string
	{
		if (auto search = fields_values.find(field_name); search != fields_values.end()) {
			return search->second;
		}
		return {};
	}

	void table::set(const std::string& field_name, const std::string& value)
	{
		if (!value.empty()) {
			fields_values[field_name] = value;
		}
	}

	auto table::save() -> bool
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

	auto table::remove() -> bool
	{
		std::string stmt = fmt::format("DELETE FROM {} WHERE {} = '{}';", tablename, key_field, fields_values[key_field]);

		srv.connect();
		srv.prepare(stmt);
		srv.execute();

		fields_values.clear();
		reset();

		return true;
	}

	auto table::select(const std::string& stmt) -> bool
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

	auto table::insert() -> bool
	{
		std::string stmt = fmt::format("REPLACE INTO {} ({}) VALUES ({});", tablename, fields(), values());
		PLOG_DEBUG << stmt;

		srv.connect();
		srv.prepare(stmt);
		srv.execute();

		return true;
	}

	auto table::reload() -> bool
	{
		std::string stmt = fmt::format("SELECT * FROM {} WHERE {} = '{}' LIMIT 1;", tablename, key_field, fields_values[key_field]);
		return select(stmt);
	}

	auto table::fields() const -> std::string
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

	auto table::values() const -> std::string
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
