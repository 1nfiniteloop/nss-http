#include <boost/json.hpp>

#include "JsonValueExtract.h"
#include "unix_account/Group.h"

using namespace boost::json;
using unix_account::Group;

namespace boost::json
{
Group tag_invoke(const value_to_tag<Group>&, const value& jsonValue)
{
  Group group{};
  const object& obj = jsonValue.as_object();
  extract(obj, "gr_name", group.name);
  extract(obj, "gr_passwd", group.password);
  extract(obj, "gr_gid", group.gid);
  extract(obj, "gr_mem", group.members);
  return group;
}
} // namespace boost::json
