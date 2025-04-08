#pragma once

#include "./ImageComponent.cpp"
#include "./ImageProcessorBase.cpp"
#include "./SliderComponent.cpp"
#include "raygui.h"
#include <cmath>
#include <vector>
#include <raylib.h>

class ZoomMenu : public ImageProcessorBase {
private:
  ImageComponent &img;
  //   ImageComponent zoomed_image = ImageComponent({1000, 50, 100, 100});
  SliderComponent slider;
  float zoom_level = 10.;

  Texture2D txr;

  Rectangle zoom_src = {0, 0, 100, 100};

public:
  ZoomMenu(ImageComponent &img) : img(img), slider(210, 50, 1, 10) {
    slider.value = 10.f;
    zoomImage();
  };

  void render() override {
    if (slider.sliderValueChengedRendered()) {
      img.pixels = LoadImageColors(img.orgImg);
      zoom_level = slider.value;
      processImage();
    }
    // zoomed_image.render();

    DrawTexture(txr, 800, 50, WHITE);
  }

  void processImage() override {
    // img.pixels = LoadImageColors(img.orgImg);

    // UpdateTexture(img.txr, img.pixels);

    zoomImage();
  }

  void calculate_pixel() {
    // i need to get 16 pixels, 4x4
    // than i need to do 4 interpolations on horizontal
    // than i need to interpolate vertical values

    // and do it for each color

    // when i do interpolation
    // i need to find a,b,c,d from ax^3 + bx^2 + cx + d
    // i can find them from solvig linear equations
  }

#pragma optimize("t") clang loop vectorize(enable) interleave(enable)
  void zoomImage() {
    const int zoom_image_height = zoom_src.height * zoom_level;
    const int zoom_image_width = zoom_src.width * zoom_level;

    Image zoomed_image =
        GenImageColor(zoom_image_height, zoom_image_width, PURPLE);
    ImageFormat(&zoomed_image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    Color *zoomed_image_pixels = LoadImageColors(zoomed_image);

    const auto linInt = [](int c1, int c2, float t) {
      return c1 + (c2 - c1) * t;
    };

    for (int X = 2; X < zoom_image_width - 2; ++X) {
      for (int Y = 2; Y < zoom_image_height - 2; ++Y) {
        const float x = zoom_src.x + X / zoom_level;
        const float y = zoom_src.y + Y / zoom_level;

        const int u = std::floor(x);
        const int v = std::floor(y);
        Color c = Color{0, 0, 0, 255};

        std::vector<std::vector<double>> img_color_frame(4, std::vector<double>(4));
        for (int i=-1;i<=2;++i){
            for(int j=-1;j<=2;++j){
                img_color_frame[i+2][j+2] = GetImageColor(img.orgImg, u+i, v+j).r;
            }
        }

        // for each horizontal line
        for(int i=0;i<4;++i){
            // i need to calculate 4 linear equations
            // for each point (x = color value)
            // then find abcd
            // calculate color in interpolation point (x)
        }

        // for resulting 4 interpolated points
        // i need to calculate 4 linear equations
        // for each point (x = interpolated color value)
        // then find abcd
        // calculate color in interpolation point (y) // y???

        // put interpolated color in pixel

        

        // const Color c1 = GetImageColor(img.orgImg, u, v);
        // const Color c2 = GetImageColor(img.orgImg, u + 1, v);

        // c.r = linInt(c1.r, c2.r, x - u);
        // c.g = linInt(c1.g, c2.g, x - u);
        // c.b = linInt(c1.b, c2.b, x - u);

        zoomed_image_pixels[X + Y * zoom_image_width] = c;
      }
    }
    txr = LoadTextureFromImage(zoomed_image);
    UpdateTexture(txr, zoomed_image_pixels);

    UnloadImage(zoomed_image);
  }
};
