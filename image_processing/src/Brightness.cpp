#pragma once
#include "./ImageComponent.cpp"
#include "./ImageProcessorBase.cpp"
#include "./SliderComponent.cpp"
#include <raylib.h>

class BrightnessMenu : public ImageProcessorBase {
  ImageComponent &img;
  SliderComponent slider;
  int brightnessValue = 0;

public:
  BrightnessMenu(ImageComponent &img) : img(img), slider(210, 50, 0, 255) {}
  void render() {
    if (slider.sliderValueChengedRendered()) {
      img.pixels = LoadImageColors(img.orgImg);
      brightnessValue = slider.value;
      processImage();
    }
  }
  void processImage() {
    std::cout << "Brightness processing" << std::endl;
    for (int i = 0; i < img.orgImg.height * img.orgImg.width; ++i) {
      const auto r = img.pixels[i].r;
      const auto g = img.pixels[i].g;
      const auto b = img.pixels[i].b;

      img.pixels[i].r = std::clamp(r + brightnessValue, 0, 255);
      img.pixels[i].g = std::clamp(g + brightnessValue, 0, 255);
      img.pixels[i].b = std::clamp(b + brightnessValue, 0, 255);
    }

    UpdateTexture(img.txr, img.pixels);
  }
};
