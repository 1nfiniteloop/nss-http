#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace unix_account
{
struct Passwd
{
  std::string name{};
  std::string password{"x"};
  std::uint32_t uid{};
  std::uint32_t gid{};
  std::string gecos{};
  std::string dir{};
  std::string shell{};
};
} // namespace unix_account