#include "SimpleDualEqUI.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>

START_NAMESPACE_DISTRHO

using DGL_NAMESPACE::Color;

// Short labels in ParamId order (8 params).
static const char* kLabels[sdeq::kNumParameters] = {
    "EQ1 Freq", "EQ1 Gain", "EQ1 Q", "EQ1 On",
    "EQ2 Freq", "EQ2 Gain", "EQ2 Q", "EQ2 On",
};

SimpleDualEqUI::SimpleDualEqUI() : UI(kWidth, kHeight)
{
    setSize(kWidth, kHeight);

    // 4 columns (Freq, Gain, Q, On) x 2 rows (EQ1, EQ2).
    const float marginX = 70.f;
    const float marginY = 90.f;
    const float colSpacing = (kWidth - 2.f * marginX) / 3.f;   // 4 cols -> 3 gaps
    const float rowSpacing = (kHeight - marginY - 50.f);        // 2 rows -> 1 gap
    const float kR = 24.f;

    for (uint32_t i = 0; i < sdeq::kNumParameters; ++i) {
        const uint32_t col = i % 4;
        const uint32_t row = i / 4;
        const float x = marginX + col * colSpacing;
        const float y = marginY + row * rowSpacing;
        knobs_.push_back({x, y, kR, i});
    }

    for (uint32_t i = 0; i < sdeq::kNumParameters; ++i)
        values_[i] = sdeq::rangeFor(i).def;
}

void SimpleDualEqUI::parameterChanged(uint32_t index, float value)
{
    if (index < sdeq::kNumParameters) {
        values_[index] = value;
        repaint();
    }
}

void SimpleDualEqUI::onNanoDisplay()
{
    beginPath();
    rect(0.f, 0.f, kWidth, kHeight);
    fillColor(Color(24, 24, 30));
    fill();

    fontSize(16.f);
    textAlign(ALIGN_LEFT | ALIGN_BASELINE);
    fillColor(Color(200, 200, 210));
    text(14.f, 24.f, "SimpleDualEq  [debug UI]", nullptr);

    fontSize(11.f);
    fillColor(Color(140, 140, 160));
    text(14.f, 44.f, "EQ1 (top)  |  EQ2 (bottom)  —  Freq / Gain / Q / On", nullptr);

    for (const auto& k : knobs_)
        drawKnob(k);
}

void SimpleDualEqUI::drawKnob(const KnobRect& k)
{
    const sdeq::ParamRange r = sdeq::rangeFor(k.paramId);
    const float value = values_[k.paramId];
    const float range = r.max - r.min;
    const float n = (range > 1e-9f) ? std::clamp((value - r.min) / range, 0.f, 1.f) : 0.f;

    beginPath();
    circle(k.x, k.y, k.r);
    if (r.isBool) {
        fillColor(value > 0.5f ? Color(80, 200, 100) : Color(50, 50, 60));
    } else {
        fillColor(Color(42, 42, 52));
    }
    fill();
    strokeColor(Color(130, 130, 150));
    strokeWidth(1.f);
    stroke();

    if (!r.isBool) {
        const float ang = -2.356f + n * 4.712f;  // -135° to +135°
        beginPath();
        moveTo(k.x, k.y);
        lineTo(k.x + std::cos(ang) * k.r * 0.85f,
               k.y + std::sin(ang) * k.r * 0.85f);
        strokeColor(Color(230, 230, 240));
        strokeWidth(2.f);
        stroke();
    }

    char buf[32];
    fontSize(10.f);
    textAlign(ALIGN_CENTER | ALIGN_BASELINE);
    fillColor(Color(180, 180, 200));
    const char* label = (k.paramId < sdeq::kNumParameters) ? kLabels[k.paramId] : "?";
    text(k.x, k.y + k.r + 14.f, label, nullptr);

    std::snprintf(buf, sizeof(buf), "%.2f", value);
    fontSize(9.f);
    fillColor(Color(120, 120, 140));
    text(k.x, k.y + k.r + 26.f, buf, nullptr);
}

bool SimpleDualEqUI::hitTestKnob(const KnobRect& k, float mx, float my, float fudge)
{
    const float dx = mx - k.x, dy = my - k.y;
    return (dx * dx + dy * dy) <= (k.r * k.r * fudge * fudge);
}

bool SimpleDualEqUI::onMouse(const MouseEvent& ev)
{
    if (ev.button != 1) return false;

    if (ev.press) {
        for (std::size_t i = 0; i < knobs_.size(); ++i) {
            if (hitTestKnob(knobs_[i], ev.pos.getX(), ev.pos.getY())) {
                const sdeq::ParamRange r = sdeq::rangeFor(knobs_[i].paramId);
                if (r.isBool) {
                    values_[knobs_[i].paramId] = (values_[knobs_[i].paramId] > 0.5f) ? 0.f : 1.f;
                    setParameterValue(knobs_[i].paramId, values_[knobs_[i].paramId]);
                    repaint();
                    return true;
                }
                draggingIdx_ = int(i);
                dragStartY_  = ev.pos.getY();
                dragStartV_  = values_[knobs_[i].paramId];
                return true;
            }
        }
        return false;
    }

    draggingIdx_ = -1;
    return true;
}

bool SimpleDualEqUI::onMotion(const MotionEvent& ev)
{
    if (draggingIdx_ < 0) return false;

    auto& k = knobs_[draggingIdx_];
    const sdeq::ParamRange r = sdeq::rangeFor(k.paramId);
    const float range = r.max - r.min;
    const float dy    = dragStartY_ - ev.pos.getY();
    float v = dragStartV_ + (dy / 200.f) * range;
    v = std::clamp(v, r.min, r.max);
    values_[k.paramId] = v;
    setParameterValue(k.paramId, v);
    repaint();
    return true;
}

UI* createUI() { return new SimpleDualEqUI(); }

END_NAMESPACE_DISTRHO
