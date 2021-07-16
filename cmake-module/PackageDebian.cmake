set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_CONTACT "Lars Gunnarsson <gunnarsson.lars@gmail.com>")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "${PROJECT_HOMEPAGE_URL}")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libssl1.1 | libssl-dev")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
  "${PROJECT_SOURCE_DIR}/debian/script/postinst"
  "${PROJECT_SOURCE_DIR}/debian/script/conffiles"
)

include(CPack)