#include "vec3D.h"

vec3D::vec3D(const double x,const double y,const double z)
{
}

vec3D::vec3D(const vec3D & vec)
{
}

double vec3D::length() const
{
}

vec3D vec3D::copy() const
{
	
}

vec3D vec3D::unit() const
{
}

vec3D cross(const vec3D& a,const vec3D& b)
{
}

vec3D vec3D::cross(const vec3D& b)
{
}

vec3D operator+(const vec3D& a, const vec3D& b)
{
}

vec3D operator-(const vec3D& a, const vec3D& b)
{
}

double operator*(const vec3D& a, const vec3D& b)
{
}

vec3D operator*(const vec3D& a, double b)
{
}

vec3D operator/(const vec3D& a, double b)
{
}

bool operator==(const vec3D& lhs, const vec3D& rhs)
{
	return lhs.x_ == rhs.x_
		&& lhs.y_ == rhs.y_
		&& lhs.z_ == rhs.z_;
}

bool operator!=(const vec3D& lhs, const vec3D& rhs)
{
	return !(lhs == rhs);
}

std::size_t hash_value(const vec3D& obj)
{
	std::size_t seed = 0x6202DA29;
	seed ^= (seed << 6) + (seed >> 2) + 0x3F0197E1 + static_cast<std::size_t>(obj.x_);
	seed ^= (seed << 6) + (seed >> 2) + 0x6D5668CA + static_cast<std::size_t>(obj.y_);
	seed ^= (seed << 6) + (seed >> 2) + 0x61E1641E + static_cast<std::size_t>(obj.z_);
	return seed;
}

std::ostream& operator<<(std::ostream& os, const vec3D& obj)
{
	return os << "(" << obj.x_ << "," << obj.y_ << "," << obj.z_ << ")";
}
