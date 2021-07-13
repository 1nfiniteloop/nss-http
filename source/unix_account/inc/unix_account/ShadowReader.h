#pragma once

#include <optional>
#include <string>
#include <vector>

#include "Shadow.h"

namespace unix_account
{
class ShadowReader
{
public:
  virtual ~ShadowReader() = default;

  virtual std::optional<Shadow> GetByName(const std::string& name) = 0;
  virtual std::vector<Shadow> getAll() = 0;
};
} // namespace unix_account