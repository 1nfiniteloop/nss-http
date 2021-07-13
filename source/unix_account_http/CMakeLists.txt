set(LIBRARY_NAME unix_account_http)
set(SOURCES
  src/ConfigurationReader.cc
  src/GroupReaderFactory.cc
  src/HttpRequest.cc
  src/JsonValueToGroup.cc
  src/JsonValueToPasswd.cc
  src/JsonValueToShadow.cc
  src/PasswdReaderFactory.cc
  src/ShadowReaderFactory.cc
)
add_library(${LIBRARY_NAME} OBJECT ${SOURCES})
set_target_properties(${LIBRARY_NAME} PROPERTIES
  POSITION_INDEPENDENT_CODE ON
  CXX_VISIBILITY_PRESET hidden
)
target_include_directories(${LIBRARY_NAME}
  PUBLIC inc
  PRIVATE src
)
target_link_libraries(${LIBRARY_NAME}
PUBLIC
  unix_account
  Cpr::cpr
  yaml-cpp
PRIVATE
  Boost::json
)

if (UNITTEST)
  add_subdirectory(test)
endif()
