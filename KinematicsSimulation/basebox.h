#pragma once
#include "vec3D.h"
#include <ostream>
#include <tuple>
#include <xhash>

class basebox
{
protected:
	~basebox() = default;
public:
	basebox(const vec3D& position, const vec3D& size);
	/**
	 * \brief test whether this box is colliding with another one.
	 * \param box the other one 
	 * \return colliding=>true , not=>false
	 */
	bool is_box_collide(const basebox& box) const;
protected:
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


	friend bool operator==(const basebox& lhs, const basebox& rhs)
	{
		return &lhs == &rhs;
	}

	friend bool operator!=(const basebox& lhs, const basebox& rhs);


	friend std::size_t hash_value(const basebox& obj)
	{
		std::size_t seed = 0x3D92BBB2;
		seed ^= (seed << 6) + (seed >> 2) + 0x01857D20 + stdext::hash_value(&obj);
		return seed;
	}

	friend std::ostream& operator<<(std::ostream& os, const basebox& obj);
};
