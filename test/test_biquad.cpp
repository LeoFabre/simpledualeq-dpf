#include "Biquad.hpp"
#include "DspMath.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>
#define EXPECT_NEAR(a, b, eps) do {                                          \
    const double _a = double(a), _b = double(b), _e = double(eps);           \
    if (std::abs(_a - _b) > _e) {                                            \
        std::fprintf(stderr, "FAIL %s:%d: %.6f != %.6f (eps=%.6g)\n",        \
            __FILE__, __LINE__, _a, _b, _e);                                 \
        std::exit(1);                                                        \
    }                                                                        \
} while (0)

// Steady-state magnitude at freqHz by driving a unit sine and measuring RMS ratio.
static float magAt(sdeq::Biquad& bq, float fs, float freqHz) {
    const int N = 200000; double sin2=0, out2=0;
    for (int n=0;n<N;++n){ float x=std::sin(2*sdeq::kPi*freqHz*n/fs); float y=bq.processSample(x);
        if(n> N/2){ sin2+=double(x)*x; out2+=double(y)*y; } }
    return (float)std::sqrt(out2/sin2);
}
int main() {
    using namespace sdeq;
    const float fs = 48000.f;
    // gain 0 (gainLinear 1) -> identity: output == input within float epsilon.
    {
        Biquad bq; bq.setPeak(fs, 1000.f, 1.f, dbToGain(0.f));
        float maxerr=0;
        for (int n=0;n<2000;++n){ float x=std::sin(2*kPi*1000.f*n/fs); float y=bq.processSample(x);
            if(n>10) maxerr=std::max(maxerr, std::fabs(y-x)); }
        EXPECT_NEAR(maxerr, 0.f, 1e-4);
    }
    // +12 dB peak at fc -> |H(fc)| ~= dbToGain(12).
    { Biquad bq; bq.setPeak(fs, 1000.f, 2.f, dbToGain(12.f));
      EXPECT_NEAR(magAt(bq, fs, 1000.f), dbToGain(12.f), 0.05f); }
    // -12 dB cut at fc -> |H(fc)| ~= dbToGain(-12).
    { Biquad bq; bq.setPeak(fs, 1000.f, 2.f, dbToGain(-12.f));
      EXPECT_NEAR(magAt(bq, fs, 1000.f), dbToGain(-12.f), 0.02f); }
    // Far from fc (octaves below) -> near unity for a Q2 bell.
    { Biquad bq; bq.setPeak(fs, 4000.f, 2.f, dbToGain(12.f));
      float m = magAt(bq, fs, 250.f);
      if (!(m > 0.9f && m < 1.15f)) { std::fprintf(stderr,"FAIL far-from-fc %.4f\n", m); return 1; } }
    std::puts("OK test_biquad");
    return 0;
}
