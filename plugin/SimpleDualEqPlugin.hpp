#pragma once
#include "DistrhoPlugin.hpp"
#include "ParameterMetadata.hpp"
#include "ParametricEq.hpp"

START_NAMESPACE_DISTRHO

class SimpleDualEqPlugin : public Plugin {
public:
    SimpleDualEqPlugin();

    const char* getLabel()       const override { return "SimpleDualEq"; }
    const char* getDescription() const override { return "Two-band parametric (peak) EQ"; }
    const char* getMaker()       const override { return "Nexus"; }
    const char* getHomePage()    const override { return "https://github.com/lfabre/simpledualeq-dpf"; }
    const char* getLicense()     const override { return "GPL-3.0-or-later"; }
    uint32_t    getVersion()     const override { return d_version(0, 1, 0); }
    int64_t     getUniqueId()    const override { return d_cconst('S','d','E','q'); }

    void initParameter(uint32_t index, Parameter& parameter) override;
    float getParameterValue(uint32_t index) const override;
    void  setParameterValue(uint32_t index, float value) override;

    void activate() override;
    void run(const float** inputs, float** outputs, uint32_t frames) override;

private:
    sdeq::ParametricEq eq_;
    float paramValues_[sdeq::kNumParameters] {};
    bool  prepared_ = false;
    void pushParamsToDsp();

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleDualEqPlugin)
};

END_NAMESPACE_DISTRHO
