#include "DspMath.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
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
    EXPECT_NEAR(dbToGain(0.0f), 1.0f, 1e-6);
    EXPECT_NEAR(dbToGain(12.0f), 3.98107f, 1e-3);
    EXPECT_NEAR(dbToGain(-12.0f), 0.251189f, 1e-4);
    EXPECT_NEAR(clampf(5.f, 0.f, 1.f), 1.f, 1e-9);
    EXPECT_NEAR(clampf(-1.f, 0.f, 1.f), 0.f, 1e-9);
    std::puts("OK test_dsp_math");
    return 0;
}
