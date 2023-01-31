#pragma once

#include "mysql_connection.h"

const std::string server_url = "";
const std::string username = "";
const std::string password = "";
const std::string database = "";

namespace mysql
{

	class server final
	{
	public:

		void connect();

	private:
		sql::Driver* driver{ nullptr };
		sql::Connection* connection{ nullptr };
		sql::Statement* stmt{ nullptr };
		sql::PreparedStatement* pstmt{ nullptr };

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

		template <typename T>
		bool select(const std::string&& field_name, T&& value)
		{
			return false;
		}

		template <typename T>
		T get(const std::string&& field_name)
		{
			return T{};
		}

		template <typename T>
		void set(const std::string&& field_name, T&& value)
		{

		}

		virtual bool save() = 0;

		[[nodiscard]] bool go()
		{
			return false;
		}

	private:
		const std::string tablename;

		server srv{};

	};

}