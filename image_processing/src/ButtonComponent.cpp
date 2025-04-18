#pragma once

#include <functional>
#include <raylib.h>
#include <string>

class ButtonComponent {
public:
  Rectangle rectangle;
  std::string text;
  std::function<void()> onPress;

  bool hovered = false;

  ButtonComponent() = default;

  ButtonComponent(Rectangle rect, std::string text, auto onPress)
      : text(text), rectangle(rect), onPress(onPress) {};

  void render() const {
    const Color bgColor = hovered ? SKYBLUE : LIGHTGRAY;
    DrawRectangleRec(rectangle, bgColor);

    DrawRectangleLines(rectangle.x, rectangle.y, rectangle.width,
                       rectangle.height,
                       GRAY); // BLUE

    const Color txtColor = hovered ? DARKBLUE : DARKGRAY;
    DrawText(text.c_str(),
             (int)(rectangle.x + rectangle.width / 2 -
                   (float)MeasureText(text.c_str(), 10) / 2),
             (int)rectangle.y + 11, 10,
             DARKGRAY); // DARKBLUE
  }

  void processMouseInput() {
    if (CheckCollisionPointRec(GetMousePosition(), rectangle)) {
      hovered = true;
      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        onPress();
      }
    } else {
      hovered = false;
    }
  }
};

