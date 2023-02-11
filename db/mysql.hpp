#pragma once

#include <map>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

namespace mysql
{

	std::string random_string(const int len);

	class server final
	{
	public:

		void connect();
		void prepare(const std::string& stmt);
		void execute();
		
		bool has_rows() const;
		void get_values(std::map<std::string, std::string>& field_values) const;

	private:
		sql::Driver* driver{ nullptr };
		sql::Connection* connection{ nullptr };
		sql::Statement* stmt{ nullptr };
		sql::PreparedStatement* pstmt{ nullptr };
		sql::ResultSet* result{ nullptr };

	};

	class table
	{
	public:
		table(const std::string& name, const std::string& field)
			: tablename(name), key_field(field) 
		{
		}

		virtual ~table() {}

		[[nodiscard]] bool save();

	protected:

		bool start(const std::string& field_name, const std::string& value);

		std::string get(const std::string& field_name) const;

		void set(const std::string& field_name, const std::string& value);

		virtual void populate() = 0;
		virtual void reset() = 0;

	private:
		const std::string tablename;
		const std::string key_field;
		std::map<std::string, std::string> fields_values{};

		server srv{};

		std::string fields() const;
		std::string values() const;

		bool select(const std::string& stmt);
		bool insert();
		bool reload();

	};

}