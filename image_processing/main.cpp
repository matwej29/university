#include "src/Binarisation.cpp"
#include "src/Brightness.cpp"
#include "src/Buttoncomponent.cpp"
#include "src/Contrast.cpp"
#include "src/DefaultImageComponent.cpp"
#include "src/Gamma.cpp"
#include "src/GrayScale.cpp"
#include "src/ImageComponent.cpp"
#include "src/ImageProcessorBase.cpp"
#include "src/Negative.cpp"
#include "src/Quantization.cpp"
#include "src/SliderComponent.cpp"
#include "src/Zoom.cpp"
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
  const int screenWidth = 1200;
  const int screenHeight = 450;

  ImageComponent image_to_process(Rectangle{350, 50, 300, 400});
  
  InitWindow(screenWidth, screenHeight, "Image Processing");

  image_to_process.loadNewImage("./apexLegends.png");
  
  SetTargetFPS(60);

  std::unique_ptr<ImageProcessorBase> selectedProcessor =
      // std::make_unique<DefaultImageMenu>(image_to_process);
      std::make_unique<ZoomMenu>(image_to_process);

  std::vector<ButtonComponent> ImageProcessorButtons = {
      {{40, 10, 150, 30},
       "ORIGINAL",
       [&image_to_process, &selectedProcessor]() {
         selectedProcessor =
             std::make_unique<DefaultImageMenu>(image_to_process);
         selectedProcessor->processImage();
       }},

      {{40, 50, 150, 30},
       "GRAYSCALE",
       [&image_to_process, &selectedProcessor]() {
         selectedProcessor = std::make_unique<GrayScaleMenu>(image_to_process);
         selectedProcessor->processImage();
       }},

      {{40, 90, 150, 30},
       "BRIGHTNESS",
       [&image_to_process, &selectedProcessor]() {
         selectedProcessor = std::make_unique<BrightnessMenu>(image_to_process);
       }},

      {{40, 130, 150, 30},
       "NEGATIVE",
       [&image_to_process, &selectedProcessor]() {
         selectedProcessor = std::make_unique<NegativeMenu>(image_to_process);
         selectedProcessor->processImage();
       }},

      {{40, 170, 150, 30},
       "BINARISATION",
       [&image_to_process, &selectedProcessor]() {
         selectedProcessor =
             std::make_unique<BinarisationMenu>(image_to_process);
       }},

      {{40, 210, 150, 30},
       "MORE CONTRAST",
       [&image_to_process, &selectedProcessor]() {
         selectedProcessor =
             std::make_unique<MoreContrastMenu>(image_to_process);
       }},

      {{40, 250, 150, 30},
       "LESS CONTRAST",
       [&image_to_process, &selectedProcessor]() {
         selectedProcessor =
             std::make_unique<LessContrastMenu>(image_to_process);
       }},

      {{40, 290, 150, 30},
       "GAMMA",
       [&image_to_process, &selectedProcessor]() {
         selectedProcessor = std::make_unique<GammaMenu>(image_to_process);
       }},

      {{40, 330, 150, 30},
       "QUANTIZATION",
       [&image_to_process, &selectedProcessor]() {
         selectedProcessor =
             std::make_unique<QuantizationMenu>(image_to_process);
       }},

      {{40, 370, 150, 30}, "ZOOM", [&image_to_process, &selectedProcessor]() {
        selectedProcessor = std::make_unique<ZoomMenu>(image_to_process);
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
