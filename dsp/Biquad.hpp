#pragma once
#include <cmath>
#include <algorithm>
#include "DspMath.hpp"

namespace sdeq {

class Biquad {
public:
    Biquad() = default;

    // Exact replica of juce::dsp::IIR::Coefficients<float>::makePeakFilter, a0-normalized.
    void setPeak(float fs, float freq, float Q, float gainLinear) {
        const float A     = std::max(0.0f, std::sqrt(gainLinear));
        const float omega = (2.0f * kPi * std::max(freq, 2.0f)) / fs;
        const float alpha = std::sin(omega) / (Q * 2.0f);
        const float c2    = -2.0f * std::cos(omega);
        const float aTA   = alpha * A;
        const float aOA   = alpha / A;
        const float b0 = 1.0f + aTA, b1 = c2, b2 = 1.0f - aTA;
        const float a0 = 1.0f + aOA, a1 = c2, a2 = 1.0f - aOA;
        b0n = b0 / a0; b1n = b1 / a0; b2n = b2 / a0;
        a1n = a1 / a0; a2n = a2 / a0;
    }

    void reset() { s1 = 0.0f; s2 = 0.0f; }

    inline float processSample(float x) {
        const float y = b0n * x + s1;
        s1 = b1n * x - a1n * y + s2;
        s2 = b2n * x - a2n * y;
        return y;
    }

private:
    float b0n = 1.0f, b1n = 0.0f, b2n = 0.0f, a1n = 0.0f, a2n = 0.0f;
    float s1 = 0.0f, s2 = 0.0f;
};

} // namespace sdeq
