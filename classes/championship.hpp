#pragma once

#include <string>

#include "club.hpp"

namespace football
{

	class championship final : public mysql::table
	{
	public:
		championship(const std::string& championship_name, const std::string country_code) 
			: table("championships", "name"), _id(""), _name(championship_name), _country(country_code)
		{
			if (start("name", _name))
			{
				_id = get("id");
				_country = get("country");
			}
		}

		championship(const std::string& id) 
			: table("championships", "name"), _id(id), _name(""), _country("")
		{
			if (start("id", _id))
			{
				_name = get("name");
				_country = get("country");
			}
		}

		championship() : championship("") {}

		championship(const championship& other)
			: table("championships", "name")
		{
			_id = other.id();
			_name = other.name();
			_country = other.country();
		}

		championship& operator=(const championship& other) 
		{
			_id = other.id();
			_name = other.name();
			_country = other.country();		
			return *this;	
		}

		std::string id() const { return _id; }
		std::string name() const { return _name; }
		std::string country() const { return _country; }

		bool empty() override
		{
			return (_id.empty() && _name.empty() && _country.empty());
		}

	private:
		std::string _id;
		std::string _name;
		std::string _country;

		void populate() override
		{
			set("id", _id);
			set("name", _name);
			set("country", _country);
		}

		void reset() override
		{
			_id = get("id");
			_name = get("name");
			_country = get("country");
		}
	};

}