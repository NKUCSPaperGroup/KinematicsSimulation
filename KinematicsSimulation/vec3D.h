#pragma once
#include <cstddef>
#include <ostream>

class vec3D
{
public:
	vec3D(): vec3D(0, 0, 0)
	{
	}

	vec3D(double x, double y, double z);

	vec3D(const vec3D& vec);

	double length() const;

	double x() const
	{
		return x_;
	}

	double y() const
	{
		return y_;
	}

	double z() const
	{
		return z_;
	}

	vec3D copy() const;

	vec3D unit() const;

	friend vec3D operator+(const vec3D& a, const vec3D& b);

	friend vec3D operator-(const vec3D& a, const vec3D& b);


	/**
	 * \brief dot product
	 * \param a 
	 * \param b 
	 * \return a*b
	 */
	friend double operator*(const vec3D& a, const vec3D& b);

	friend vec3D operator*(const vec3D& a, double b);

	friend vec3D operator/(const vec3D& a, double b);


	/**
	 * \brief cross product
	 * \param b 
	 * \return 
	 */
	vec3D cross(const vec3D& b) const;

	/**
	 * \brief cross product
	 * \param a 
	 * \param b 
	 * \return 
	 */
	static vec3D cross(const vec3D& a, const vec3D& b);

	friend bool operator==(const vec3D& lhs, const vec3D& rhs);

	friend bool operator!=(const vec3D& lhs, const vec3D& rhs);

	friend std::size_t hash_value(const vec3D& obj);

	friend std::ostream& operator<<(std::ostream& os, const vec3D& obj);

private:
	double x_;
	double y_;
	double z_;
};
