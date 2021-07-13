#include <cpr/bearer.h>
#include <cpr/cpr.h>
#include <string>

#include "unix_account/Exception.h"

#include "Configuration.h"
#include "HttpRequest.h"

namespace
{
cpr::SslOptions BuildSslOptions(const unix_account::Configuration::Tls& tls)
{
  return cpr::Ssl(
      cpr::ssl::CaInfo{tls.dir / tls.caFile},
      cpr::ssl::CertFile{tls.dir / tls.certFile},
      cpr::ssl::KeyFile{tls.dir / tls.keyFile});
}
} // namespace

namespace unix_account
{
HttpRequest::HttpRequest(cpr::Url url)
    : _url{std::move(url)}
{
}

void HttpRequest::setTlsConfig(const Configuration::Tls& tls)
{
  _session.SetSslOptions(BuildSslOptions(tls));
}

void HttpRequest::setApiToken(const std::string& apiToken)
{
  _session.SetOption(cpr::Bearer{apiToken});
}

std::string HttpRequest::HttpGet(cpr::Parameters parameters)
{
  _session.SetUrl(_url);
  _session.SetParameters(std::move(parameters));
  auto resp = _session.Get();
  if (resp.status_code == 200 or resp.status_code == 404)
  {
    return resp.text;
  }
  else if (resp.error)
  {
    throw UnixAccountLookupFailed(resp.error.message);
  }
  else
  {
    throw UnixAccountLookupFailed("Unexpected http response code: " + std::to_string(resp.status_code));
  }
}
} // namespace unix_account