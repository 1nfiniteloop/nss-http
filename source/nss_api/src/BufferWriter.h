#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace nss_api
{
class BufferWriter final
{
private:
  using byte_t = char;
  byte_t* _cursor{};
  byte_t* const _end{};

public:
  BufferWriter(byte_t* begin, std::size_t size);
  ~BufferWriter() = default;

  BufferWriter(const BufferWriter& cls) = delete;            // Copy-constructor
  BufferWriter& operator=(const BufferWriter& cls) = delete; // Copy-assignment constructor
  BufferWriter(BufferWriter&& cls) = delete;                 // Move-constructor
  BufferWriter& operator=(BufferWriter&& cls) = delete;      // Move-assignment constructor

  byte_t** writeArray(const std::vector<std::string>& textFields);

  byte_t* write(const std::string& text)
  {
    return _write(text);
  }

  bool isTruncated() const
  {
    return _cursor >= _end;
  }

private:
  byte_t* _write(const std::string& text);
  void _try_write(const std::string& text);
  std::size_t getAvailable() const;
};
} // namespace nss_api
