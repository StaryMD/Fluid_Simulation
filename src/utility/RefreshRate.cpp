#include "utility/RefreshRate.hpp"

#include <algorithm>
#include <cstddef>
#include <limits>
#include <utility>

#include <SFML/System/Clock.hpp>

RefreshRate::RefreshRate() {
  past_fps_buffer_.resize(kFpsBufferSize);
  Reset();
}

bool RefreshRate::IsTimeForNewFrame() const {
  return GetFrameElapsedTime() >= kWantedSecondsPerFrame;
}

void RefreshRate::Reset() {
  frame_counter_ = 0;
  start_clock_ = sf::Clock();
  last_frame_start_clock_ = sf::Clock();

  std::fill(past_fps_buffer_.begin(), past_fps_buffer_.end(), 0.0);
}

double RefreshRate::GetTotalElapsedTime() const {
  return start_clock_.getElapsedTime().asSeconds();
}

double RefreshRate::GetFrameElapsedTime() const {
  return last_frame_start_clock_.getElapsedTime().asSeconds();
}

double RefreshRate::ResetFrameTime() {
  const double elapsed_time = GetFrameElapsedTime();

  past_fps_buffer_[(frame_counter_++) % kFpsBufferSize] = static_cast<double>(1.0) / elapsed_time;

  last_frame_start_clock_.restart();

  return elapsed_time;
}

std::pair<double, double> RefreshRate::GetFpsInfo() const {
  double min_fps = std::numeric_limits<double>::max();
  double avg_fps = 0;

  for (const double fps_count : past_fps_buffer_) {
    min_fps = std::min(min_fps, fps_count);
    avg_fps += fps_count;
  }

  const size_t cat = std::min(frame_counter_, kFpsBufferSize);
  avg_fps /= static_cast<double>(cat);

  return {avg_fps, min_fps};
}
