#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace unix_account
{
struct Configuration
{
  struct Endpoints
  {
    std::string url{"http://localhost:8025"};
    std::string group{"/api/group"};
    std::string user{"/api/user"};
    std::string password{"/api/password"};
  };

  struct Tls
  {
    std::filesystem::path dir{};
    std::filesystem::path certFile{};
    std::filesystem::path keyFile{};
    std::filesystem::path caFile{};
  };
  Endpoints endpoints{};
  std::optional<Tls> tls{};
  std::string apiToken{}; // required to access shadow api.
};
} // namespace unix_account