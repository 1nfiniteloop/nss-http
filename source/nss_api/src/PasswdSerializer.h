#pragma once

#include <cstdint>

#include "BufferWriter.h"
#include "unix_account/Passwd.h"

struct passwd;

namespace nss_api
{
class PasswdSerializer final
{
private:
  passwd& _passwd;
  BufferWriter _writer;

public:
  PasswdSerializer(
      passwd& passwd,
      char* buffer,
      std::size_t size);
  ~PasswdSerializer() = default;

  PasswdSerializer(const PasswdSerializer& cls) = delete;            // Copy-constructor
  PasswdSerializer& operator=(const PasswdSerializer& cls) = delete; // Copy-assignment constructor
  PasswdSerializer(PasswdSerializer&& cls) = delete;                 // Move-constructor
  PasswdSerializer& operator=(PasswdSerializer&& cls) = delete;      // Move-assignment constructor

  PasswdSerializer& operator<<(const unix_account::Passwd& passwd)
  {
    append(passwd);
    return *this;
  }

  bool isTruncated() const { return _writer.isTruncated(); }

private:
  void append(const unix_account::Passwd& passwd);
};
} // namespace nss_api