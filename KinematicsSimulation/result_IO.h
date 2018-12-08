#pragma once
#include "physics_scene.h"

void write_result(std::ostream& os, std::pair<physics_scene::time_sequence, physics_scene::result>);

void write_result(std::ostream& os,
                  std::tuple<physics_scene::time_sequence, std::vector<vec3D>, std::vector<vec3D>>);

std::tuple<physics_scene::time_sequence, std::vector<vec3D>, std::vector<vec3D>>
read_result(std::istream&);
