#pragma once

#include <iostream>
#include <istream>
#include <string>
#include <yaml-cpp/yaml.h>

#include "Configuration.h"

namespace unix_account
{
class ConfigurationReader final
{
private:
  const std::string _configurationFile{};
  const std::string _apiTokenFile{};

public:
  ConfigurationReader(
      const std::string& configurationFile,
      const std::string& apiTokenFile);

  ConfigurationReader(std::istream& stream);

  ~ConfigurationReader() = default;

  ConfigurationReader(const ConfigurationReader& cls) = delete;            // Copy-constructor
  ConfigurationReader& operator=(const ConfigurationReader& cls) = delete; // Copy-assignment constructor
  ConfigurationReader(ConfigurationReader&& cls) = delete;                 // Move-constructor
  ConfigurationReader& operator=(ConfigurationReader&& cls) = delete;      // Move-assignment constructor

  Configuration read() const;

private:
  Configuration tryRead() const;
  std::string readApiToken() const;
};
} // namespace unix_account