#pragma once
#include "vec3D.h"
#include <ostream>

/**
 * \brief the type of force
 */
enum force_type
{
};

/**
 * \brief a class to describe Force.
 */
class force : public vec3D
{
	force_type type_;
public:
	force(force_type type, double x, double y, double z);

	force(force_type type, const vec3D& vec);

	force_type type() const
	{
		return type_;
	}


	friend bool operator==(const force& lhs, const force& rhs);

	friend bool operator!=(const force& lhs, const force& rhs);

	friend std::ostream& operator<<(std::ostream& os, const force& obj);
};
