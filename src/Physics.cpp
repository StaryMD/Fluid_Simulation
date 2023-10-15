#include "Physics.hpp"

#include <SFML/System/Vector2.hpp>
#include <algorithm>

#include "Common.hpp"

void Solve(std::vector<Cell>& particles_in, std::vector<Cell>& particles_out, const float delta_time) {
  Diffuse(particles_in, particles_out, delta_time);
  std::swap(particles_in, particles_out);

  Advect(particles_in, particles_out, delta_time);
  std::swap(particles_in, particles_out);
}

void Diffuse(const std::vector<Cell>& particles_in, std::vector<Cell>& particles_out, const float delta_time) {
  const float rate = kDiffusionRate * delta_time * kWorldHeight * kWorldWidth;

  for (size_t iter = 0; iter < kDiffusionIterationCount; ++iter) {
    for (size_t world_y = 1; world_y <= kWorldHeight; ++world_y) {
      for (size_t world_x = 1; world_x <= kWorldWidth; ++world_x) {
        const size_t particle_idx = GetIndex(world_y, world_x);

        const float neighbour_sum = particles_out[GetIndex(world_y - 1, world_x)].density +
                                    particles_out[GetIndex(world_y + 1, world_x)].density +
                                    particles_out[GetIndex(world_y, world_x - 1)].density +
                                    particles_out[GetIndex(world_y, world_x + 1)].density;

        const float new_particle_density = (particles_in[particle_idx].density + rate * neighbour_sum) / (1 + 4 * rate);

        particles_out[particle_idx].density = new_particle_density;
      }
    }
  }
}

void Advect(const std::vector<Cell>& particles_in, std::vector<Cell>& particles_out, const float delta_time) {
  for (size_t world_y = 1; world_y <= kWorldHeight; ++world_y) {
    for (size_t world_x = 1; world_x <= kWorldWidth; ++world_x) {
      const size_t particle_idx = GetIndex(world_y, world_x);

      const sf::Vector2f float_coord = sf::Vector2f(static_cast<float>(world_x - 1), static_cast<float>(world_y - 1));
      sf::Vector2f pos = float_coord + delta_time * particles_in[particle_idx].velocity;

      std::clamp(pos.x, 0.5F, kWorldWidth + 0.5F);
      std::clamp(pos.y, 0.5F, kWorldHeight + 0.5F);

      const int left_coord = static_cast<int>(pos.x);
      const int top_coord = static_cast<int>(pos.y);
      const int right_coord = left_coord + 1;
      const int bott_coord = top_coord + 1;

      const float left_dist = pos.x - static_cast<float>(left_coord);
      const float top_dist = pos.y - static_cast<float>(top_coord);
      const float right_dist = 1 - left_dist;
      const float bott_dist = 1 - top_dist;

      const float left_density = left_dist * (top_dist * particles_in[GetIndex(top_coord, left_coord)].density +
                                              bott_dist * particles_in[GetIndex(bott_coord, left_coord)].density);
      const float right_density = right_dist * (top_dist * particles_in[GetIndex(top_coord, right_coord)].density +
                                                bott_dist * particles_in[GetIndex(bott_coord, right_coord)].density);

      particles_out[particle_idx].density = left_density + right_density;
    }
  }
}
