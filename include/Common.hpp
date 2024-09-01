#ifndef COMMON_H_
#define COMMON_H_

#include <SFML/System/Vector2.hpp>
#include <cstddef>

constexpr size_t kWorldWidth = 120;
constexpr size_t kWorldHeight = 100;

constexpr size_t kPixelSize = 8;

constexpr size_t kScreenWidth = kWorldWidth * kPixelSize;
constexpr size_t kScreenHeight = kWorldHeight * kPixelSize;

constexpr float kDiffusionRate = 0.001;
constexpr size_t kDiffusionIterationCount = 20;

struct Cell {
  float density;
  sf::Vector2f velocity;
};

inline size_t GetIndex(const size_t idx_y, const size_t idx_x) {
  return idx_y * (kWorldWidth + 2) + idx_x;
};

#endif  //COMMON_H_
