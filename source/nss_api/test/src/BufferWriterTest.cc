#include <algorithm>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sstream>

#include "BufferWriter.h"

using namespace nss_api;
using namespace testing;

TEST(BufferWriter, CheckOffset)
{
  constexpr std::size_t size{64};
  char buffer[size];
  BufferWriter writer{buffer, size};
  const std::string text{"Hello, world!"};
  char* first = writer.write(text);
  char* second = writer.write(text);
  EXPECT_EQ(buffer, first);
  EXPECT_EQ(buffer + text.size() + /*null-termination=*/1, second);
}

TEST(BufferWriter, CheckNullTermination)
{
  constexpr std::size_t size{64};
  char buffer[size];
  BufferWriter writer{buffer, size};
  const std::string text{"Hello, world!"};
  char* first = writer.write(text);
  char* second = writer.write(text);
  EXPECT_EQ(0, *(first + text.size()));
  EXPECT_EQ(0, *(second + text.size()));
}

TEST(BufferWriter, CheckArrayNullTermination)
{
  constexpr std::size_t size{64};
  char buffer[size];
  BufferWriter writer{buffer, size};
  const std::string text{"Hello, world!"};
  char** arr = writer.writeArray({text, text, text});
  EXPECT_EQ(0, arr[3]);
}

TEST(BufferWriter, WriteManyTexts)
{
  constexpr std::size_t size{64};
  char buffer[size];
  BufferWriter writer{buffer, size};
  const std::string text{"Hello, world!"};
  char* first = writer.write(text);
  char* second = writer.write(text);
  char* third = writer.write(text);
  EXPECT_EQ(text, first);
  EXPECT_EQ(text, second);
  EXPECT_EQ(text, third);
}

TEST(BufferWriter, WriteEmpty)
{
  constexpr std::size_t size{64};
  char buffer[size];
  BufferWriter writer{buffer, size};
  const std::string text{};
  char* first = writer.write(text);
  char* second = writer.write(text);
  char* third = writer.write(text);
  // expects to insert null termination when writing empty text:
  EXPECT_EQ(&buffer[0], first);
  EXPECT_EQ(&buffer[1], second);
  EXPECT_EQ(&buffer[2], third);
}

TEST(BufferWriter, TruncateTextWhenArrayIndexOverflow)
{
  constexpr std::size_t size{20}; // index takes 8 * (3+1) = 32 bytes
  char buffer[size];
  BufferWriter writer{buffer, size};
  const std::string text{"Hello, world!"};
  writer.writeArray({text, text, text});
  EXPECT_TRUE(writer.isTruncated());
}

TEST(BufferWriter, TruncateTextWhenOverflow)
{
  constexpr std::size_t size{20};
  char buffer[size];
  BufferWriter writer{buffer, size};
  const std::string text{"Hello, world!"};
  writer.write(text);
  char* truncatedText = writer.write(text);
  EXPECT_TRUE(writer.isTruncated());
  EXPECT_THAT(text, StartsWith(truncatedText));
}

TEST(BufferWriter, TruncateTextWhenArrayOverflow)
{
  constexpr std::size_t size{64};
  char buffer[size];
  BufferWriter writer{buffer, size};
  const std::string text{"Hello, world!"};
  char** arr = writer.writeArray({text, text, text});
  EXPECT_EQ(text, arr[0]);
  EXPECT_EQ(text, arr[1]);
  EXPECT_THAT(text, StartsWith(arr[2])); // text gets truncated.
}

TEST(BufferWriter, PointToBufferEndWhenOverflow)
{
  constexpr std::size_t size{16};
  char buffer[size];
  BufferWriter writer{buffer, size};
  const std::string text{"Hello, world!"};
  writer.write(text);
  writer.write(text);
  char* pos = writer.write(text);
  char* last = buffer + size - 1;
  EXPECT_EQ(last, pos);
  EXPECT_EQ(*last, 0);
}

TEST(BufferWriter, PointToBufferEndWhenArrayOverflow)
{
  constexpr std::size_t size{40}; // index takes 8 * (3+1) = 32 bytes
  char buffer[size];
  BufferWriter writer{buffer, size};
  const std::string text{"Hello, world!"};
  char** arr = writer.writeArray({text, text, text});
  char* last = buffer + size - 1;
  // arr[0] contains truncated text.
  EXPECT_EQ(last, arr[1]);
  EXPECT_EQ(last, arr[2]);
  EXPECT_EQ(*last, 0);
}

TEST(BufferWriter, TextFitsExcactly)
{
  constexpr std::size_t size{12};
  char buffer[size];
  BufferWriter writer{buffer, size};
  const std::string text{"Hey"};
  char* first = writer.write(text);  // +4 bytes
  char* second = writer.write(text); // +4 bytes
  char* third = writer.write(text);  // +4 bytes
  EXPECT_EQ(text, first);
  EXPECT_EQ(text, second);
  EXPECT_EQ(text, third);
  // all text fits, but it is flagged as truncated anyway to avoid
  // complicated corner cases in implementation for recurrent writes.
  EXPECT_TRUE(writer.isTruncated());
}
