#include <algorithm>
#include <forward_list>
#include <iterator>
#include <nss.h>
#include <shadow.h>

#include "unix_account/Exception.h"
#include "unix_account/Factory.h"
#include "unix_account/Shadow.h"
#include "unix_account/ShadowReader.h"

#include "ShadowSerializer.h"
#include "LogError.h"
#include "NssApiStatus.h"

#define NSS_API __attribute__((visibility("default")))

using namespace nss_api;
using namespace unix_account;

namespace
{
thread_local std::forward_list<Shadow> shadows{};  // stream for reading all
} // namespace

extern "C"
{
/**
 * Open stream
 */
NSS_API enum nss_status _nss_http_setspent(int stayopen)
{
  (void)stayopen;
  nss_status response{NSS_STATUS_SUCCESS};
  try
  {
    auto reader = unix_account::create<ShadowReader>();
    auto all = reader->getAll();
    shadows = {
        std::make_move_iterator(all.begin()),
        std::make_move_iterator(all.end())};
  }
  catch (const UnixAccountLookupFailed& err)
  {
    LogError(err.what());
    response = NSS_STATUS_UNAVAIL;
  }
  return response;
}

/**
 * Get next entry in stream
 */
NSS_API enum nss_status _nss_http_getspent_r(
    struct spwd* shadow,
    char* buffer,
    size_t buflen,
    int* errnop)
{
  NssApiStatus status{*errnop};
  ShadowSerializer out{*shadow, buffer, buflen};
  if (shadows.empty())
  {
    status.SetNoResultFound();
  }
  else
  {
    out << shadows.front();
    if (out.isTruncated())
    {
      status.SetBufferTooSmall();
    }
    else
    {
      shadows.pop_front();
    }
  }
  return status;
}

/**
 * Close stream
 */
NSS_API enum nss_status _nss_http_endspent()
{
  shadows.clear();
  return NSS_STATUS_SUCCESS;
}

/**
 * Get shadow by name
 */
NSS_API enum nss_status _nss_http_getspnam_r(
    const char* name,
    struct spwd* shadow,
    char* buffer,
    size_t buflen,
    int* errnop)
{
  NssApiStatus status{*errnop};
  ShadowSerializer out{*shadow, buffer, buflen};
  try
  {
    auto reader = unix_account::create<ShadowReader>();
    auto res = reader->GetByName(name);
    if (res)
    {
      out << *res;
      if (out.isTruncated())
      {
          status.SetBufferTooSmall();
      }
    }
    else
    {
      status.SetNoResultFound();
    }
  }
  catch (UnixAccountLookupFailed& err)
  {
    LogError(err.what());
    status.SetLookupFailed();
  }
  return status;
}
}