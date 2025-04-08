#pragma once
#include <raylib.h>
#include <string>

class ImageComponent {
public:
  Image orgImg;
  Texture2D txr;

  Rectangle rect;

  Color *pixels;

  ImageComponent(Rectangle rect): rect(rect)  {};

  void render() {
    if (!imageLoaded)
      return;
    DrawTexture(txr, rect.x, rect.y, WHITE);
  }

  void reset_pixels() { pixels = LoadImageColors(orgImg); }

  void loadNewImage(const std::string &filePath) {
    orgImg = LoadImage(filePath.c_str());
    ImageFormat(&orgImg, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    ImageResize(&orgImg, rect.width, rect.height);
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
