set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.75 REQUIRED
  COMPONENTS json
  PATH_SUFFIXES lib/cmake/Boost-1.75.0
)

# target library names can be found in file:
# ${THIRD_PARTY_ROOT}/install/${DIST}/boost_*/lib/cmake/Boost-*/BoostConfig.cmake
