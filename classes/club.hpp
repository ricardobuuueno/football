#pragma once

#include "player.hpp"

#include "../db/mysql.hpp"

#include <string>
#include <vector>

namespace football
{

	class club final : public mysql::table
	{
	public:
		club(const std::string& club_name) 
			: table("clubs", "name"), _id(""), _name(club_name), _players{}
		{
			if (start("name", _name))
			{
				_id = get("id");
			}
		}

		club() : club("") {}

		std::string id() const { return _id; }
		std::string name() const { return _name; }

		void name(const std::string& club_name) { _name = club_name; }

	private:
		std::string _id;
		std::string _name;
		std::vector<player> _players;

		void populate() override
		{
			set("id", _id);
			set("name", _name);
		}

		void reset() override
		{
			_id = get("id");
			_name = get("name");
		}

	};

}
