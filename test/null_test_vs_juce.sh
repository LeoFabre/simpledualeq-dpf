#!/usr/bin/env bash
# Render the same input wav through SimpleDualParametricEq-JUCE and SimpleDualEq-DPF
# using an offline VST3 host, then compare the outputs sample-by-sample.
#
# Requirements:
#   - An offline VST3 host (carla-single by default; adapt to pluginval/etc.).
#   - SIMPLEDUALEQ_JUCE_VST3 pointing at the JUCE bundle, built from
#     /Users/lfabre/nexus-workdir/juce-SimpleDualParametricEq.
#
# Presets (spec §4):
#   default : both plugins at factory defaults (both bands on, gain 0 -> flat).
#   stress  : EQ1 200 Hz +12 dB Q2, EQ2 6 kHz -9 dB Q0.7. Applying it requires a host
#             that sets VST3 parameters before rendering; wire to your host.
#
# Acceptance: peak residual <= -80 dB (relaxable to -60 dB with a documented reason).
set -euo pipefail
HERE="$(cd "$(dirname "$0")" && pwd)"
PRESET="${1:-default}"
FIXTURE="${HERE}/fixtures/input.wav"
DPF_VST3="${HERE}/../build/bin/SimpleDualEq.vst3"
JUCE_VST3="${SIMPLEDUALEQ_JUCE_VST3:?Set SIMPLEDUALEQ_JUCE_VST3 to the JUCE VST3 bundle}"
THRESHOLD_DB="${THRESHOLD_DB:--80}"
OUT_JUCE="${HERE}/fixtures/out_juce_${PRESET}.wav"
OUT_DPF="${HERE}/fixtures/out_dpf_${PRESET}.wav"

[[ -f "${FIXTURE}" ]] || python3 "${HERE}/fixtures/generate_input.py" "${FIXTURE}"

if [[ "${PRESET}" != "default" ]]; then
    echo "WARNING: preset '${PRESET}' requires a host that sets VST3 params before render (spec §4)." >&2
fi

if command -v carla-single >/dev/null 2>&1; then
    carla-single vst3 "${JUCE_VST3}" "${FIXTURE}" "${OUT_JUCE}"
    carla-single vst3 "${DPF_VST3}"  "${FIXTURE}" "${OUT_DPF}"
else
    echo "ERROR: no carla-single. Install Carla or adapt to your offline VST3 host." >&2
    exit 2
fi

python3 "${HERE}/compare_wavs.py" "${OUT_JUCE}" "${OUT_DPF}" --threshold-db "${THRESHOLD_DB}"
