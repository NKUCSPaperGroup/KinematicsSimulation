#pragma once
#include "basebox.h"
#include "masscenter.h"

class field : public basebox
{
public:

	field(const vec3D& position, const vec3D& size)
		: basebox(position, size)
	{
	}

	field(const field& f) = default;

	virtual ~field() = default;


	friend bool operator==(const field& lhs, const field& rhs)
	{
		return static_cast<const basebox&>(lhs) == static_cast<const basebox&>(rhs);
	}

	friend bool operator!=(const field& lhs, const field& rhs)
	{
		return !(lhs == rhs);
	}

	virtual bool is_in_field(double time, masscenter& obj) { return false; }

	virtual void add_force(double time, masscenter& obj)
	{
	}
};
