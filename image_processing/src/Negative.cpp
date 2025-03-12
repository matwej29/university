#pragma once
#include "./ImageComponent.cpp"
#include "./ImageProcessorBase.cpp"
#include <iostream>
#include <raylib.h>

class NegativeMenu : public ImageProcessorBase {
private:
  ImageComponent &img;

public:
  NegativeMenu(ImageComponent &img) : img(img) {}

  void render() override {
    // Simple render method - no parameters to adjust for negative effect
  }

  // dont suggest proccessImage()
  void processImage() override {
    for (int i = 0; i < img.orgImg.height * img.orgImg.width; ++i) {
      img.pixels[i].r = 255 - img.pixels[i].r;
      img.pixels[i].g = 255 - img.pixels[i].g;
      img.pixels[i].b = 255 - img.pixels[i].b;
    }
    UpdateTexture(img.txr, img.pixels);
  }
};
