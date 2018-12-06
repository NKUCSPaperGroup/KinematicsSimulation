#pragma once
#include "physics_sence.h"

void write_result(std::ostream& os, std::pair<physics_sence::time_squence, physics_sence::result>);

void write_result(std::ostream& os,
                  std::tuple<physics_sence::time_squence, std::vector<vec3D>, std::vector<vec3D>>);

std::tuple<physics_sence::time_squence, std::vector<vec3D>, std::vector<vec3D>>
read_result(std::istream&);
