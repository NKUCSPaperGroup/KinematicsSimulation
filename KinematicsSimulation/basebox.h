#pragma once
#include "vec3D.h"
#include <ostream>

class basebox
{
public:
	basebox(const vec3D& position,const vec3D& size);
	/**
	 * \brief test whether this box is colliding another one.
	 * \param box the other one 
	 * \return colliding=>true , not=>false
	 */
	bool is_box_collide(const basebox& box);
private:
	//position_ is the position of center
	vec3D position_;
	//size_ = {length,width,heigth}
	vec3D size_;

public:
	vec3D position() const
	{
		return position_;
	}

	vec3D size() const
	{
		return size_;
	}

	friend bool operator==(const basebox& lhs, const basebox& rhs);

	friend bool operator!=(const basebox& lhs, const basebox& rhs);

	friend std::ostream& operator<<(std::ostream& os, const basebox& obj);
};

