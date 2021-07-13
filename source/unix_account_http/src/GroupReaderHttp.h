#pragma once

#include <string>

#include "unix_account/Group.h"
#include "unix_account/GroupReader.h"

#include "Configuration.h"
#include "HttpRequest.h"

namespace unix_account
{
class GroupReaderHttp final
    : public GroupReader
{
public:
  HttpRequest _http;

  GroupReaderHttp(Configuration config)
      : _http{GetUrl(config.endpoints)}
  {
    if (config.tls)
    {
      _http.setTlsConfig(*config.tls);
    }
  }

  cpr::Url GetUrl(const Configuration::Endpoints endpoints) const
  {
    return {endpoints.url + endpoints.group};
  }

  ~GroupReaderHttp() = default;

  GroupReaderHttp(const GroupReaderHttp& cls) = delete;            // Copy-constructor
  GroupReaderHttp& operator=(const GroupReaderHttp& cls) = delete; // Copy-assignment constructor
  GroupReaderHttp(GroupReaderHttp&& cls) = delete;                 // Move-constructor
  GroupReaderHttp& operator=(GroupReaderHttp&& cls) = delete;      // Move-assignment constructor

  std::optional<Group> GetById(std::uint32_t gid) override
  {
    return _http.Get<Group>({{"id", std::to_string(gid)}});
  }

  std::optional<Group> GetByName(const std::string& name) override
  {
    return _http.Get<Group>({{"name", name}});
  }

  std::vector<Group> getAll() override
  {
    return _http.Get<Group>();
  }
};
} // namespace unix_account
