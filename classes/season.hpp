#pragma once

#include "championship.hpp"
#include "club.hpp"

#include <string>
#include <vector>

namespace football
{

	class season final: public mysql::table
	{
	public:
		season(const championship& championship, const std::string& year) 
			: table("seasons", "championship", "year"), _id(""), _championship(championship), _year(year), _clubs{}
		{
			if (start("championship", _championship.id(), "year", _year))
			{
				_id = get("id");
				_year = get("year");
			}
		}

		season() : season({},"") {}

		std::string id() const { return _id; }
		std::string year() const { return _year; }
		const football::championship& championship() const { return _championship; }

		void add_club(football::club club) { _clubs.push_back(club); }
		const football::club& club_at(std::size_t index) const { return _clubs.at(index); }
		std::size_t club_count() { return _clubs.size(); }
		void remove_club(football::club club) { if (auto it = std::find(_clubs.begin(), _clubs.end(), club); it != _clubs.end()) { _clubs.erase(it); } }
		//void remove_club(football::club club) {}

		bool empty() override
		{
			return (_id.empty() && _championship.empty() && _year.empty() && _clubs.empty());
		}

	private:
		std::string _id;
		football::championship _championship;
		std::string _year;
		std::vector<club> _clubs;

		void populate() override
		{
			set("id", _id);
			set("championship", _championship.id());
			set("year", _year);
		}

		void reset() override
		{
			_id = get("id");
			_year = get("year");

			football::championship cs(get("championship"));
			_championship = cs;
		}		

	};

}