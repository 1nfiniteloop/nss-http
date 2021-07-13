#include "unix_account/Factory.h"
#include "unix_account/ShadowReader.h"

#include "Configuration.h"
#include "ConfigurationFileName.h"
#include "ConfigurationReader.h"
#include "ShadowReaderHttp.h"

namespace unix_account
{
template <>
std::unique_ptr<ShadowReader> create()
{
  ConfigurationReader configuration{
      CONFIGURATION_FILE_NAME,
      API_TOKEN_FILE_NAME};
  return std::make_unique<ShadowReaderHttp>(configuration.read());
}
} // namespace unix_account
