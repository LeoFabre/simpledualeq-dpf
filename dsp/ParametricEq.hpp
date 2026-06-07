#pragma once
#include "Biquad.hpp"
#include "DspMath.hpp"

namespace sdeq {

// Two peak bands in series, stereo. Coefficients are shared per band across channels;
// filter state is independent per channel. An OFF band passes through and is reset
// (mirrors the JUCE original's updateFilters()/reset() bypass behavior).
class ParametricEq {
public:
    static constexpr int kNumBands = 2;

    void prepare(float sampleRate) {
        fs_ = sampleRate;
        for (int b = 0; b < kNumBands; ++b) {
            on_[b] = true;
            for (int ch = 0; ch < 2; ++ch) biquad_[b][ch].reset();
        }
    }

    void setBand(int idx, float freqHz, float gainDb, float Q, bool on) {
        const bool wasOn = on_[idx];
        on_[idx] = on;
        if (on) {
            const float g = dbToGain(gainDb);
            for (int ch = 0; ch < 2; ++ch) biquad_[idx][ch].setPeak(fs_, freqHz, Q, g);
        } else if (wasOn) {
            for (int ch = 0; ch < 2; ++ch) biquad_[idx][ch].reset();
        }
    }

    inline float processSample(float x, int ch) {
        for (int b = 0; b < kNumBands; ++b)
            if (on_[b]) x = biquad_[b][ch].processSample(x);
        return x;
    }

    void process(const float* in, float* out, int n, int ch) {
        for (int i = 0; i < n; ++i) out[i] = processSample(in[i], ch);
    }

private:
    float fs_ = 48000.0f;
    bool  on_[kNumBands] { true, true };
    Biquad biquad_[kNumBands][2];
};

} // namespace sdeq
