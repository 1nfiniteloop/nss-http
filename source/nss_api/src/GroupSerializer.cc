#include <grp.h>

#include "GroupSerializer.h"
#include "unix_account/Group.h"

using unix_account::Group;

namespace nss_api
{
GroupSerializer::GroupSerializer(
    group& group,
    char* buffer,
    std::size_t size)
    : _group{group},
      _writer{buffer, size}
{
  char* const last = buffer + size - 1;
  char* const lastIndex = buffer + size - sizeof(char*);
  _group.gr_gid = 1000;
  _group.gr_name = last;
  _group.gr_passwd = last;
  _group.gr_mem = reinterpret_cast<char**>(lastIndex);
}

void GroupSerializer::append(const Group& group)
{
  _group.gr_gid = group.gid;
  _group.gr_name = _writer.write(group.name);
  _group.gr_passwd = _writer.write(group.password);
  _group.gr_mem = _writer.writeArray(group.members);
}
} // namespace nss_api