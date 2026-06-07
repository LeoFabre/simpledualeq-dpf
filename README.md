# simpledualeq-dpf

A DPF port of [SimpleDualParametricEq](https://github.com/LeoFabre/juce-SimpleDualParametricEq) —
two RBJ peak/bell EQ filters in series, stereo — built to run headless in Sushi on Bela. VST3 + LV2.

## Layout

```
simpledualeq-dpf/
├── dpf/      DISTRHO/DPF submodule (+ recursive pugl)
├── dsp/      pure C++17 inline headers: DspMath, Biquad, ParametricEq
├── plugin/   DPF facade + ParameterMetadata + optional NanoVG UI
└── test/     unit tests (CTest) + null-test harness
```

The `Biquad` replicates `juce::dsp::IIR::Coefficients<float>::makePeakFilter` exactly (RBJ bell),
normalized by a0, Transposed-Direct-Form-II, in float. No parameter smoothing (matches the JUCE
original), so the null-test against the JUCE build is exact.

## Parameters (8)

EQ1 & EQ2, each: frequency (20–20000 Hz, logarithmic), gain (−24..+24 dB), Q (0.1..10), on (bool).
Defaults verbatim from JUCE: EQ1 1000 Hz, EQ2 5000 Hz, gain 0, Q 1, both on.

## Build

```bash
# Host, headless (tests + offline VST3):
cmake -S . -B build -DSIMPLEDUALEQ_BUILD_UI=OFF
cmake --build build -j
ctest --test-dir build --output-on-failure          # 4 unit tests

# Host, with the optional NanoVG debug UI:
cmake -S . -B build-ui -DSIMPLEDUALEQ_BUILD_UI=ON
cmake --build build-ui -j

# Cross-compile for Bela (aarch64), from the parent nexus-preamp/:
./cross-build-dpf-plugin.sh plugins/simpledualeq-dpf          # headless
./cross-build-dpf-plugin.sh plugins/simpledualeq-dpf --ui     # with UI
```

Headless VST3 ≈ 224 KB. Cross-build needs Docker + the `elk-crossbuild-bookworm` image; the wrapper
auto-detects the `SIMPLEDUALEQ_BUILD_UI` CMake option.

## Deploy to Bela

```bash
./deploy-plugin.sh SimpleDualEq --config bela-project/sushi-config-simpledualeq.json
```

## Status

| Gate | State |
|------|-------|
| DSP unit tests (host) | ✅ 4/4 pass |
| Headless VST3 < 1 MB | ✅ ~224 KB |
| UI build (host) | ✅ links with `SIMPLEDUALEQ_BUILD_UI=ON` |
| Null-test ≤ −80 dB vs JUCE | ⏳ harness ready; needs an offline VST3 host + the JUCE build |
| ARM64 cross-build | ⏳ needs Docker + elk image |
| Bela load + RSS delta | ⏳ needs a reachable Bela board |

See [`test/README.md`](test/README.md) for the null-test procedure.

## License

GPL-3.0-or-later.
