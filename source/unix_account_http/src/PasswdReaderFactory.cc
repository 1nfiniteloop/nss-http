

#include "unix_account/Factory.h"
#include "unix_account/PasswdReader.h"

#include "Configuration.h"
#include "ConfigurationFileName.h"
#include "ConfigurationReader.h"
#include "PasswdReaderHttp.h"

namespace unix_account
{
template <>
std::unique_ptr<PasswdReader> create()
{
  ConfigurationReader configuration{
      CONFIGURATION_FILE_NAME,
      API_TOKEN_FILE_NAME};
  return std::make_unique<PasswdReaderHttp>(configuration.read());
}
} // namespace unix_account
