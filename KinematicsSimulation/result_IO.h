/**************************************************************
 *  Copyright: Copyright (c) 2018
 *  Created on 2018-12
 *  Author: NKUCSPaperGroup
 *  At: https://github.com/NKUCSPaperGroup
 *  Email: hamiguazzz@qq.com
 **************************************************************/
#pragma once
#include "physics_scene.h"

void write_result(std::ostream& os, std::pair<physics_scene::time_sequence, physics_scene::result>);

void write_result(std::ostream& os,
	std::tuple<physics_scene::time_sequence, std::vector<vec3D>, std::vector<vec3D>>);

std::tuple<physics_scene::time_sequence, std::vector<vec3D>, std::vector<vec3D>>
read_result(std::istream&);
