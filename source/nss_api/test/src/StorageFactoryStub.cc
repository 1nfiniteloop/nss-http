#include <memory>

#include "unix_account/Factory.h"
#include "unix_account/Group.h"
#include "unix_account/GroupReader.h"
#include "unix_account/Passwd.h"
#include "unix_account/PasswdReader.h"
#include "unix_account/Shadow.h"
#include "unix_account/ShadowReader.h"

namespace unix_account
{
template <>
std::unique_ptr<GroupReader> create()
{
  return {};
}

template <>
std::unique_ptr<PasswdReader> create()
{
  return {};
}

template <>
std::unique_ptr<ShadowReader> create()
{
  return {};
}
} // namespace unix_account