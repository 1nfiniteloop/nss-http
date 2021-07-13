set(LIBRARY_NAME unix_account)
add_library(${LIBRARY_NAME} INTERFACE)
target_include_directories(${LIBRARY_NAME}
  INTERFACE inc
)
