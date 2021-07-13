cmake_minimum_required(VERSION 3.5)
project(nss-http
  VERSION ${VERSION}
  DESCRIPTION "Name service switch implementation for query unix accounts over http"
  HOMEPAGE_URL "https://github.com/1nfiniteloop/nss-http"
)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake-module)
set(CMAKE_BUILD_TYPE ${BUILD})
option(UNITTEST "Build unit tests and its dependencies" ON)

# Language-specific settings
add_compile_options(-Wall -Wextra -pedantic -Werror)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# THIRD_PARTY_ROOT and DIST provided from Dockerfile
set(CMAKE_PREFIX_PATH
  $ENV{THIRD_PARTY_ROOT}/install/$ENV{DIST}/boost_1_75_0
  $ENV{THIRD_PARTY_ROOT}/install/$ENV{DIST}/cpr-1.6.2
  $ENV{THIRD_PARTY_ROOT}/install/$ENV{DIST}/yaml-cpp-0.6.3
  $ENV{THIRD_PARTY_ROOT}/install/$ENV{DIST}/googletest-release-1.10.0
)

if (UNITTEST)
  include(ImportGTest)
  enable_testing()
  add_custom_target(test_all COMMAND ${CMAKE_CTEST_COMMAND})
endif()

# Third party dependencies:
include(ImportBoost)
include(ImportCpr)
include(ImportYamlCpp)

# Project components
add_subdirectory(debian)
add_subdirectory(source)

# Package
include(PackageDebian)