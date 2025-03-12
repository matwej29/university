#pragma once

#include "./ImageComponent.cpp"
#include "./ImageProcessorBase.cpp"
#include "./SliderComponent.cpp"
#include <cmath>
#include <raylib.h>
#include <iostream>

class GrayScaleMenu : public ImageProcessorBase {
  ImageComponent &img;

public:
  GrayScaleMenu(ImageComponent &img) : img(img) {}
  void render() {}

  void processImage() {
    std::cout << "GrayScale processing" << std::endl;
    for (int i = 0; i < img.orgImg.height * img.orgImg.width; ++i) {
      const auto r = img.pixels[i].r;
      const auto g = img.pixels[i].g;
      const auto b = img.pixels[i].b;

      const unsigned char grayColor = std::round(0.3 * r + 0.59 * g + 0.11 * b);
      img.pixels[i].r = grayColor;
      img.pixels[i].g = grayColor;
      img.pixels[i].b = grayColor;
    }

    UpdateTexture(img.txr, img.pixels);
  }
};
