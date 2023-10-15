#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <vector>

#include "Common.hpp"

void Diffuse(const std::vector<Cell>& particles_in, std::vector<Cell>& particles_out, float delta_time);

void Advect(const std::vector<Cell>& particles_in, std::vector<Cell>& particles_out, float delta_time);

void Solve(std::vector<Cell>& particles_in, std::vector<Cell>& particles_out, float delta_time);

#endif  //PHYSICS_H_
