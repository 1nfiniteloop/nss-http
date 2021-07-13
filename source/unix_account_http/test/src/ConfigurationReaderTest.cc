#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include <string_view>
#include <yaml-cpp/yaml.h>

#include "unix_account/Exception.h"

#include "Configuration.h"
#include "ConfigurationReader.h"

using namespace unix_account;

std::ostream& operator<<(std::ostream& out, const Configuration::Endpoints& endpoints)
{
  out << "endpoints:" << std::endl
      << "  url: " << endpoints.url << std::endl
      << "  group: " << endpoints.group << std::endl
      << "  user: " << endpoints.user << std::endl
      << "  password: " << endpoints.password << std::endl
      << std::endl;
  return out;
}

std::ostream& operator<<(std::ostream& out, const Configuration::Tls& tls)
{
  out << "tls:" << std::endl
      << "  dir: " << tls.dir << std::endl
      << "  cert: " << tls.certFile << std::endl
      << "  key: " << tls.keyFile << std::endl
      << "  ca_cert: " << tls.caFile << std::endl
      << std::endl;
  return out;
}

std::ostream& operator<<(std::ostream& out, const Configuration& config)
{
  out << config.endpoints;
  if (config.tls)
  {
    out << *config.tls;
  }
  return out;
}

class ConfigurationReaderTest : public ::testing::Test
{
public:
  struct Defaults
  {
    Configuration configuration{
        Configuration::Endpoints{"http://localhost:8025", "/api/group", "/api/user", "/api/password"},
        Configuration::Tls{"/etc/certs", "*.cert.pem", "*.key.pem", "*.ca-chain.pem"}};
    std::string configFileName{"nss-api-config.yaml"};
    std::string apiTokenFileName{"nss-api-token.base64"};
    std::string apiToken{"secret token"};
  };
  static const Defaults defaults;

  /**
   * Helper for writing to default configuration file
   */
  class ConfigurationFile : public std::ofstream
  {
  public:
    ConfigurationFile()
        : std::ofstream{defaults.configFileName, std::ios::out | std::ios::trunc}
    {
    }
    ConfigurationFile(const Configuration& config)
        : ConfigurationFile{}
    {
      *this << config;
    }

    ConfigurationFile(const Configuration::Endpoints& endpoints)
        : ConfigurationFile{}
    {
      *this << endpoints;
    }

    ConfigurationFile(const Configuration::Tls& tls)
        : ConfigurationFile{}
    {
      *this << tls;
    }

    ~ConfigurationFile() = default;
  };

  /**
   * Helper for writing to default api token file
   */
  class ApiTokenFile : public std::ofstream
  {
  public:
    ApiTokenFile(const std::string& apiToken)
        : ApiTokenFile{}
    {
      *this << apiToken;
    }

    ApiTokenFile()
        : std::ofstream{defaults.apiTokenFileName, std::ios::out | std::ios::trunc}
    {
    }
    ~ApiTokenFile() = default;
  };

  static void SetUpTestSuite()
  {
    // create files
    ConfigurationFile{};
    ApiTokenFile{};
  }

  static void TearDownTestSuite()
  {
    std::filesystem::remove(defaults.configFileName);
    std::filesystem::remove(defaults.apiTokenFileName);
  }

protected:
  ConfigurationReader reader{defaults.configFileName, defaults.apiTokenFileName};
};

const ConfigurationReaderTest::Defaults ConfigurationReaderTest::defaults{};

TEST_F(ConfigurationReaderTest, ReadNonexistingTlsConfiguration)
{
  ConfigurationFile{defaults.configuration.endpoints};

  auto config = reader.read();
  ASSERT_FALSE(config.tls);
}

TEST_F(ConfigurationReaderTest, ReadTlsConfiguration)
{
  ConfigurationFile{defaults.configuration};

  auto config = reader.read();
  ASSERT_TRUE(config.tls);
  EXPECT_EQ(defaults.configuration.tls->dir, config.tls->dir);
  EXPECT_EQ(defaults.configuration.tls->certFile, config.tls->certFile);
  EXPECT_EQ(defaults.configuration.tls->keyFile, config.tls->keyFile);
  EXPECT_EQ(defaults.configuration.tls->caFile, config.tls->caFile);
}

TEST_F(ConfigurationReaderTest, ReadInconsistentTlsConfiguration)
{
  ConfigurationFile out{defaults.configuration.endpoints};
  out << "tls:" << std::endl
      // cert + key is missing
      << "  dir: " << defaults.configuration.tls->dir << std::endl
      << "  ca_cert: " << defaults.configuration.tls->caFile << std::endl
      << std::endl;
  ASSERT_THROW({ reader.read(); }, UnixAccountLookupFailed);
}

TEST_F(ConfigurationReaderTest, ReadConfigurationFromNonexistingFile)
{
  ConfigurationReader reader{"nonexisting-file.yaml", defaults.apiTokenFileName};
  ASSERT_THROW({ reader.read(); }, UnixAccountLookupFailed);
}

TEST_F(ConfigurationReaderTest, ReadApiTokenFromNonexistingFile)
{
  ConfigurationFile{defaults.configuration};
  ConfigurationReader reader{defaults.configFileName, "nonexisting-token.base64"};
  auto config = reader.read();
  ASSERT_TRUE(config.apiToken.empty());
}

TEST_F(ConfigurationReaderTest, ReadEndpointsConfiguration)
{
  ConfigurationFile{defaults.configuration};

  auto config = reader.read();
  EXPECT_EQ(defaults.configuration.endpoints.url, config.endpoints.url);
  EXPECT_EQ(defaults.configuration.endpoints.group, config.endpoints.group);
  EXPECT_EQ(defaults.configuration.endpoints.user, config.endpoints.user);
  EXPECT_EQ(defaults.configuration.endpoints.password, config.endpoints.password);
}

TEST_F(ConfigurationReaderTest, ReadInconsistentEndpointsConfiguration)
{
  ConfigurationFile out{};
  out << "endpoints:" << std::endl
      << "  url: http://localhost:8025" << std::endl
      // group, user, password is missing
      << std::endl;

  ASSERT_THROW({ reader.read(); }, UnixAccountLookupFailed);
}

TEST_F(ConfigurationReaderTest, ReadNonexistingEndpointsConfiguration)
{
  ConfigurationFile out{};
  out << "foo:" << std::endl
      << "  key: val" << std::endl
      << std::endl;

  ASSERT_THROW({ reader.read(); }, UnixAccountLookupFailed);
}

TEST_F(ConfigurationReaderTest, ReadApiToken)
{
  ConfigurationFile{defaults.configuration};
  ApiTokenFile{defaults.apiToken};

  auto config = reader.read();
  EXPECT_EQ(defaults.apiToken, config.apiToken);
}

TEST_F(ConfigurationReaderTest, ReadApiTokenWithoutPermission)
{
  ConfigurationFile{defaults.configuration};
  ApiTokenFile{defaults.apiToken};

  using std::filesystem::perms;
  std::filesystem::permissions(
      defaults.apiTokenFileName,
      perms::none | perms::owner_write);
  auto config = reader.read();
  EXPECT_TRUE(config.apiToken.empty());

  std::filesystem::permissions(defaults.apiTokenFileName, perms::all);
}