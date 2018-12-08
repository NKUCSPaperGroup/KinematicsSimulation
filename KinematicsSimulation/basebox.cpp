/**************************************************************
 *  Copyright: Copyright (c) 2018
 *  Created on 2018-12
 *  Author: NKUCSPaperGroup
 *  At: https://github.com/NKUCSPaperGroup
 *  Email: hamiguazzz@qq.com
 **************************************************************/
#include "basebox.h"
#include <cmath>

basebox::basebox(const vec3D& position, const vec3D& size) : position_(position), size_(size)
{
}

bool basebox::is_box_collide(const basebox& box) const
{
#define COMP(x) (std::abs(this->position().x()-box.position().x())*2.0<this->size().x()+box.size().x())
	return COMP(x) && COMP(y) && COMP(z);
}

vec3D basebox::position() const
{
	return position_;
}

vec3D basebox::size() const
{
	return size_;
}

bool operator==(const basebox& lhs, const basebox& rhs)
{
	return &lhs == &rhs;
}

std::size_t hash_value(const basebox& obj)
{
	std::size_t seed = 0x3D92BBB2;
	seed ^= (seed << 6) + (seed >> 2) + 0x01857D20 + stdext::hash_value(&obj);
	return seed;
}

bool operator!=(const basebox& lhs, const basebox& rhs)
{
	return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const basebox& obj)
{
	return os
		<< "{position: " << obj.position_
		<< " ,size: " << obj.size_ << "}";
}