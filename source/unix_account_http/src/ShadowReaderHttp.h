#pragma once

#include <string>

#include "unix_account/Shadow.h"
#include "unix_account/ShadowReader.h"

#include "Configuration.h"
#include "HttpRequest.h"

namespace unix_account
{
class ShadowReaderHttp final
    : public ShadowReader
{
public:
  HttpRequest _http;
  const std::optional<std::string> _apiToken{};
  const bool _hasApiToken{};

  ShadowReaderHttp(Configuration config)
      : _http{GetUrl(config.endpoints)},
        _hasApiToken{not config.apiToken.empty()}
  {
    if (config.tls)
    {
      _http.setTlsConfig(*config.tls);
    }
    if (config.apiToken.size())
    {
      _http.setApiToken(config.apiToken);
    }
  }

  cpr::Url GetUrl(const Configuration::Endpoints endpoints) const
  {
    return {endpoints.url + endpoints.password};
  }

  ~ShadowReaderHttp() = default;

  ShadowReaderHttp(const ShadowReaderHttp& cls) = delete;            // Copy-constructor
  ShadowReaderHttp& operator=(const ShadowReaderHttp& cls) = delete; // Copy-assignment constructor
  ShadowReaderHttp(ShadowReaderHttp&& cls) = delete;                 // Move-constructor
  ShadowReaderHttp& operator=(ShadowReaderHttp&& cls) = delete;      // Move-assignment constructor

  std::optional<Shadow> GetByName(const std::string& name) override
  {
    if (_hasApiToken)
    {
      return _http.Get<Shadow>({{"name", name}});
    }
    else
    {
      return {};
    }
  }

  std::vector<Shadow> getAll() override
  {
    if (_hasApiToken)
    {
      return _http.Get<Shadow>();
    }
    else
    {
      return {};
    }
  }
};
} // namespace unix_account
