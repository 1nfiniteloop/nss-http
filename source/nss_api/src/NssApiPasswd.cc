#include <algorithm>
#include <forward_list>
#include <iterator>
#include <pwd.h>
#include <nss.h>

#include "unix_account/Exception.h"
#include "unix_account/Factory.h"
#include "unix_account/Passwd.h"
#include "unix_account/PasswdReader.h"

#include "PasswdSerializer.h"
#include "LogError.h"
#include "NssApiStatus.h"

#define NSS_API __attribute__((visibility("default")))

using namespace nss_api;
using namespace unix_account;


namespace
{
thread_local std::forward_list<Passwd> passwds{};  // stream for reading all
} // namespace

extern "C"
{
/**
 * Open stream
 */
NSS_API enum nss_status _nss_http_setpwent(int stayopen)
{
  (void)stayopen;
  nss_status response{NSS_STATUS_SUCCESS};
  try
  {
    auto reader = unix_account::create<PasswdReader>();
    auto all = reader->getAll();
    passwds = {
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
NSS_API enum nss_status _nss_http_getpwent_r(
    struct passwd* passwd,
    char* buffer,
    size_t buflen,
    int* errnop)
{
  NssApiStatus status{*errnop};
  PasswdSerializer out{*passwd, buffer, buflen};
  if (passwds.empty())
  {
    status.SetNoResultFound();
  }
  else
  {
    out << passwds.front();
    if (out.isTruncated())
    {
      status.SetBufferTooSmall();
    }
    else
    {
      passwds.pop_front();
    }
  }
  return status;
}

/**
 * Close stream
 */
NSS_API enum nss_status _nss_http_endpwent()
{
  passwds.clear();
  return NSS_STATUS_SUCCESS;
}

/**
 * Get passwd by name
 */
NSS_API enum nss_status _nss_http_getpwnam_r(
    const char* name,
    struct passwd* passwd,
    char* buffer,
    size_t buflen,
    int* errnop)
{
  NssApiStatus status{*errnop};
  PasswdSerializer out{*passwd, buffer, buflen};
  try
  {
    auto reader = unix_account::create<PasswdReader>();
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

/**
 * Get passwd by gid
 */
NSS_API enum nss_status _nss_http_getpwuid_r(
    gid_t gid,
    struct passwd* passwd,
    char* buffer,
    size_t buflen,
    int* errnop)
{
  NssApiStatus status{*errnop};
  PasswdSerializer out{*passwd, buffer, buflen};
  try
  {
    auto reader = unix_account::create<PasswdReader>();
    auto res = reader->GetById(gid);
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