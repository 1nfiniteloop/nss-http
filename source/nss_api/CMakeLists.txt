set(LIBRARY_NAME nss_api)
set(SOURCES
  src/BufferWriter.cc
  src/GroupSerializer.cc
  src/LogError.cc
  src/PasswdSerializer.cc
  src/ShadowSerializer.cc
  src/NssApiGroup.cc
  src/NssApiPasswd.cc
  src/NssApiShadow.cc
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
  PUBLIC unix_account
)

if (UNITTEST)
  add_subdirectory(test)
endif()