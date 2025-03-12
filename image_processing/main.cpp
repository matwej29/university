#include "src/Buttoncomponent.cpp"
#include "src/DefaultImageComponent.cpp"
#include "src/GrayScale.cpp"
#include "src/ImageComponent.cpp"
#include "src/ImageProcessorBase.cpp"
#include "src/SliderComponent.cpp"
#include "src/Brightness.cpp"
#include "src/Negative.cpp"
#include <algorithm>
#include <cmath>
#include <functional>
#include <memory>
#include <raylib.h>
#include <thread>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include <iostream>
#include <string>
#include <vector>

void loadImageFromDroppedFiles(ImageComponent &img) {
  FilePathList droppedFiles = LoadDroppedFiles();

  std::string filePath = droppedFiles.paths[0];

  img.loadNewImage(filePath);

  UnloadDroppedFiles(droppedFiles);
}

void setGrayScale(ImageComponent &img) {}

void setBrightness(ImageComponent &img, int value) {
  for (int i = 0; i < img.orgImg.height * img.orgImg.width; ++i) {
    const auto r = img.pixels[i].r;
    const auto g = img.pixels[i].g;
    const auto b = img.pixels[i].b;

    img.pixels[i].r = std::clamp(r + value, 0, 255);
    img.pixels[i].g = std::clamp(g + value, 0, 255);
    img.pixels[i].b = std::clamp(b + value, 0, 255);
  }

  UpdateTexture(img.txr, img.pixels);
}

// grayScale
// brightness
// negative
// binary
// moreContrast
// lessContrast
// gamma
// kvantation
// revert

// histogram

int main() {
  const int screenWidth = 800;
  const int screenHeight = 450;

  ImageComponent image_to_process;

  InitWindow(screenWidth, screenHeight, "Image Processing");

  SetTargetFPS(60);

  std::unique_ptr<ImageProcessorBase> selectedProcessor =
      std::make_unique<DefaultImageMenu>(image_to_process);

  std::vector<ButtonComponent> ImageProcessorButtons = {
      {{40, 10, 150, 30}, "ORIGINAL", [&image_to_process, &selectedProcessor]() {
         selectedProcessor = std::make_unique<DefaultImageMenu>(image_to_process);
         selectedProcessor->processImage();
       }},

      {{40, 50, 150, 30},
       "GRAYSCALE",
       [&image_to_process, &selectedProcessor]() {
         selectedProcessor = std::make_unique<GrayScaleMenu>(image_to_process);
         selectedProcessor->processImage();
       }},

      {{40, 90, 150, 30}, "BRIGHTNESS", [&image_to_process, &selectedProcessor]() {
         selectedProcessor =
             std::make_unique<BrightnessMenu>(image_to_process);
       }},
      {{40, 130, 150, 30}, "NEGATIVE", [&image_to_process, &selectedProcessor]() {
         selectedProcessor = std::make_unique<NegativeMenu>(image_to_process);
         selectedProcessor->processImage();
       }},
      };

  // Exit by pressing ESC or close button
  while (!WindowShouldClose()) {
    if (IsFileDropped()) {
      loadImageFromDroppedFiles(image_to_process);
    }

    // Check button hover
    for (auto &btn : ImageProcessorButtons) {
      btn.processMouseInput();
    }

    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawFPS(10, 10);

    image_to_process.render();

    std::for_each(ImageProcessorButtons.begin(), ImageProcessorButtons.end(),
                  [](const auto &btn) { btn.render(); });

    selectedProcessor->render();

    // for (const auto &btn : ImageProcessorButtons) {
    //   btn.render();
    // }

    // if (simpleSlider.sliderValueChengedRendered()) {
    // refresh image
    // grayscle
    // brightness
    // depends on slider

    // active обработчик()
    // }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
