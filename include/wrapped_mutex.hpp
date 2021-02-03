#ifndef MIK90_WRAPPED_MUTEX_HPP
#define MIK90_WRAPPED_MUTEX_HPP

#include <mutex>
#include <utility>

namespace mik {

/**
 * @brief RAII style accessor for a WrappedMutex var
 */
template <class UnderlyingVarType, class MutexType>
class VarAccessor {
public:
  VarAccessor(UnderlyingVarType& var, std::unique_lock<MutexType> lock)
      : var_(var), lock_(std::move(lock)) {}

  UnderlyingVarType& get_ref() noexcept { return var_; }

  const UnderlyingVarType& get_ref() const noexcept { return var_; }

  UnderlyingVarType clone() const { return var_; }

private:
  UnderlyingVarType& var_;
  std::unique_lock<MutexType> lock_;
};

/**
 * @brief Mutex wrapper around variable of type UnderlyingVarType
 */
template <class UnderlyingVarType, class MutexType = std::mutex>
class WrappedMutex {
public:
  template <class... Args>
  explicit WrappedMutex(Args&&... args) : var_(std::forward<Args>(args)...) {}

  VarAccessor<UnderlyingVarType, MutexType> get() {
    return {var_, std::unique_lock<MutexType>{mutex_}};
  }

  bool try_lock() { return mutex_.try_lock(); }

private:
  UnderlyingVarType var_;
  MutexType mutex_;
};

} // namespace mik

#endif