#pragma once

#include <cstdint>

#include "BufferWriter.h"
#include "unix_account/Group.h"

struct group;

namespace nss_api
{
class GroupSerializer final
{
private:
  group& _group;
  BufferWriter _writer;

public:
  GroupSerializer(
      group& group,
      char* buffer,
      std::size_t size);
  ~GroupSerializer() = default;

  GroupSerializer(const GroupSerializer& cls) = delete;            // Copy-constructor
  GroupSerializer& operator=(const GroupSerializer& cls) = delete; // Copy-assignment constructor
  GroupSerializer(GroupSerializer&& cls) = delete;                 // Move-constructor
  GroupSerializer& operator=(GroupSerializer&& cls) = delete;      // Move-assignment constructor

  GroupSerializer& operator<<(const unix_account::Group& group)
  {
    append(group);
    return *this;
  }

  bool isTruncated() const { return _writer.isTruncated(); }

private:
  void append(const unix_account::Group& group);
};
} // namespace nss_api