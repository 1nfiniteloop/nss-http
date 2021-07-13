#pragma once

#include <string>

#include "unix_account/Passwd.h"
#include "unix_account/PasswdReader.h"

#include "Configuration.h"
#include "HttpRequest.h"

namespace unix_account
{
class PasswdReaderHttp final
    : public PasswdReader
{
public:
  HttpRequest _http;

  PasswdReaderHttp(Configuration config)
      : _http{GetUrl(config.endpoints)}
  {
    if (config.tls)
    {
      _http.setTlsConfig(*config.tls);
    }
  }

  cpr::Url GetUrl(const Configuration::Endpoints endpoints) const
  {
    return {endpoints.url + endpoints.user};
  }

  ~PasswdReaderHttp() = default;

  PasswdReaderHttp(const PasswdReaderHttp& cls) = delete;            // Copy-constructor
  PasswdReaderHttp& operator=(const PasswdReaderHttp& cls) = delete; // Copy-assignment constructor
  PasswdReaderHttp(PasswdReaderHttp&& cls) = delete;                 // Move-constructor
  PasswdReaderHttp& operator=(PasswdReaderHttp&& cls) = delete;      // Move-assignment constructor

  std::optional<Passwd> GetById(std::uint32_t gid) override
  {
    return _http.Get<Passwd>({{"id", std::to_string(gid)}});
  }

  std::optional<Passwd> GetByName(const std::string& name) override
  {
    return _http.Get<Passwd>({{"name", name}});
  }

  std::vector<Passwd> getAll() override
  {
    return _http.Get<Passwd>();
  }
};
} // namespace unix_account
