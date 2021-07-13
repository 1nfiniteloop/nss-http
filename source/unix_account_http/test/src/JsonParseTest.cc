#include <gtest/gtest.h>

#include "unix_account/Group.h"
#include "unix_account/Passwd.h"
#include "unix_account/Shadow.h"

#include "JsonValueTo.h"

using namespace unix_account;

TEST(JsonParseTest, PasswdValueIsNull)
{
  const std::string jsonText{"{\"pw_name\": \"foo\", \"pw_passwd\": \"x\", \"pw_uid\": 10000, \"pw_gid\": 10000, \"pw_gecos\": null, \"pw_dir\": \"/home/foo\", \"pw_shell\": \"/bin/bash\"}"};
  ASSERT_NO_THROW({ JsonValueTo<Passwd>(jsonText); });
}

TEST(JsonParseTest, ShadowValueIsNull)
{
  const std::string jsonText{"{\"sp_namp\": \"foo\", \"sp_pwdp\": \"*\", \"sp_lstchg\": 18820, \"sp_min\": 0, \"sp_max\": 99999, \"sp_warn\": 7, \"sp_inact\": null, \"sp_expire\": null}"};
  ASSERT_NO_THROW({ JsonValueTo<Shadow>(jsonText); });
}
