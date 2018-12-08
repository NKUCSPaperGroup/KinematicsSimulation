/**************************************************************
 *  Copyright: Copyright (c) 2018
 *  Created on 2018-12
 *  Author: NKUCSPaperGroup
 *  At: https://github.com/NKUCSPaperGroup
 *  Email: hamiguazzz@qq.com
 **************************************************************/
#include "force.h"

force::force(const force_type type, const double x, const double y, const double z) : vec3D(x, y, z), type_(type)
{
}

force::force(const force_type type, const vec3D& vec) : vec3D(vec), type_(type)
{
}

bool operator==(const force& lhs, const force& rhs)
{
	return static_cast<const vec3D&>(lhs) == static_cast<const vec3D&>(rhs)
		&& lhs.type_ == rhs.type_;
}

bool operator!=(const force& lhs, const force& rhs)
{
	return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const force& obj)
{
	return os
		<< " type: " << obj.type_ << "="
		<< static_cast<const vec3D&>(obj);
}