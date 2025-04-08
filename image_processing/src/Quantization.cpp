#pragma once
#include "./ImageComponent.cpp"
#include "./ImageProcessorBase.cpp"
#include "./SliderComponent.cpp"
#include <iostream>
#include <raylib.h>

class QuantizationMenu : public ImageProcessorBase {
  ImageComponent &img;
  SliderComponent levelsSlider;
  int levels = 4;

public:
  QuantizationMenu(ImageComponent &img) 
      : img(img), levelsSlider(210, 50, 2, 8) {
    levelsSlider.value = levels;
  }
  
  void render() override {
    DrawText("Color Levels:", 210, 30, 10, DARKGRAY);
    if (levelsSlider.sliderValueChengedRendered()) {
      img.pixels = LoadImageColors(img.orgImg);
      levels = static_cast<int>(levelsSlider.value);
      processImage();
    }
  }

  // processImage()
};
