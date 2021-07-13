#include <pwd.h>

#include "PasswdSerializer.h"

using unix_account::Passwd;

namespace nss_api
{
PasswdSerializer::PasswdSerializer(
    passwd& passwd,
    char* buffer,
    std::size_t size)
    : _passwd{passwd},
      _writer{buffer, size}
{
  char* const last = buffer + size - 1;
  _passwd.pw_name = last;
  _passwd.pw_passwd = last;
  _passwd.pw_uid = 1000;
  _passwd.pw_gid = 1000;
  _passwd.pw_gecos = last;
  _passwd.pw_dir = last;
  _passwd.pw_shell = last;
}

void PasswdSerializer::append(const Passwd& passwd)
{
  _passwd.pw_name = _writer.write(passwd.name);
  _passwd.pw_passwd = _writer.write(passwd.password);
  _passwd.pw_uid = passwd.uid;
  _passwd.pw_gid = passwd.gid;
  _passwd.pw_gecos = _writer.write(passwd.gecos);
  _passwd.pw_dir = _writer.write(passwd.dir);
  _passwd.pw_shell = _writer.write(passwd.shell);
}
} // namespace nss_api