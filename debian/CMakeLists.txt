install(
  FILES config/configuration.yaml
  DESTINATION /etc/nss-http
)

install(
  FILES config/shadow.auth-token.base64
  DESTINATION /etc/nss-http
  PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ
)
