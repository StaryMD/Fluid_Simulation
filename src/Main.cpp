#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "utility/EventHandler.hpp"
#include "utility/RefreshRate.hpp"

constexpr size_t kWorldWidth = 120;
constexpr size_t kWorldHeight = 100;

constexpr size_t kPixelSize = 8;

constexpr size_t kScreenWidth = kWorldWidth * kPixelSize;
constexpr size_t kScreenHeight = kWorldHeight * kPixelSize;

void HandleInput(sf::RenderWindow& window);

void DrawWorld(const std::vector<float>& densities, sf::Uint8* screen_pixels);

int main() {
  sf::RenderWindow window(sf::VideoMode(kScreenWidth, kScreenHeight), "Fluid Simulation");
  window.setVerticalSyncEnabled(true);

  std::vector<float> densities(kWorldWidth * kWorldHeight);

  std::vector<sf::Uint8> screen_pixels(kScreenWidth * kScreenHeight * 4);

  sf::Texture screen_texture;
  if (not screen_texture.create(kScreenWidth, kScreenHeight)) {
    std::cerr << "Texture failed to create\n";
    return EXIT_FAILURE;
  }

  sf::Sprite screen_sprite(screen_texture);

  for (float& density : densities) {
    density = static_cast<float>(rand()) / RAND_MAX;
  }

  RefreshRate refresh_rate;
  while (window.isOpen()) {
    if (refresh_rate.IsTimeForNewFrame()) {
      refresh_rate.ResetFrameTime();

      HandleInput(window);

      DrawWorld(densities, screen_pixels.data());

      screen_texture.update(screen_pixels.data());

      window.draw(screen_sprite);
      window.display();
    }
  }

  return 0;
}

void DrawWorld(const std::vector<float>& densities, sf::Uint8* screen_pixels) {
  sf::Color* screen_pixels_true = reinterpret_cast<sf::Color*>(screen_pixels);

  for (size_t screen_y = 0; screen_y < kScreenHeight; ++screen_y) {
    for (size_t screen_x = 0; screen_x < kScreenWidth; ++screen_x) {
      const float density = densities[(screen_y / kPixelSize) * kWorldWidth + (screen_x / kPixelSize)];
      const uint32_t grayness = static_cast<unsigned>(std::round(density * 256));
      const sf::Color color_value = sf::Color(grayness, grayness, grayness);

      *(screen_pixels_true++) = color_value;
    }
  }
}

void HandleInput(sf::RenderWindow& window) {
  // const sf::Clock timer;
  EventHandler event_handler;

  event_handler.HandleEvents(window);

  if (window.hasFocus()) {
    if (event_handler.IsKeyPressed(sf::Keyboard::Escape)) {
      window.close();
    }
  }

  // timer.getElapsedTime().asSeconds();
}
