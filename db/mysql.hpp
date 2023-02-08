#pragma once

#include <map>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

namespace mysql
{

	class server final
	{
	public:

		void connect();
		void prepare(const std::string& stmt);
		void execute();

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
		explicit table(const std::string& name)
			: tablename(name) 
		{
		}

		virtual ~table() {}

	protected:

		bool select(const std::string& field_name, const std::string& value);

		std::string get(const std::string& field_name) const;

		void set(const std::string& field_name, const std::string& value);

		virtual bool save() = 0;

		[[nodiscard]] bool execute();

	private:
		const std::string tablename;
		std::map<std::string, std::string> fields_values{};

		server srv{};

		std::string fields() const;

	};

}