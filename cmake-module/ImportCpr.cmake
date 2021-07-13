find_package(CURL REQUIRED
  PATH_SUFFIXES
  /usr/local/lib/cmake/CURL
  /usr/local/lib64/cmake/CURL
)

# target library names can be found in file:
# ${THIRD_PARTY_ROOT}/install/${DIST}/cpr-*/usr/local/lib/cmake/CURL/CURLTargets.cmake


find_library(CPR_LIBRARY NAMES cpr
  PATH_SUFFIXES /usr/local/lib
)
find_path(CPR_INCLUDE_DIR
    NAMES cpr/cpr.h
    PATH_SUFFIXES /usr/local/include
)
if(CPR_LIBRARY AND CPR_INCLUDE_DIR)
  set(CPR_FOUND "YES")
endif()

if(CPR_FOUND)
  add_library(Cpr::cpr STATIC IMPORTED)
  set_target_properties(Cpr::cpr PROPERTIES
    IMPORTED_LOCATION ${CPR_LIBRARY}
    INTERFACE_INCLUDE_DIRECTORIES ${CPR_INCLUDE_DIR}
    IMPORTED_LINK_INTERFACE_LIBRARIES CURL::libcurl
  )
else()
  message(FATAL_ERROR "Could not find cpr library")
endif()
