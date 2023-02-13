#include "util.hpp"

#include <array>
#include <ctime>
#include <unistd.h>

namespace util
{

	auto random_string(const int len) -> std::string
	{
		static const std::array<char, 63> alphanum = {"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"};

		srand((unsigned)time(nullptr) * getpid()); 
		std::string tmp_s;
		tmp_s.reserve(len);

		for (int i = 0; i < len; ++i) {
			tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
		}
		
		return tmp_s;
	}

}