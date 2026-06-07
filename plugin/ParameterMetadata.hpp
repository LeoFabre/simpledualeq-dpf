#pragma once
#include <cstdint>

namespace sdeq {

enum ParamId : uint32_t {
    kEq1Freq = 0, kEq1Gain, kEq1Q, kEq1On,
    kEq2Freq,     kEq2Gain, kEq2Q, kEq2On,
    kNumParameters
};

struct ParamRange { float min, max, def; bool isBool; bool isLog; };

inline ParamRange rangeFor(uint32_t id) {
    switch (id) {
        case kEq1Freq: return {20.f, 20000.f, 1000.f, false, true};
        case kEq1Gain: return {-24.f, 24.f, 0.f, false, false};
        case kEq1Q:    return {0.1f, 10.f, 1.f, false, false};
        case kEq1On:   return {0.f, 1.f, 1.f, true, false};
        case kEq2Freq: return {20.f, 20000.f, 5000.f, false, true};
        case kEq2Gain: return {-24.f, 24.f, 0.f, false, false};
        case kEq2Q:    return {0.1f, 10.f, 1.f, false, false};
        case kEq2On:   return {0.f, 1.f, 1.f, true, false};
        default:       return {0.f, 1.f, 0.f, false, false};
    }
}

} // namespace sdeq
