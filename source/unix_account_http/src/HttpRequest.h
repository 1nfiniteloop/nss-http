#pragma once

#include <cpr/bearer.h>
#include <cpr/cpr.h>
#include <cpr/cprtypes.h>
#include <cpr/session.h>
#include <string_view>

#include "Configuration.h"
#include "JsonValueTo.h"

namespace unix_account
{
class HttpRequest final
{
private:
  cpr::Session _session{};
  cpr::Url _url{};

public:
  HttpRequest(cpr::Url url);
  ~HttpRequest() = default;

  HttpRequest(const HttpRequest& cls) = delete;            // Copy-constructor
  HttpRequest& operator=(const HttpRequest& cls) = delete; // Copy-assignment constructor
  HttpRequest(HttpRequest&& cls) = delete;                 // Move-constructor
  HttpRequest& operator=(HttpRequest&& cls) = delete;      // Move-assignment constructor

  void setTlsConfig(const Configuration::Tls& tls);
  void setApiToken(const std::string& apiToken);

  /**
   * Get one item
   */
  template <typename T>
  std::optional<T> Get(cpr::Parameters parameters)
  {
    return JsonValueTo<T>(HttpGet(std::move(parameters)));
  }

  /**
   * Get all items
   */
  template <typename T>
  std::vector<T> Get()
  {
    return JsonValueToVector<T>(HttpGet());
  }

private:
  std::string HttpGet(cpr::Parameters parameters = {});
};
} // namespace unix_account