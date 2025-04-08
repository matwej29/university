#pragma once
#include "./ImageComponent.cpp"
#include "./ImageProcessorBase.cpp"
#include "./SliderComponent.cpp"
#include <iostream>
#include <raylib.h>

class GammaMenu : public ImageProcessorBase {
  ImageComponent &img;
  SliderComponent gammaSlider;
  float gammaValue = 1.0f;

public:
  GammaMenu(ImageComponent &img) 
      : img(img), gammaSlider(210, 50, 0.1f, 3.0f) {
    gammaSlider.value = gammaValue;
  }
  
  void render() override {
    DrawText("Gamma Correction:", 210, 30, 10, DARKGRAY);
    if (gammaSlider.sliderValueChengedRendered()) {
      img.pixels = LoadImageColors(img.orgImg);
      gammaValue = gammaSlider.value;
      processImage();
    }
  }

  // processImage()
};
