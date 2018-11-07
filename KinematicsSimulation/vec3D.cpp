#include "vec3D.h"

vec3D::vec3D(const double x, const double y, const double z) : x_(x), y_(y), z_(z)
{
}

vec3D::vec3D(const vec3D& vec) : vec3D(vec.x_, vec.y_, vec.z_)
{
}

double vec3D::length() const
{
	return sqrt(x() * x() + y() * y() + z() * z());
}

vec3D vec3D::copy() const
{
	return vec3D{this->x(), this->y(), this->z()};
}

vec3D vec3D::unit() const
{
	return *this / this->length();
}

vec3D vec3D::cross(const vec3D& b) const
{
	return cross(*this, b);
}

vec3D vec3D::cross(const vec3D& a, const vec3D& b)
{
	return vec3D(a.y_ * b.z_ - a.z_ * b.y_, a.z_ * b.z_ - a.x_ * b.z_, a.x_ * b.y_ - a.y_ * b.x_);
}

vec3D operator+(const vec3D& a, const vec3D& b)
{
	return vec3D(a.x_ + b.x_, a.y_ + b.y_, a.z_ + b.z_);
}

vec3D operator-(const vec3D& a, const vec3D& b)
{
	return vec3D(a.x_ - b.x_, a.y_ - b.y_, a.z_ - b.z_);
}

double operator*(const vec3D& a, const vec3D& b)
{
	return a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_;
}

vec3D operator*(const vec3D& a, double b)
{
	return vec3D{b * (a.x_), b * (a.y_), b * (a.z_)};
}

vec3D operator/(const vec3D& a, double b)
{
	return vec3D((a.x_) / b, (a.y_) / b, (a.z_) / b);
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
