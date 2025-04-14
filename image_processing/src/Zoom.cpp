#pragma once

#include "./GaussSolve.cpp"
#include "./ImageComponent.cpp"
#include "./ImageProcessorBase.cpp"
#include "./SliderComponent.cpp"
#include "raygui.h"
#include <algorithm>
#include <cmath>
#include <raylib.h>
#include <stdexcept>
#include <vector>

class ZoomMenu : public ImageProcessorBase {
private:
  ImageComponent &img;
  //   ImageComponent zoomed_image = ImageComponent({1000, 50, 100, 100});
  SliderComponent slider;
  float zoom_level = 10.;

  Texture2D txr;

  Rectangle zoom_src = {100, 100, 100, 100};

public:
  ZoomMenu(ImageComponent &img) : img(img), slider(210, 50, 1, 10) {
    slider.value = zoom_level;
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
  }

#pragma optimize("t") clang loop vectorize(enable) interleave(enable)
  void zoomImage() {
    const int zoom_image_height = zoom_src.height * zoom_level;
    const int zoom_image_width = zoom_src.width * zoom_level;

    Image zoomed_image =
        GenImageColor(zoom_image_height, zoom_image_width, WHITE);
    ImageFormat(&zoomed_image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    Color *zoomed_image_pixels = LoadImageColors(zoomed_image);

    const auto linInt = [](int c1, int c2, float t) {
      return c1 + (c2 - c1) * t;
    };

    for (int X = 0; X < zoom_image_width; ++X) {
      for (int Y = 0; Y < zoom_image_height; ++Y) {
        const double x = (double)zoom_src.x + (double)X / zoom_level;
        const double y = (double)zoom_src.y + (double)Y / zoom_level;

        const int u = std::trunc(x);
        const int v = std::trunc(y);
        Color c = Color{0, 0, 0, 255};

        std::vector<std::vector<double>> img_color_frame(
            4, std::vector<double>(4));

        const auto calculate_interpolated_color = [&]() {
          std::array<double, 4> y_interpolation;
          // for each horizontal line
          for (int row = 0; row < 4; ++row) {
            std::vector<std::vector<double>> mat(4, std::vector<double>(5));
            for (int col = 0; col < 4; ++col) {
              double xv = (double)col;
              mat[col][0] = xv * xv * xv;
              mat[col][1] = xv * xv;
              mat[col][2] = xv;
              mat[col][3] = 1.;
              mat[col][4] = img_color_frame[row][col];
            }

            // Solve for a,b,c,d
            const auto coeffs = Gauss::solve_mat(mat);
            const double tx = x - u;
            const double interpolated = coeffs[0] * tx * tx * tx +
                                        coeffs[1] * tx * tx + coeffs[2] * tx +
                                        coeffs[3];

            // Store this horizontal result in a temporary array
            y_interpolation[row] = interpolated;
          }
          std::vector<std::vector<double>> mat(4, std::vector<double>(5));
          for (int row = 0; row < 4; ++row) {
            double xv = (double)row;
            mat[row][0] = xv * xv * xv;
            mat[row][1] = xv * xv;
            mat[row][2] = xv;
            mat[row][3] = 1.;
            mat[row][4] = y_interpolation[row];
          }
          const auto coeffs = Gauss::solve_mat(mat);
          const double ty = y - v;
          const double interpolated = coeffs[0] * ty * ty * ty +
                                      coeffs[1] * ty * ty + coeffs[2] * ty +
                                      coeffs[3];
          return interpolated;
        };

        for (int i = -1; i <= 2; ++i) {
          for (int j = -1; j <= 2; ++j) {
            img_color_frame[j + 1][i + 1] =
                GetImageColor(img.orgImg, u + i, v + j).r;
          }
        }
        c.r = std::round(calculate_interpolated_color());
        c.r = std::clamp<unsigned char>(c.r, 0, 255);

        for (int i = -1; i <= 2; ++i) {
          for (int j = -1; j <= 2; ++j) {
            img_color_frame[j + 1][i + 1] =
                GetImageColor(img.orgImg, u + i, v + j).g;
          }
        }
        c.g = std::round(calculate_interpolated_color());
        c.g = std::clamp<unsigned char>(c.g, 0, 255);

        for (int i = -1; i <= 2; ++i) {
          for (int j = -1; j <= 2; ++j) {
            img_color_frame[j + 1][i + 1] =
                GetImageColor(img.orgImg, u + i, v + j).b;
          }
        }
        c.b = std::round(calculate_interpolated_color());
        c.b = std::clamp<unsigned char>(c.b, 0, 255);

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
