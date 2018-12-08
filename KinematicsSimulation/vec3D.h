/**************************************************************
 *  Copyright: Copyright (c) 2018
 *  Created on 2018-12
 *  Author: NKUCSPaperGroup
 *  At: https://github.com/NKUCSPaperGroup
 *  Email: hamiguazzz@qq.com
 **************************************************************/
#pragma once
#include <cstddef>
#include <ostream>

class vec3D
{
public:
	vec3D() : vec3D(0, 0, 0)
	{
	}
	/*
	 * define  the cooridinate of the vector
	 */
	vec3D(double x, double y, double z);

	vec3D(const vec3D& vec);

	/*
	 * return the length of a vector
	 */
	double length() const;
	/*
	 * return the x cooridinate of a vector
	 */
	double x() const
	{
		return x_;
	}
	/*
	 * return the y cooridinate of a vector
	 */
	double y() const
	{
		return y_;
	}
	/*
	 * return the z cooridinate of a vector
	 */
	double z() const
	{
		return z_;
	}
	/*
     * copy the vector
	 */
	vec3D copy() const;
	/*
	 * define the unit vector
	 */
	vec3D unit() const;

	friend vec3D operator+(const vec3D& a, const vec3D& b);
	/*
	 * the sum of the vector
	 */
	vec3D& operator+=(const vec3D&);
	/*
	 * the substact of the vector
	 */
	vec3D& operator-=(const vec3D&);
	/*
	 * the dot of the vector
	 */
	vec3D& operator*=(double);
	/*
	 * the divide of the vector
	 */
	vec3D& operator/=(double);

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
