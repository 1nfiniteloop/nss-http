#include <boost/json.hpp>

#include "JsonValueExtract.h"
#include "unix_account/Passwd.h"

using namespace boost::json;
using unix_account::Passwd;

namespace boost::json
{
Passwd tag_invoke(const value_to_tag<Passwd>&, const value& jsonValue)
{
  Passwd passwd{};
  const object& obj = jsonValue.as_object();
  extract(obj, "pw_name", passwd.name);
  extract(obj, "pw_passwd", passwd.password);
  extract(obj, "pw_uid", passwd.uid);
  extract(obj, "pw_gid", passwd.gid);
  extract(obj, "pw_gecos", passwd.gecos);
  extract(obj, "pw_dir", passwd.dir);
  extract(obj, "pw_shell", passwd.shell);
  return passwd;
}
} // namespace boost::json
