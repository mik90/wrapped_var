#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "wrapped_mutex.hpp"

using namespace mik;

TEST(WrappedMutexTest, intConstructor) {
  // Check that it event compiles
  ASSERT_NO_THROW(WrappedMutex<int> wrappedInt{3});
}