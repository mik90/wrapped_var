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
TEST(wrapped_varTest, intConstructor) {
  wrapped_var<int> wrappedInt{3};
  auto var_with_lock = wrappedInt.get();
  ASSERT_EQ(var_with_lock.get_ref(), 3);
}

TEST(wrapped_varTest, constRef) {
  wrapped_var<int> wrappedInt{3};
  const auto var_with_lock = wrappedInt.get();
  const auto ref = var_with_lock.get_cref();
  ASSERT_EQ(ref, 3);
}

TEST(wrapped_varTest, recursiveMutex) {
  wrapped_var<int, std::recursive_mutex> wrappedInt{3};
  auto var_with_lock = wrappedInt.get();

  ASSERT_EQ(var_with_lock.get_ref(), 3);
}

TEST(wrapped_varTest, userDefinedType) {
  struct UserDefinedStruct {
    int value_0;
    float value_1;
    std::string value_3;
  };

  UserDefinedStruct udt{0, 0.0f, "hello"};
  wrapped_var<UserDefinedStruct> wrapped_udt{udt};
}

TEST(wrapped_varTest, explicitTypeDeduction) {
  // Compiles
  std::string a_string{"hello"};
  wrapped_var<std::string> wrapped_string{a_string};
  /* Doesn't/Shouldn't compile:
   * std::string a_string{"hello"};
   * wrapped_var wrapped_string{a_string};
   */
}

TEST(wrapped_varTest, modifyVariable) {
  wrapped_var<std::string> wrapped_string{"modify me"};
  {
    auto string_accessor = wrapped_string.get();
    string_accessor.get_ref() = "you are modified";
  }

  const auto const_string_accessor = wrapped_string.get();
  ASSERT_EQ(const_string_accessor.get_cref(), "you are modified");
}

TEST(wrapped_varTest, forwardArgs) {
  // Similar to vector::emplace_back
  using vector_type = std::vector<int>;
  constexpr vector_type::size_type size = 5;
  constexpr vector_type::value_type value = 12;
  wrapped_var<vector_type> wrapped_string(size, value);
}

TEST(wrapped_varTest, accessFromOtherThread) {
  wrapped_var<int> wrapped_int{15};
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