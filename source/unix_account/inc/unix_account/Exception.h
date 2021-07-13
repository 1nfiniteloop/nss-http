#pragma once

#include <exception>
#include <stdexcept>

namespace unix_account
{
class UnixAccountLookupFailed : public std::runtime_error
{
public:
  UnixAccountLookupFailed(const std::string message)
      : std::runtime_error{message}
  {
  }
  ~UnixAccountLookupFailed() = default;
};
} // namespace unix_account
