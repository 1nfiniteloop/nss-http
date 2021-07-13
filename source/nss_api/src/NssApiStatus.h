#pragma once

#include <nss.h>
#include <string>

namespace nss_api
{
class NssApiStatus final
{
private:
  int& _errnop;
  nss_status _returnStatus;

public:
  NssApiStatus(int& errnop)
      : _errnop{errnop},
        _returnStatus{NSS_STATUS_SUCCESS}
  {
  }
  ~NssApiStatus() = default;

  NssApiStatus(const NssApiStatus& cls) = delete;            // Copy-constructor
  NssApiStatus& operator=(const NssApiStatus& cls) = delete; // Copy-assignment constructor
  NssApiStatus(NssApiStatus&& cls) = delete;                 // Move-constructor
  NssApiStatus& operator=(NssApiStatus&& cls) = delete;      // Move-assignment constructor

  void SetBufferTooSmall()
  {
    _returnStatus = NSS_STATUS_TRYAGAIN;
    _errnop = ERANGE;
  }

  void SetLookupFailed()
  {
    _returnStatus = NSS_STATUS_UNAVAIL; // is this correct? set error code also??
  }

  void SetNoResultFound()
  {
    _returnStatus = NSS_STATUS_NOTFOUND;
  }

  operator nss_status() const
  {
    return _returnStatus;
  }
};
} // namespace nss_api
