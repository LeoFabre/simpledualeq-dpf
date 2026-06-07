# SimpleDualEq-DPF tests

## Unit tests (CTest)

Standalone (no DPF):

```bash
cmake -S . -B build-test          # from plugins/simpledualeq-dpf/test
cmake --build build-test -j
ctest --test-dir build-test --output-on-failure
```

Or via the full project:

```bash
cmake -S . -B build -DSIMPLEDUALEQ_BUILD_UI=OFF   # from plugins/simpledualeq-dpf
cmake --build build -j
ctest --test-dir build --output-on-failure
```

4 executables: `test_dsp_math`, `test_biquad`, `test_parametric_eq`, `test_plugin_offline`.

## Null-test against SimpleDualParametricEq-JUCE (offline)

```bash
# Build the JUCE original first (host VST3):
cmake -S /Users/lfabre/nexus-workdir/juce-SimpleDualParametricEq -B /Users/lfabre/nexus-workdir/juce-SimpleDualParametricEq/build-host -DCMAKE_BUILD_TYPE=Release
cmake --build /Users/lfabre/nexus-workdir/juce-SimpleDualParametricEq/build-host -j

export SIMPLEDUALEQ_JUCE_VST3=/path/to/built/SimpleDualParametricEq.vst3
./null_test_vs_juce.sh default     # both at factory defaults (flat)
./null_test_vs_juce.sh stress      # see note in the script
```

Default host: `carla-single`. Acceptance: peak residual ≤ −80 dB (relaxable to −60 dB with a
documented reason). The DSP is closed-form with no smoothing/RNG, so an exact null is expected.

The fixture is **not committed** — regenerated on demand by `fixtures/generate_input.py`
(pure-stdlib Python, no numpy).
