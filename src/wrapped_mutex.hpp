#pragma once

#include <mutex>
#include <utility>

namespace mik {

/**
 * @brief RAII style accessor for a WrappedMutex var
 */
template <class UnderlyingVarType, class MutexType>
class VarWithLock {
public:
  VarWithLock(UnderlyingVarType var, std::unique_lock<MutexType> lock)
      : var_(std::move(var)), lock_(std::move(lock)) {}

  UnderlyingVarType& get_ref() noexcept { return var_; }

  const UnderlyingVarType& get_ref() const noexcept { return var_; }

  UnderlyingVarType clone() const { return var_; }

private:
  UnderlyingVarType var_;
  std::unique_lock<MutexType> lock_;
};

/**
 * @brief Mutex wrapper around variable of type UnderlyingVarType
 */
template <class UnderlyingVarType, class MutexType = std::mutex>
class WrappedMutex {
public:
  explicit WrappedMutex(UnderlyingVarType var) : var_(std::move(var)) {}
  VarWithLock<UnderlyingVarType, MutexType> get() { return {var_, std::unique_lock{mutex_}}; }
  bool try_lock() { return mutex_.try_lock(); }

private:
  UnderlyingVarType var_;
  MutexType mutex_;
};

} // namespace mik