#ifndef REFRESH_RATE_HPP_
#define REFRESH_RATE_HPP_

#include <cstdint>
#include <utility>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

constexpr double kWantedFramesPerSecond = 60.0;
constexpr size_t kFpsBufferSize = 120;

constexpr double kWantedSecondsPerFrame = 1.0 / kWantedFramesPerSecond;

class RefreshRate {
 public:
  using Frame_counter_t = uint64_t;

  RefreshRate();

  void Reset();

  void ResetFrameTime();

  [[nodiscard]] bool IsTimeForNewFrame() const;

  [[nodiscard]] double GetTotalElapsedTime() const;
  [[nodiscard]] double GetFrameElapsedTime() const;

  [[nodiscard]] std::pair<double, double> GetFpsInfo() const;

  [[nodiscard]] Frame_counter_t GetFrameCount() const { return frame_counter_; };

 private:
  Frame_counter_t frame_counter_{};

  sf::Clock start_clock_;
  sf::Clock last_frame_start_clock_;

  std::vector<double> past_fps_buffer_;
};

#endif /* REFRESH_RATE_HPP_ */
