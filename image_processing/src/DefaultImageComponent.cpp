#pragma once

#include "./ImageComponent.cpp"
#include "./ImageProcessorBase.cpp"
#include <cmath>
#include <raylib.h>

class DefaultImageMenu : public ImageProcessorBase {
private:
  ImageComponent &img;

public:
  DefaultImageMenu(ImageComponent &img) : img(img) {};
  void render() override {
    // ?
  }

  void processImage() override {
    img.pixels = LoadImageColors(img.orgImg);

    UpdateTexture(img.txr, img.pixels);
  }
};
