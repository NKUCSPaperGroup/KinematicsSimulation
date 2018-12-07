#pragma once
#include "vec3D.h"
#include <ostream>
#include <tuple>
#include <xhash>

class basebox
{
public:
	basebox(const vec3D& position, const vec3D& size);
	virtual ~basebox() = default;
	/**
	 * \brief test whether this box is colliding with another one.
	 * \param box the other one 
	 * \return colliding=>true , not=>false
	 */
	virtual bool is_box_collide(const basebox& box) const;
protected:
	//position_ is the position of center
	vec3D position_;
	//size_ = {length,width,heigth}
	vec3D size_;

public:
	vec3D position() const;

	vec3D size() const;


	friend bool operator==(const basebox& lhs, const basebox& rhs);

	friend bool operator!=(const basebox& lhs, const basebox& rhs);


	friend std::size_t hash_value(const basebox& obj);

	friend std::ostream& operator<<(std::ostream& os, const basebox& obj);
};
