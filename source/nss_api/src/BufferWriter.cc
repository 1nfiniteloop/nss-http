#include <algorithm>

#include "BufferWriter.h"

namespace nss_api
{
BufferWriter::BufferWriter(byte_t* begin, std::size_t size)
    : _cursor{begin},
      _end{begin + size}
{
  std::fill(_cursor, _end, 0);
}

BufferWriter::byte_t** BufferWriter::writeArray(const std::vector<std::string>& textFields)
{
  byte_t** arr = reinterpret_cast<byte_t**>(_cursor);
  _cursor += sizeof(byte_t*) * (textFields.size() + 1); // array null termination
  if (_cursor < _end)
  {
    for (std::size_t n{}; n < textFields.size(); ++n)
    {
      arr[n] = _write(textFields[n]);
    }
  }
  return arr;
}

BufferWriter::byte_t* BufferWriter::_write(const std::string& text)
{
  byte_t* begin = _cursor;
  if (_cursor < _end)
  {
    begin = _cursor;
    _try_write(text);
  }
  else
  {
    // point to last byte in buffer (which is a null termination) on recurrent
    // writes after overflow.
    begin = _end - 1;
  }
  return begin;
}

void BufferWriter::_try_write(const std::string& text)
{
  const std::size_t available = getAvailable();
  const std::size_t size = text.size();
  if (available)
  {
    if (size)
    {
      // text might get truncated:
      const std::size_t bytesToCopy = std::min(
          available,
          size);
      std::copy_n(
          text.begin(),
          bytesToCopy,
          _cursor);
    }
    _cursor += size + 1; // insert null termination after text (implicitly, since buffer contains all zeroes)
  }
}

std::size_t BufferWriter::getAvailable() const
{
  std::size_t available{};
  const byte_t* last = _end - 1; // always reserve space for last null termination in buffer
  if (_cursor < last)
  {
    available = last - _cursor;
  }
  return available;
}
} // namespace nss_api