set(LIBRARY_NAME nss_http)
set(SOURCES
  src/NssHttp.cc
)
add_library(${LIBRARY_NAME} SHARED ${SOURCES})
set_target_properties(${LIBRARY_NAME} PROPERTIES
  VERSION 2
  CXX_VISIBILITY_PRESET hidden
)
target_link_libraries(${LIBRARY_NAME}
  PRIVATE
  nss_api
  unix_account_http
)
install(
  TARGETS ${LIBRARY_NAME}
  DESTINATION /lib/x86_64-linux-gnu
  COMPONENT ${LIBRARY_NAME}
)
