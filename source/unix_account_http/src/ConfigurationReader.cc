#include <fstream>
#include <string>
#include <yaml-cpp/exceptions.h>

#include "Configuration.h"
#include "ConfigurationReader.h"
#include "unix_account/Exception.h"

namespace unix_account
{
ConfigurationReader::ConfigurationReader(
    const std::string& configurationFile,
    const std::string& apiTokenFile)
    : _configurationFile{configurationFile},
      _apiTokenFile{apiTokenFile}
{
}

Configuration ConfigurationReader::read() const
{
  try
  {
    return tryRead();
  }
  catch (YAML::Exception& err)
  {
    throw UnixAccountLookupFailed("Failed to read or parse configuration: " + err.msg);
  }
}

Configuration::Endpoints readEndpointsFrom(const YAML::Node& node)
{
  Configuration::Endpoints endpoints{};
  auto config = node["endpoints"];
  endpoints.url = config["url"].as<std::string>();
  endpoints.group = config["group"].as<std::string>();
  endpoints.user = config["user"].as<std::string>();
  endpoints.password = config["password"].as<std::string>();
  return endpoints;
}

Configuration::Tls parseTlsConfiguration(const YAML::Node& config)
{
  Configuration::Tls tls{};
  tls.dir = config["dir"].as<std::string>();
  tls.certFile = config["cert"].as<std::string>();
  tls.keyFile = config["key"].as<std::string>();
  if (config["ca_cert"])
  {
    tls.caFile = config["ca_cert"].as<std::string>();
  }
  return tls;
}

std::optional<Configuration::Tls> readTlsFrom(const YAML::Node& node)
{
  auto config = node["tls"];
  if (config)
  {
    return parseTlsConfiguration(config);
  }
  else
  {
    return {};
  }
}

Configuration ConfigurationReader::tryRead() const
{
  Configuration config{};
  auto root = YAML::LoadFile(_configurationFile);
  config.endpoints = readEndpointsFrom(root);
  config.tls = readTlsFrom(root);
  config.apiToken = readApiToken();
  return config;
}

/**
 * Api token has a separate file to restrict permissions on shadow api calls.
 */
std::string ConfigurationReader::readApiToken() const
{
  std::string apiToken{};
  std::ifstream stream{_apiTokenFile};
  if (stream)
  {
    std::getline(stream, apiToken);
  }
  return apiToken;
}
} // namespace unix_account