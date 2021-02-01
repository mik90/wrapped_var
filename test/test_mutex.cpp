#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <mutex>
#include <string>

#include "wrapped_mutex.hpp"

using namespace mik;

// Check that it even compiles
TEST(WrappedMutexTest, intConstructor) {
  WrappedMutex<int> wrappedInt{3};
  auto var_with_lock = wrappedInt.get();
  ASSERT_EQ(var_with_lock.get_ref(), 3);
}

TEST(WrappedMutexTest, recursiveMutex) {
  WrappedMutex<int, std::recursive_mutex> wrappedInt{3};
  auto var_with_lock = wrappedInt.get();

  ASSERT_EQ(var_with_lock.get_ref(), 3);
}

TEST(WrappedMutexTest, userDefinedType) {
  struct UserDefinedStruct {
    int value_0;
    float value_1;
    std::string value_3;
  };

  UserDefinedStruct udt{0, 0.0f, "hello"};
  WrappedMutex<UserDefinedStruct> wrappedUdt{udt};
}

TEST(WrappedMutexTest, templateDeduction) {
  std::string a_string{"hello"};
  WrappedMutex wrappedString{a_string};
}

/// @todo Make tests that attempt to access the mutex-protected underyling var