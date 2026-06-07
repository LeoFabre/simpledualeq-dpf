#include "SimpleDualEqPlugin.hpp"

START_NAMESPACE_DISTRHO
using namespace sdeq;

SimpleDualEqPlugin::SimpleDualEqPlugin() : Plugin(kNumParameters, 0, 0) {
    Parameter tmp;
    for (uint32_t i = 0; i < kNumParameters; ++i) { initParameter(i, tmp); paramValues_[i] = tmp.ranges.def; }
}

void SimpleDualEqPlugin::initParameter(uint32_t index, Parameter& p) {
    const ParamRange r = rangeFor(index);
    p.hints = kParameterIsAutomatable;
    if (r.isBool) p.hints |= kParameterIsBoolean;
    if (r.isLog)  p.hints |= kParameterIsLogarithmic;
    p.ranges.min = r.min; p.ranges.max = r.max; p.ranges.def = r.def;
    switch (index) {
        case kEq1Freq: p.name="EQ1 Frequency"; p.symbol="eq1Freq"; p.unit="Hz"; break;
        case kEq1Gain: p.name="EQ1 Gain";      p.symbol="eq1Gain"; p.unit="dB"; break;
        case kEq1Q:    p.name="EQ1 Q";         p.symbol="eq1Q";    p.unit="";   break;
        case kEq1On:   p.name="EQ1 On";        p.symbol="eq1On";   p.unit="";   break;
        case kEq2Freq: p.name="EQ2 Frequency"; p.symbol="eq2Freq"; p.unit="Hz"; break;
        case kEq2Gain: p.name="EQ2 Gain";      p.symbol="eq2Gain"; p.unit="dB"; break;
        case kEq2Q:    p.name="EQ2 Q";         p.symbol="eq2Q";    p.unit="";   break;
        case kEq2On:   p.name="EQ2 On";        p.symbol="eq2On";   p.unit="";   break;
    }
}

float SimpleDualEqPlugin::getParameterValue(uint32_t index) const {
    return index < kNumParameters ? paramValues_[index] : 0.f;
}
void SimpleDualEqPlugin::setParameterValue(uint32_t index, float value) {
    if (index >= kNumParameters) return;
    paramValues_[index] = value;
    if (prepared_) pushParamsToDsp();
}

void SimpleDualEqPlugin::activate() {
    eq_.prepare(float(getSampleRate()));
    prepared_ = true;
    pushParamsToDsp();
}

void SimpleDualEqPlugin::pushParamsToDsp() {
    eq_.setBand(0, paramValues_[kEq1Freq], paramValues_[kEq1Gain],
                   paramValues_[kEq1Q], paramValues_[kEq1On] > 0.5f);
    eq_.setBand(1, paramValues_[kEq2Freq], paramValues_[kEq2Gain],
                   paramValues_[kEq2Q], paramValues_[kEq2On] > 0.5f);
}

void SimpleDualEqPlugin::run(const float** inputs, float** outputs, uint32_t frames) {
    for (int ch = 0; ch < 2; ++ch)
        eq_.process(inputs[ch], outputs[ch], (int)frames, ch);
}

Plugin* createPlugin() { return new SimpleDualEqPlugin(); }

END_NAMESPACE_DISTRHO
