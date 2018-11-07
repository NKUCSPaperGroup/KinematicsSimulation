#pragma once
#include "basebox.h"

class massbody : public basebox
{
public:


	massbody(const vec3D& position, const vec3D& size, const double mass)
		: basebox(position, size),
		  mass_(mass)
	{
	}

	massbody(const basebox& box, const double mass)
		: basebox(box),
		  mass_(mass)
	{
	}

	double mass() const
	{
		return mass_;
	}

	friend bool operator==(const massbody& lhs, const massbody& rhs)
	{
		return &lhs == &rhs;
	}

	friend bool operator!=(const massbody& lhs, const massbody& rhs)
	{
		return !(lhs == rhs);
	}

	friend std::size_t hash_value(const massbody& obj)
	{
		std::size_t seed = 0x16D5E13A;
		seed ^= (seed << 6) + (seed >> 2) + 0x623E7E90 + hash_value(static_cast<const basebox&>(obj));
		seed ^= (seed << 6) + (seed >> 2) + 0x5D3D0131 + static_cast<std::size_t>(obj.mass_);
		return seed;
	}

private:
	double mass_;
};
