#pragma once
#include "./ImageComponent.cpp"
#include "./ImageProcessorBase.cpp"
#include "./SliderComponent.cpp"
#include <iostream>
#include <raylib.h>

class MoreContrastMenu : public ImageProcessorBase {
  ImageComponent &img;
  SliderComponent contrastSlider;
  float contrastFactor = 1.2f;

public:
  MoreContrastMenu(ImageComponent &img) 
      : img(img), contrastSlider(210, 50, 1.0f, 2.0f) {
    contrastSlider.value = contrastFactor;
  }
  
  void render() override {
    DrawText("Increase Contrast:", 210, 30, 10, DARKGRAY);
    if (contrastSlider.sliderValueChengedRendered()) {
      img.pixels = LoadImageColors(img.orgImg);
      contrastFactor = contrastSlider.value;
      processImage();
    }
  }

  // processImage()
};

class LessContrastMenu : public ImageProcessorBase {
  ImageComponent &img;
  SliderComponent contrastSlider;
  float contrastFactor = 0.8f;

public:
  LessContrastMenu(ImageComponent &img) 
      : img(img), contrastSlider(210, 50, 0.2f, 1.0f) {
    contrastSlider.value = contrastFactor;
  }
  
  void render() override {
    DrawText("Decrease Contrast:", 210, 30, 10, DARKGRAY);
    if (contrastSlider.sliderValueChengedRendered()) {
      img.pixels = LoadImageColors(img.orgImg);
      contrastFactor = contrastSlider.value;
      processImage();
    }
  }

  // processImage()
};
