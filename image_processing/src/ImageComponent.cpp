#pragma once
#include <raylib.h>
#include <string>

class ImageComponent {
public:
  Image orgImg;
  Texture2D txr;

  Color *pixels;

  ImageComponent() = default;

  // const auto b = pixels[j * img.width + i].b;
  void render() {
    if (!imageLoaded)
      return;
    DrawTexture(txr, 350, 50, WHITE);
  }

  void reset_pixels() { pixels = LoadImageColors(orgImg); }

  void loadNewImage(std::string filePath) {
    orgImg = LoadImage(filePath.c_str());
    ImageFormat(&orgImg, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    ImageResize(&orgImg, 300, 400);
    pixels = LoadImageColors(orgImg);
    txr = LoadTextureFromImage(orgImg);

    imageLoaded = true;
  }

  ~ImageComponent() {
    UnloadImage(orgImg);
    UnloadImageColors(pixels);
    UnloadTexture(txr);
  }

private:
  bool imageLoaded = false;
};
