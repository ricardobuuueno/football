#pragma once

#include "mysql.hpp"

#include <string>

namespace football
{

	class club final : public mysql::table
	{
	public:
		club(const std::string& club_name) 
			: table("clubs"), _id(""), _name(club_name)
		{
			if (select("name", _name))
			{
				_id = get("id");
			}
		}

		club() : club("") {}

		bool save() override
		{
			set("id", _id);
			set("name", _name);
			return execute();
		}

		std::string id() const { return _id; }
		std::string name() const { return _name; }

		void name(const std::string& club_name) { _name = club_name; }

	private:
		std::string _id;
		std::string _name;

	};

}
