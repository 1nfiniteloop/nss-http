#include <boost/json.hpp>

#include "JsonValueExtract.h"
#include "unix_account/Shadow.h"

using namespace boost::json;
using unix_account::Shadow;

namespace boost::json
{
Shadow tag_invoke(const value_to_tag<Shadow>&, const value& jsonValue)
{
  Shadow shadow{};
  const object& obj = jsonValue.as_object();
  extract(obj, "sp_namp", shadow.name);
  extract(obj, "sp_pwdp", shadow.password);
  extract(obj, "sp_lstchg", shadow.timeLastChange);
  extract(obj, "sp_min", shadow.daysMin);
  extract(obj, "sp_max", shadow.daysMax);
  extract(obj, "sp_warn", shadow.daysWarn);
  extract(obj, "sp_inact", shadow.daysInactive);
  extract(obj, "sp_expire", shadow.timeExpires);
  return shadow;
}
} // namespace boost::json
