#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "utility/EventHandler.hpp"
#include "utility/RefreshRate.hpp"

constexpr size_t kWorldWidth = 120;
constexpr size_t kWorldHeight = 100;

constexpr size_t kPixelSize = 8;

constexpr size_t kScreenWidth = kWorldWidth * kPixelSize;
constexpr size_t kScreenHeight = kWorldHeight * kPixelSize;

void HandleInput(sf::RenderWindow& window);

int main() {
  sf::RenderWindow window(sf::VideoMode(kScreenWidth, kScreenHeight), "Fluid Simulation");
  window.setVerticalSyncEnabled(true);

  RefreshRate refresh_rate;
  while (window.isOpen()) {
    if (refresh_rate.IsTimeForNewFrame()) {
      refresh_rate.ResetFrameTime();

      HandleInput(window);
    }
  }

  return 0;
}

void HandleInput(sf::RenderWindow& window) {
  // const sf::Clock timer;
  EventHandler event_handler;

  event_handler.HandleEvents(window);

  if (window.hasFocus()) {}

  // timer.getElapsedTime().asSeconds();
}
