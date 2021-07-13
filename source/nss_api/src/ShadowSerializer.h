#pragma once

#include <cstdint>

#include "BufferWriter.h"
#include "unix_account/Shadow.h"

struct spwd;

namespace nss_api
{
class ShadowSerializer final
{
private:
  spwd& _shadow;
  BufferWriter _writer;

public:
  ShadowSerializer(
      spwd& shadow,
      char* buffer,
      std::size_t size);
  ~ShadowSerializer() = default;

  ShadowSerializer(const ShadowSerializer& cls) = delete;            // Copy-constructor
  ShadowSerializer& operator=(const ShadowSerializer& cls) = delete; // Copy-assignment constructor
  ShadowSerializer(ShadowSerializer&& cls) = delete;                 // Move-constructor
  ShadowSerializer& operator=(ShadowSerializer&& cls) = delete;      // Move-assignment constructor

  ShadowSerializer& operator<<(const unix_account::Shadow& shadow)
  {
    append(shadow);
    return *this;
  }

  bool isTruncated() const { return _writer.isTruncated(); }

private:
  void append(const unix_account::Shadow& shadow);
};
} // namespace nss_api