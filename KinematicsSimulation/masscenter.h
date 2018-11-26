#pragma once
#include "massbody.h"
#include <string>
#include "force.h"
#include "basebox.h"

class masscenter : public massbody
{
public:

	masscenter(const std::string name, const vec3D& position, const vec3D& size, const vec3D& velosity, double mass,
	           double q,
	           double e
	);


	masscenter(const std::string name, const basebox& box, const vec3D& velosity, double mass, double q, double e
	);

	masscenter(const std::string name, const basebox& box, const double mass): masscenter(name, box, vec3D{}, mass, 0,
	                                                                                      1)
	{
	}

	void calculate_acceleration();

	virtual void move(double dt);

	void clear_force();

	void add_force(force&);

	static void add_gravity_to(masscenter& obj, double other_mass);

	static void add_electrostatic_force_to(masscenter& obj, double other_q);


	std::list<force> forces() const
	{
		return forces_;
	}

	static masscenter merge(const std::vector<masscenter>& ms, const std::vector<double>& vs)
	{
		vec3D p, v, a;
		double sum = 0;
		for (size_t i = 0; i < ms.size(); ++i)
		{
			sum += vs[i];
			p += ms[i].position_;
			p *= vs[i];
			v += ms[i].velosity_;
			v *= vs[i];
			a += ms[i].acceleration_;
			a *= vs[i];
		}
		p /= sum;
		v /= sum;
		a /= sum;
		auto re = masscenter{ms[0].name_, p, ms[0].size_, v, ms[0].mass_, ms[0].q_, ms[0].e_};
		re.acceleration_ = a;
		return re;
	}

private:
	double q_;
	double e_;
	std::string name_;
	vec3D velosity_;
	vec3D acceleration_;
	std::list<force> forces_;

public:
	double q() const
	{
		return q_;
	}

	double e() const
	{
		return e_;
	}

	std::string name() const
	{
		return name_;
	}

	vec3D velosity() const
	{
		return velosity_;
	}

	friend bool operator==(const masscenter& lhs, const masscenter& rhs);

	friend bool operator!=(const masscenter& lhs, const masscenter& rhs);

	friend std::size_t hash_value(const masscenter& obj);

	class save_type
	{
	public:
		explicit save_type(const masscenter& o)
			: position(o.position()), v(o.velosity()), foces(o.forces())
		{
		}

		vec3D position;
		vec3D v;
		std::list<force> foces;
	};
};

