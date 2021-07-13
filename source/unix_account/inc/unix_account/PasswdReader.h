#pragma once

#include <optional>
#include <string>
#include <vector>

#include "Passwd.h"

namespace unix_account
{
class PasswdReader
{
public:
  virtual ~PasswdReader() = default;

  virtual std::optional<Passwd> GetById(std::uint32_t uid) = 0;
  virtual std::optional<Passwd> GetByName(const std::string& name) = 0;
  virtual std::vector<Passwd> getAll() = 0;
};
} // namespace unix_account