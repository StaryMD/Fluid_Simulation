#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <vector>

#include "Common.hpp"

void DensityDiffuse(const std::vector<Cell>& particles_in, std::vector<Cell>& particles_out, float delta_time);

void DensityAdvect(const std::vector<Cell>& particles_in, std::vector<Cell>& particles_out, float delta_time);

void Solve(std::vector<Cell>& particles_in, std::vector<Cell>& particles_out, float delta_time);

void BoundSet();

#endif  //PHYSICS_H_
