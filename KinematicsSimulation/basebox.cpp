#include "basebox.h"

basebox::basebox(const vec3D& position, const vec3D& size) : position_(position), size_(size)
{
}

bool basebox::is_box_collide(const basebox& box)
{
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
