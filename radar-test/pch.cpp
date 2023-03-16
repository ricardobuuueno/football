//
// pch.cpp
//

#include "pch.h"

namespace test
{

auto sleep(int secs) -> void
{
    std::this_thread::sleep_for(std::chrono::seconds(secs));
}

} // namespace test