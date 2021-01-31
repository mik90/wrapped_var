#pragma once

#include <utility>

namespace mik {
template <class UnderlyingVarType>
class WrappedMutex {
public:
  explicit WrappedMutex(UnderlyingVarType var) : var_(std::move(var)) {}

private:
  UnderlyingVarType var_;
};
} // namespace mik