#pragma once
#include "./ImageComponent.cpp"
#include "./ImageProcessorBase.cpp"
#include "./SliderComponent.cpp"
#include <iostream>
#include <raylib.h>

class BinarisationMenu : public ImageProcessorBase {
  ImageComponent &img;
  SliderComponent thresholdSlider;
  int threshold = 128;

public:
  BinarisationMenu(ImageComponent &img) 
      : img(img), thresholdSlider(210, 50, 0, 255) {
    thresholdSlider.value = threshold;
  }
  
  void render() override {
    DrawText("Threshold:", 210, 30, 10, DARKGRAY);
    if (thresholdSlider.sliderValueChengedRendered()) {
      img.pixels = LoadImageColors(img.orgImg);
      threshold = thresholdSlider.value;
      processImage();
    }
  }

  // processImage()
};
