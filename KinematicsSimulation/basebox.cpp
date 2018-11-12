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
