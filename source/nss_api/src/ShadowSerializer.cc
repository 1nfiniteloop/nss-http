#include <shadow.h>

#include "ShadowSerializer.h"

constexpr std::uint32_t DEFAULT_DAYS_MIN{};
constexpr std::uint32_t DEFAULT_DAYS_MAX{9999};
constexpr std::uint32_t DEFAULT_DAYS_WARN{7};
constexpr std::int8_t DEFAULT_DAYS_INACTIVE{-1};
constexpr std::int8_t DEFAULT_TIME_EXPIRES{-1};

using unix_account::Shadow;

namespace nss_api
{
ShadowSerializer::ShadowSerializer(
    spwd& shadow,
    char* buffer,
    std::size_t size)
    : _shadow{shadow},
      _writer{buffer, size}
{
  char* const last = buffer + size - 1;
  _shadow.sp_namp = last;
  _shadow.sp_pwdp = last;
  _shadow.sp_lstchg = 0;
  _shadow.sp_min = DEFAULT_DAYS_MIN;
  _shadow.sp_max = DEFAULT_DAYS_MAX;
  _shadow.sp_warn = DEFAULT_DAYS_WARN;
  _shadow.sp_inact = DEFAULT_DAYS_INACTIVE;
  _shadow.sp_expire = DEFAULT_TIME_EXPIRES;
}

void ShadowSerializer::append(const Shadow& shadow)
{
  _shadow.sp_namp = _writer.write(shadow.name);
  _shadow.sp_pwdp = _writer.write(shadow.password);
  _shadow.sp_lstchg = shadow.timeLastChange;
  // TODO: check if set, to not overwrite defaults?
  _shadow.sp_min = shadow.daysMin;
  _shadow.sp_max = shadow.daysMax;
  _shadow.sp_warn = shadow.daysWarn;
  if (shadow.daysInactive)
  {
    _shadow.sp_inact = shadow.daysInactive;
  }
  if (shadow.timeExpires)
  {
    _shadow.sp_expire = shadow.timeExpires;
  }
}
} // namespace nss_api