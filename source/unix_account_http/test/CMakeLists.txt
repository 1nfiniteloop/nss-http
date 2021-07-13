set(UNITTEST_NAME "${LIBRARY_NAME}_test")
set(SOURCES
  src/ConfigurationReaderTest.cc
  src/JsonParseTest.cc
)
add_executable(${UNITTEST_NAME} ${SOURCES})
add_test(NAME ${UNITTEST_NAME} COMMAND ${UNITTEST_NAME})
add_dependencies(test_all ${UNITTEST_NAME})
target_include_directories(${UNITTEST_NAME}
  PRIVATE ../src
)
target_link_libraries(${UNITTEST_NAME} PRIVATE
  ${LIBRARY_NAME}
  GTest::gmock_main
  Boost::json
)
