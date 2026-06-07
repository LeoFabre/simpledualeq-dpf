#pragma once
#include <cmath>

namespace sdeq {
constexpr float kPi = 3.14159265358979323846f;
inline float dbToGain(float db)  { return std::pow(10.0f, 0.05f * db); }
inline float clampf(float x, float lo, float hi) { return x < lo ? lo : (x > hi ? hi : x); }
} // namespace sdeq
