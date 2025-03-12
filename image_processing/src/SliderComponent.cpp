#pragma once
#include <raygui.h>
#include <raylib.h>

class SliderComponent {
public:
  float x = 0;
  float y = 0;
  float value;

  SliderComponent(const float x, const float y, const float min,
                  const float max)
      : x(x), y(y), min(min), max(max) {}

  // render
  bool sliderValueChengedRendered() {
    float new_value = value;
    GuiSliderBar({x, y, 100, 30}, "0", "255", &new_value, min, max);

    bool value_changed = false;
    if (value != new_value) {
      value_changed = true;
    }
    value = new_value;

    return value_changed;
  }

private:
  const float min;
  const float max;
};