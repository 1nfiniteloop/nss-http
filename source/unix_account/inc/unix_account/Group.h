#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace unix_account
{
struct Group
{
  std::string name{};
  std::string password{"x"};
  std::uint32_t gid{};
  std::vector<std::string> members{};
};
} // namespace unix_account