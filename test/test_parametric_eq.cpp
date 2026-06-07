#include "ParametricEq.hpp"
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

int main() {
    using namespace sdeq;
    const float fs = 48000.f; const int N = 4096;
    // Both bands OFF -> exact passthrough on both channels.
    {
        ParametricEq eq; eq.prepare(fs);
        eq.setBand(0, 1000.f, 12.f, 2.f, false);
        eq.setBand(1, 5000.f, -6.f, 1.f, false);
        for (int ch=0; ch<2; ++ch) {
            float maxerr=0;
            for (int n=0;n<N;++n){ float x=std::sin(2*kPi*440.f*n/fs); float y=eq.processSample(x, ch);
                maxerr=std::max(maxerr, std::fabs(y-x)); }
            EXPECT_NEAR(maxerr, 0.f, 1e-6);
        }
    }
    // Band on with gain 0 -> passthrough (identity coeffs).
    {
        ParametricEq eq; eq.prepare(fs);
        eq.setBand(0, 1000.f, 0.f, 1.f, true);
        eq.setBand(1, 5000.f, 0.f, 1.f, false);
        float maxerr=0;
        for (int n=0;n<N;++n){ float x=std::sin(2*kPi*440.f*n/fs); float y=eq.processSample(x, 0);
            if(n>10) maxerr=std::max(maxerr, std::fabs(y-x)); }
        EXPECT_NEAR(maxerr, 0.f, 1e-4);
    }
    // Two bands boosting -> finite, non-silent.
    {
        ParametricEq eq; eq.prepare(fs);
        eq.setBand(0, 1000.f, 12.f, 2.f, true);
        eq.setBand(1, 5000.f, 9.f, 1.f, true);
        double e=0;
        for (int n=0;n<N;++n){ float x=std::sin(2*kPi*1000.f*n/fs); float y=eq.processSample(x,0);
            if(!std::isfinite(y)){ std::fprintf(stderr,"FAIL nan\n"); return 1; } e+=double(y)*y; }
        if(!(e>0.0)){ std::fprintf(stderr,"FAIL silent\n"); return 1; }
    }
    // Per-channel independence: exciting ch0 must not affect ch1.
    {
        ParametricEq eq; eq.prepare(fs);
        eq.setBand(0, 1000.f, 12.f, 5.f, true);
        eq.setBand(1, 5000.f, 0.f, 1.f, false);
        for (int n=0;n<100;++n) (void)eq.processSample(1.0f, 0); // excite ch0 only
        float y1a = eq.processSample(0.0f, 1); // ch1 still at rest -> 0
        EXPECT_NEAR(y1a, 0.0f, 1e-6);
    }
    std::puts("OK test_parametric_eq");
    return 0;
}
