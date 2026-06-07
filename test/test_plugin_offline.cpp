#include "ParametricEq.hpp"
#include "DspMath.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>

int main() {
    using namespace sdeq;
    const float fs = 48000.f; const int N = 4096;
    ParametricEq eq; eq.prepare(fs);
    eq.setBand(0, 1000.f, 12.f, 2.f, true);
    eq.setBand(1, 5000.f, -6.f, 1.f, true);
    std::vector<float> in((size_t)N), out((size_t)N);
    for (int n=0;n<N;++n) in[n]=std::sin(2*kPi*440.f*n/fs);
    double e=0;
    for (int ch=0; ch<2; ++ch) {
        eq.process(in.data(), out.data(), N, ch);
        for (int n=0;n<N;++n){ if(!std::isfinite(out[n])){std::fprintf(stderr,"FAIL nan\n");return 1;} e+=double(out[n])*out[n]; }
    }
    if(!(e>1.0)){ std::fprintf(stderr,"FAIL silent %.4f\n", e); return 1; }
    std::puts("OK test_plugin_offline");
    return 0;
}
