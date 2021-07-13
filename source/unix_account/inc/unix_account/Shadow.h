#pragma once

#include <cstdint>
#include <ctime>
#include <string>
#include <vector>

// man 3 getspent
namespace unix_account
{
struct Shadow
{
  std::string name{};
  std::string password{"!"};
  std::time_t timeLastChange{};
  std::uint32_t daysMin{};
  std::uint32_t daysMax{};
  std::uint32_t daysWarn{};
  std::uint32_t daysInactive{};
  std::time_t timeExpires{};
};
} // namespace unix_account