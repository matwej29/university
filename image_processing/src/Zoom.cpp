#pragma once

#include "./ButtonComponent.cpp"
#include "./GaussSolve.cpp"
#include "./ImageComponent.cpp"
#include "./ImageProcessorBase.cpp"
#include "./SliderComponent.cpp"
#include "Buttoncomponent.cpp"
#include "raygui.h"
#include <algorithm>
#include <cmath>
#include <raylib.h>
#include <thread>
#include <vector>

class ZoomMenu : public ImageProcessorBase {
private:
  ImageComponent &img;

  SliderComponent slider;
  float zoom_level = 10.;

  ButtonComponent applyButton;

  Texture2D txr;

  Rectangle zoom_src = {100, 100, 100, 100};

  std::vector<std::thread> threads;

public:
  ZoomMenu(ImageComponent &img) : img(img), slider(210, 50, 1, 10) {
    slider.value = zoom_level;
    zoomImage();

    const uint hardware_threads = std::thread::hardware_concurrency();
    threads.resize(hardware_threads);

    applyButton = ButtonComponent({210, 90, 100, 30}, "APPLY",
                                  [&]() { this->processImage(); });
  };

  void render() override {
    if (slider.sliderValueChengedRendered()) {
      img.pixels = LoadImageColors(img.orgImg);
      zoom_level = slider.value;
      // processImage();
    }
    applyButton.processMouseInput();

    processImageRegionSelection();

    drawSelectionRectangle();

    // draw zoomed image
    DrawTexture(txr, 800, 50, WHITE);

    applyButton.render();
  }

  void drawSelectionRectangle() {
    float x = zoom_src.x, y = zoom_src.y;
    float width = zoom_src.width;
    float height = zoom_src.height;
    if (zoom_src.width < 0) {
      width = -zoom_src.width;
      x -= width;
    }
    if (zoom_src.height < 0) {
      height = -zoom_src.height;
      y -= height;
    }
    DrawRectangleLinesEx({img.rect.x + x, img.rect.y + y, width, height}, 2.,
                         BLUE);
  }

  void processImageRegionSelection() {
    const Vector2 mouse_pos = GetMousePosition();

    if (!CheckCollisionPointRec(mouse_pos, img.rect)) {
      return;
    }

    const Vector2 relative_mouse_pos = {mouse_pos.x - img.rect.x,
                                        mouse_pos.y - img.rect.y};

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      zoom_src.x = relative_mouse_pos.x;
      zoom_src.y = relative_mouse_pos.y;

      zoom_src.height = 1;
      zoom_src.width = 1;
    } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      if (zoom_src.width < 0) {
        const float width = -zoom_src.width;
        zoom_src.x -= width;
        zoom_src.width = width;
      }
      if (zoom_src.height < 0) {
        const float height = -zoom_src.height;
        zoom_src.y -= height;
        zoom_src.height = height;
      }
    } else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      const float x0 = zoom_src.x, y0 = zoom_src.y, x1 = relative_mouse_pos.x,
                  y1 = relative_mouse_pos.y;

      const float width = x1 - x0;
      const float height = y1 - y0;

      zoom_src.width = width;
      zoom_src.height = height;
    }

    zoom_src.x = std::clamp<float>(zoom_src.x, 2, img.rect.width - 2);
    zoom_src.y = std::clamp<float>(zoom_src.y, 2, img.rect.height - 2);
  }

  void processImage() override {
    // img.pixels = LoadImageColors(img.orgImg);

    // UpdateTexture(img.txr, img.pixels);

    zoomImage();
  }

  double calculate_interpolated_color(
      const double x, const double y, const int u, const int v,
      std::vector<std::vector<double>> &img_color_frame) {

    std::array<double, 4> y_interpolation;
    for (int row = 0; row < 4; ++row) {
      std::vector<std::vector<double>> mat(4, std::vector<double>(5));
      for (int col = 0; col < 4; ++col) {
        const double xv = (double)col;
        mat[col][0] = xv * xv * xv;
        mat[col][1] = xv * xv;
        mat[col][2] = xv;
        mat[col][3] = 1.;
        mat[col][4] = img_color_frame[row][col];
      }

      const auto coeffs = Gauss::solve_mat(mat);
      const double tx = x - u;
      const double interpolated = coeffs[0] * tx * tx * tx +
                                  coeffs[1] * tx * tx + coeffs[2] * tx +
                                  coeffs[3];

      y_interpolation[row] = interpolated;
    }

    std::vector<std::vector<double>> mat(4, std::vector<double>(5));
    for (int row = 0; row < 4; ++row) {
      const double xv = (double)row;
      mat[row][0] = xv * xv * xv;
      mat[row][1] = xv * xv;
      mat[row][2] = xv;
      mat[row][3] = 1.;
      mat[row][4] = y_interpolation[row];
    }
    const auto coeffs = Gauss::solve_mat(mat);
    const double ty = y - v;
    const double interpolated = coeffs[0] * ty * ty * ty + coeffs[1] * ty * ty +
                                coeffs[2] * ty + coeffs[3];
    return interpolated;
  }

  inline Color processPixel(const ImageComponent &img, const int u, const int v,
                            const double x, const double y) {
    Color c;
    c.a = 255;
    std::vector<std::vector<double>> img_color_frame(4, std::vector<double>(4));

    for (int i = -1; i <= 2; ++i) {
      for (int j = -1; j <= 2; ++j) {
        img_color_frame[j + 1][i + 1] =
            GetImageColor(img.orgImg, u + i, v + j).r;
      }
    }
    c.r = std::round(calculate_interpolated_color(x, y, u, v, img_color_frame));
    c.r = std::clamp<unsigned char>(c.r, 0, 255);

    for (int i = -1; i <= 2; ++i) {
      for (int j = -1; j <= 2; ++j) {
        img_color_frame[j + 1][i + 1] =
            GetImageColor(img.orgImg, u + i, v + j).g;
      }
    }
    c.g = std::round(calculate_interpolated_color(x, y, u, v, img_color_frame));
    c.g = std::clamp<unsigned char>(c.g, 0, 255);

    for (int i = -1; i <= 2; ++i) {
      for (int j = -1; j <= 2; ++j) {
        img_color_frame[j + 1][i + 1] =
            GetImageColor(img.orgImg, u + i, v + j).b;
      }
    }
    c.b = std::round(calculate_interpolated_color(x, y, u, v, img_color_frame));
    c.b = std::clamp<unsigned char>(c.b, 0, 255);

    return c;
  }

#pragma optimize("t") clang loop vectorize(enable) interleave(enable)
  void zoomImage() {
    const int zoom_image_height = zoom_src.height * zoom_level;
    const int zoom_image_width = zoom_src.width * zoom_level;

    Image zoomed_image =
        GenImageColor(zoom_image_width, zoom_image_height, WHITE);
    ImageFormat(&zoomed_image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    Color *zoomed_image_pixels = LoadImageColors(zoomed_image);

    for (int i = 0; i < threads.size(); ++i) {
      threads[i] = std::thread([&, i]() {
        for (int X = i; X < zoom_image_width; X += threads.size()) {
          for (int Y = 0; Y < zoom_image_height; ++Y) {
            const double x = (double)zoom_src.x + (double)X / zoom_level;
            const double y = (double)zoom_src.y + (double)Y / zoom_level;

            const int u = std::trunc(x);
            const int v = std::trunc(y);
            Color c = processPixel(img, u, v, x, y);

            zoomed_image_pixels[X + (Y + 1) * zoom_image_width] = c;
          }
        }
      });
    }
    for (auto &thread : threads) {
      thread.join();
    }

    txr = LoadTextureFromImage(zoomed_image);
    UpdateTexture(txr, zoomed_image_pixels);

    UnloadImage(zoomed_image);
    UnloadImageColors(zoomed_image_pixels);
  }
};
