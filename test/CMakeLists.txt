include(GoogleTest)

add_executable(UnitTest 
  main.cpp
  test_mutex.cpp
)

target_link_libraries(UnitTest PRIVATE
  project_warnings
  project_options
  CONAN_PKG::gtest
  WrappedVarLib
)

gtest_discover_tests(UnitTest)