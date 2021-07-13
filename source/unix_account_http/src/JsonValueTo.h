#pragma once

#include <boost/json.hpp>
#include <exception>
#include <optional>
#include <string>
#include <system_error>

#include "unix_account/Exception.h"
#include "unix_account/Group.h"
#include "unix_account/Passwd.h"
#include "unix_account/Shadow.h"

using namespace unix_account;

namespace boost::json
{
Group tag_invoke(const value_to_tag<Group>&, const value& jsonValue);
Passwd tag_invoke(const value_to_tag<Passwd>&, const value& jsonValue);
Shadow tag_invoke(const value_to_tag<Shadow>&, const value& jsonValue);
} // namespace boost::json

namespace unix_account
{
template <typename T>
std::optional<T> JsonValueTo(const std::string& text)
{
  if (text.size())
  {
    try
    {
      return boost::json::value_to<T>(
          boost::json::parse(text));
    }
    catch (std::exception& err)
    {
      throw UnixAccountLookupFailed(std::string{"Json conversion error: "} + err.what());
    }
  }
  else
  {
    return {};
  }
}

template <typename T>
std::vector<T> JsonValueToVector(const std::string& text)
{
  if (text.size())
  {
    try
    {
      auto jv = boost::json::parse(text);
      auto groups = jv.as_object().at("all");
      return boost::json::value_to<std::vector<T>>(
          groups);
    }
    catch (std::exception& err)
    {
      throw UnixAccountLookupFailed(std::string{"Json conversion error: "} + err.what());
    }
  }
  else
  {
    return {};
  }
}
} // namespace unix_account