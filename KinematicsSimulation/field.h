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

	virtual bool is_in_field(double time, masscenter& obj) = 0;

	virtual void add_force(double time, masscenter& obj) = 0;
};
