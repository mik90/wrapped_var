#pragma once

namespace mik {
    // TODO 
    // MutexWrap<T>
    template<class UnderlyingVarType>
    class MutexWrap {
        public:
        MutexWrap(UnderlyingVarType var) : var_(std::move(var)) {}

        private:
        UnderlyingVarType var_;
    };
}