#pragma once
#include "./ImageComponent.cpp"
#include <iostream>
#include <raylib.h>

class ImageProcessorBase {
public:
  virtual void processImage() {
    std::cout << "Base processImage called" << std::endl;
  };
  virtual void render() {};
};
