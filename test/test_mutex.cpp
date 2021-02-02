#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <mutex>
#include <string>
#include <thread>

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
  WrappedMutex<UserDefinedStruct> wrapped_udt{udt};
}

TEST(WrappedMutexTest, templateDeduction) {
  std::string a_string{"hello"};
  WrappedMutex wrappedS_Tring{a_string};
}

TEST(WrappedMutexTest, accessFromOtherThread) {
  WrappedMutex wrapped_int{15};
  auto owned_var = wrapped_int.get();
  std::atomic<uint32_t> try_lock_attempts{0};

  auto thread_without_lock = std::thread([&wrapped_int, &try_lock_attempts] {
    // Try to get the lock 5 times
    while (try_lock_attempts < 5) {
      ASSERT_FALSE(wrapped_int.try_lock())
          << "Mutex is already locked so it should not able to be unlocked";
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
      ++try_lock_attempts;
    }
  });

  thread_without_lock.join();
}