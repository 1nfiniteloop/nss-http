#pragma once

#include <boost/json.hpp>
#include <string_view>

#include <iostream>

namespace boost::json
{
template <class T>
void extract(const object& obj, string_view key, T& dst)
{
  auto val = obj.at(key);
  if (val.is_null() or val.kind() == kind::null)
  {
    dst = T{};
  }
  else
  {
    dst = value_to<T>(val);
  }
}
} // namespace boost::json