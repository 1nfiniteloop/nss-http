#pragma once

#include <optional>
#include <string>
#include <vector>

#include "Group.h"

namespace unix_account
{
class GroupReader
{
public:
  virtual ~GroupReader() = default;

  virtual std::optional<Group> GetById(std::uint32_t gid) = 0;
  virtual std::optional<Group> GetByName(const std::string& name) = 0;
  virtual std::vector<Group> getAll() = 0;
};
} // namespace unix_account