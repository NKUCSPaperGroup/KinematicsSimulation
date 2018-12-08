/**************************************************************
 *  Copyright: Copyright (c) 2018
 *  Created on 2018-12
 *  Author: NKUCSPaperGroup
 *  At: https://github.com/NKUCSPaperGroup
 *  Email: hamiguazzz@qq.com
 **************************************************************/
#pragma once
#include "basebox.h"

class massbody : public basebox
{
public:

	massbody(const vec3D& position, const vec3D& size, const double mass);

	massbody(const basebox& box, const double mass);
	/*
	 * the mass of the body
	 */
	double mass() const;

	friend bool operator==(const massbody& lhs, const massbody& rhs);

	friend bool operator!=(const massbody& lhs, const massbody& rhs);

	friend std::size_t hash_value(const massbody& obj);

protected:
	double mass_;
};

inline massbody::massbody(const vec3D& position, const vec3D& size, const double mass) : basebox(position, size),
mass_(mass)
{
}

inline massbody::massbody(const basebox& box, const double mass) : basebox(box),
mass_(mass)
{
}
/*
 * return the mass of the body
 */
inline double massbody::mass() const
{
	return mass_;
}

inline bool operator==(const massbody& lhs, const massbody& rhs)
{
	return &lhs == &rhs;
}

inline bool operator!=(const massbody& lhs, const massbody& rhs)
{
	return !(lhs == rhs);
}

inline std::size_t hash_value(const massbody& obj)
{
	std::size_t seed = 0x16D5E13A;
	seed ^= (seed << 6) + (seed >> 2) + 0x623E7E90 + hash_value(static_cast<const basebox&>(obj));
	seed ^= (seed << 6) + (seed >> 2) + 0x5D3D0131 + static_cast<std::size_t>(obj.mass_);
	return seed;
}
