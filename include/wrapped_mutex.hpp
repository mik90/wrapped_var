#ifndef MIK90_WRAPPED_MUTEX_HPP
#define MIK90_WRAPPED_MUTEX_HPP

#include <mutex>
#include <utility>

namespace mik {

/**
 * @brief RAII style accessor for a wrapped_var var
 * @tparam UnderlyingVarType the variable type protected by the mutex
 * @tparam MutexType the mutex that protects UnderlyingType
 * @tparam LockType the type used for locking MutexType
 */
template <class UnderlyingVarType, class MutexType, class LockType>
class var_accessor {
public:
  var_accessor(UnderlyingVarType& var, LockType lock)
      : var_(var), lock_(std::move(lock)) {}

  UnderlyingVarType& get_ref() noexcept { return var_; }

  const UnderlyingVarType& get_cref() const noexcept { return var_; }

  UnderlyingVarType clone() const { return var_; }

private:
  UnderlyingVarType& var_;
  LockType lock_;
};

/**
 * @brief Mutex wrapper around variable of type UnderlyingVarType
 * @tparam UnderlyingVarType the variable type protected by the mutex
 * @tparam MutexType the mutex that protects UnderlyingType
 * @tparam LockType the type used for locking MutexType
 */
template <class UnderlyingVarType, class MutexType = std::mutex, class LockType = std::unique_lock<MutexType>>
class wrapped_var {
public:
  template <class... Args>
  explicit wrapped_var(Args&&... args) : var_(std::forward<Args>(args)...) {}

  var_accessor<UnderlyingVarType, MutexType, LockType> get() {
    return {var_, LockType{mutex_}};
  }

  bool try_lock() { return mutex_.try_lock(); }

private:
  UnderlyingVarType var_;
  MutexType mutex_;
};

} // namespace mik

#endif