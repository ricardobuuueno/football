#pragma once

#include <map>
#include <fmt/core.h>

#include "../plog/Log.h"

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

namespace mysql
{

	struct environment final
	{
		std::string server;
		std::string port;
		std::string username;
		std::string password;
		std::string database;		
	};

	extern std::unique_ptr<environment> env;

	auto init(const std::string& config_filename) -> void;

	class server final
	{
	public:

		void connect();
		void prepare(const std::string& stmt);
		void execute();
		
		auto has_rows() const -> bool;
		void get_values(std::map<std::string, std::string>& field_values) const;

	private:
		sql::Driver* driver{ nullptr };
		sql::Connection* connection{ nullptr };
		sql::Statement* stmt{ nullptr };
		sql::PreparedStatement* pstmt{ nullptr };
		sql::ResultSet* result{ nullptr };

		static std::string configuration_filename;

	};

	class table
	{
	public:
		table(const std::string& name, const std::string& field)
			: tablename(name), key_field(field) 
		{
			PLOG_DEBUG << fmt::format("[DB] Table {} [C]onstructor.", tablename);
		}

		virtual ~table() {
			PLOG_DEBUG << fmt::format("[DB] Table {} [D]estructor.", tablename);
		}

		[[nodiscard]] auto save() -> bool;
		[[nodiscard]] auto remove() -> bool;

		virtual bool empty() = 0;

	protected:

		auto start(const std::string& field_name, const std::string& value) -> bool;

		auto get(const std::string& field_name) const -> std::string;

		void set(const std::string& field_name, const std::string& value);

		virtual void populate() = 0;
		virtual void reset() = 0;

	private:
		const std::string tablename;
		const std::string key_field;
		std::map<std::string, std::string> fields_values{};

		server srv{};

		auto fields() const -> std::string;
		auto values() const -> std::string;

		auto select(const std::string& stmt) -> bool;
		auto insert() -> bool;
		auto reload() -> bool;

	};

}