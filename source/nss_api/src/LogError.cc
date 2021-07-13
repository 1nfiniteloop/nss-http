#include <iostream>
#include <syslog.h>

#include "LogError.h"

namespace nss_api
{
void LogError(const std::string& message)
{
  openlog("nss-http", LOG_CONS  | LOG_NDELAY, LOG_LOCAL1);
  syslog(LOG_ERR, "nss-http error: %s", message.c_str());
  closelog();
}
} // namespace nss_api