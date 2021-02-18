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
TEST(wrapped_var_test, int_constructor) {
  wrapped_var<int> wrapped_int{3};
  auto var_with_lock = wrapped_int.get();
  ASSERT_EQ(var_with_lock.get_ref(), 3);
}

TEST(wrapped_var_test, const_ref) {
  wrapped_var<int> wrapped_int{3};
  const auto var_with_lock = wrapped_int.get();
  const auto cref = var_with_lock.get_cref();
  ASSERT_EQ(cref, 3);
}


TEST(wrapped_var_test, unique_lock) {
  /// @todo Is there any way to avoid writing "std::mutex" twice?
  wrapped_var<int, std::mutex, std::unique_lock<std::mutex>> wrapped_int{3};
  const auto var_with_lock = wrapped_int.get();
  ASSERT_EQ(var_with_lock.get_cref(), 3);
}

/**
TEST(wrapped_var_test, lock_guard) {
  /// @todo Can i get lock_guard to work?
  wrapped_var<int, std::mutex, std::lock_guard<std::mutex>> wrapped_int{3};
  const auto var_with_lock = wrapped_int.get();
  ASSERT_EQ(var_with_lock.get_cref(), 3);
}

// C++17 and newer
#if __cplusplus >= 201703L
TEST(wrapped_var_test, scoped_lock) {
  /// @todo Can i get scoped_lock to work?
  wrapped_var<int, std::mutex, std::scoped_lock<std::mutex>> wrapped_int{3};
  const auto var_with_lock = wrapped_int.get();
  ASSERT_EQ(var_with_lock.get_cref(), 3);
}
#endif
*/

TEST(wrapped_var_test, recursive_mutex) {
  wrapped_var<int, std::recursive_mutex> wrapped_int{3};
  const auto var_with_lock = wrapped_int.get();
  ASSERT_EQ(var_with_lock.get_cref(), 3);
}

TEST(wrapped_var_test, timed_mutex) {
  wrapped_var<int, std::timed_mutex> wrapped_int{3};
  const auto var_with_lock = wrapped_int.get();
  ASSERT_EQ(var_with_lock.get_cref(), 3);
}

TEST(wrapped_var_test, recursive_timed_mutex) {
  wrapped_var<int, std::recursive_timed_mutex> wrapped_int{3};
  const auto var_with_lock = wrapped_int.get();
  ASSERT_EQ(var_with_lock.get_cref(), 3);
}

TEST(wrapped_var_test, user_defined_type) {
  struct user_defined_struct {
    int value_0;
    float value_1;
    std::string value_3;
  };

  user_defined_struct udt{0, 0.0f, "hello"};
  wrapped_var<user_defined_struct> wrapped_udt{udt};
}

TEST(wrapped_var_test, explicit_type_deduction) {
  // Compiles
  std::string a_string{"hello"};
  wrapped_var<std::string> wrapped_string{a_string};
  /* Doesn't/Shouldn't compile:
   * std::string a_string{"hello"};
   * wrapped_var wrapped_string{a_string};
   */
}

TEST(wrapped_var_test, modify_var) {
  wrapped_var<std::string> wrapped_string{"modify me"};
  {
    auto string_accessor = wrapped_string.get();
    string_accessor.get_ref() = "you are modified";
  }

  const auto const_string_accessor = wrapped_string.get();
  ASSERT_EQ(const_string_accessor.get_cref(), "you are modified");
}

TEST(wrapped_var_test, forward_args) {
  // Similar to vector::emplace_back
  using vector_type = std::vector<int>;
  constexpr vector_type::size_type size = 5;
  constexpr vector_type::value_type value = 12;
  wrapped_var<vector_type> wrapped_string(size, value);
}

TEST(wrapped_var_test, access_from_other_thread) {
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