#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Common.hpp"
#include "Physics.hpp"
#include "utility/EventHandler.hpp"
#include "utility/RefreshRate.hpp"

void HandleInput(sf::RenderWindow& window);

void FillWorld(std::vector<Cell>& particles);

void DrawWorld(const std::vector<Cell>& particles, sf::Uint8* screen_pixels);

int main() {
  sf::RenderWindow window(sf::VideoMode(kScreenWidth, kScreenHeight), "Fluid Simulation");
  window.setVerticalSyncEnabled(true);

  std::array<std::vector<Cell>, 2> particles = {std::vector<Cell>((kWorldWidth + 2) * (kWorldHeight + 2)),
                                                std::vector<Cell>((kWorldWidth + 2) * (kWorldHeight + 2))};

  std::vector<sf::Uint8> screen_pixels(kScreenWidth * kScreenHeight * 4);

  sf::Texture screen_texture;
  if (not screen_texture.create(kScreenWidth, kScreenHeight)) {
    std::cerr << "Texture failed to create\n";
    return EXIT_FAILURE;
  }

  sf::Sprite screen_sprite(screen_texture);

  FillWorld(particles[0]);

  RefreshRate refresh_rate;
  while (window.isOpen()) {
    if (refresh_rate.IsTimeForNewFrame()) {
      const float elapsed_time = static_cast<float>(refresh_rate.ResetFrameTime());
      window.setTitle("Fluid Simulation" + std::to_string(refresh_rate.GetFpsInfo().first));

      HandleInput(window);

      Solve(particles[0], particles[1], elapsed_time);

      DrawWorld(particles[0], screen_pixels.data());

      screen_texture.update(screen_pixels.data());

      window.draw(screen_sprite);
      window.display();
    }
  }

  return 0;
}

void FillWorld(std::vector<Cell>& particles) {
  for (size_t world_y = kWorldHeight / 3; world_y < kWorldHeight * 2 / 3; ++world_y) {
    for (size_t world_x = kWorldWidth / 3; world_x < kWorldWidth * 2 / 3; ++world_x) {
      const size_t particle_idx = world_y * (kWorldWidth + 2) + world_x;
      const float rand_value = 1;

      particles[particle_idx] = {rand_value, sf::Vector2f(10, -10)};
    }
  }
  for (size_t world_y = 0; world_y < kWorldHeight; ++world_y) {
    for (size_t world_x = 0; world_x < kWorldWidth; ++world_x) {
      const size_t particle_idx = world_y * (kWorldWidth + 2) + world_x;

      particles[particle_idx] = {particles[particle_idx].density, sf::Vector2f(1, -1)};
    }
  }
}

void DrawWorld(const std::vector<Cell>& particles, sf::Uint8* screen_pixels) {
  sf::Color* screen_pixels_true = reinterpret_cast<sf::Color*>(screen_pixels);

  for (size_t screen_y = 0; screen_y < kScreenHeight; ++screen_y) {
    for (size_t screen_x = 0; screen_x < kScreenWidth; ++screen_x) {
      const Cell particle = particles[(screen_y / kPixelSize + 1) * (kWorldWidth + 2) + (screen_x / kPixelSize + 1)];
      const uint32_t grayness = static_cast<unsigned>(std::round(particle.density * 255));
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
