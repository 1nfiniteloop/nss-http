#include <algorithm>
#include <forward_list>
#include <grp.h>
#include <iterator>
#include <nss.h>
#include <string>

#include "unix_account/Exception.h"
#include "unix_account/Factory.h"
#include "unix_account/Group.h"
#include "unix_account/GroupReader.h"

#include "GroupSerializer.h"
#include "LogError.h"
#include "NssApiStatus.h"

#define NSS_API __attribute__((visibility("default")))

using namespace nss_api;
using namespace unix_account;

namespace
{
thread_local std::forward_list<Group> groups{}; // stream for reading all
} // namespace


extern "C"
{
/**
 * Open stream
 */
NSS_API enum nss_status _nss_http_setgrent(int stayopen)
{
  (void)stayopen;
  nss_status response{NSS_STATUS_SUCCESS};
  try
  {
    auto reader = unix_account::create<GroupReader>();
    auto all = reader->getAll();
    groups = {
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
NSS_API enum nss_status _nss_http_getgrent_r(
    struct group* grp,
    char* buffer,
    size_t buflen,
    int* errnop)
{
  NssApiStatus status{*errnop};
  GroupSerializer out{*grp, buffer, buflen};
  if (groups.empty())
  {
    status.SetNoResultFound();
  }
  else
  {
    out << groups.front();
    if (out.isTruncated())
    {
      status.SetBufferTooSmall();
    }
    else
    {
      groups.pop_front();
    }
  }
  return status;
}

/**
 * Close stream
 */
NSS_API enum nss_status _nss_http_endgrent()
{
  groups.clear();
  return NSS_STATUS_SUCCESS;
}

/**
 * Get group by name
 */
NSS_API enum nss_status _nss_http_getgrnam_r(
    const char* name,
    struct group* grp,
    char* buffer,
    size_t buflen,
    int* errnop)
{
  NssApiStatus status{*errnop};
  GroupSerializer out{*grp, buffer, buflen};
  try
  {
    auto reader = unix_account::create<GroupReader>();
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
 * Get group by gid
 */
NSS_API enum nss_status _nss_http_getgrgid_r(
    gid_t gid,
    struct group* grp,
    char* buffer,
    size_t buflen,
    int* errnop)
{
  NssApiStatus status{*errnop};
  GroupSerializer out{*grp, buffer, buflen};
  try
  {
    auto reader = unix_account::create<GroupReader>();
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