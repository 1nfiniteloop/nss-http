#include "unix_account/Factory.h"
#include "unix_account/GroupReader.h"

#include "Configuration.h"
#include "ConfigurationFileName.h"
#include "ConfigurationReader.h"
#include "GroupReaderHttp.h"

namespace unix_account
{
template <>
std::unique_ptr<GroupReader> create()
{
  ConfigurationReader configuration{
      CONFIGURATION_FILE_NAME,
      API_TOKEN_FILE_NAME};
  return std::make_unique<GroupReaderHttp>(configuration.read());
}
} // namespace unix_account
