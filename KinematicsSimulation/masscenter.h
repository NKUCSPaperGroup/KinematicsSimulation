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

	masscenter(const std::string name, const basebox& box, const double mass);

	void calculate_acceleration();

	virtual void move(double dt);

	void clear_force();

	void add_force(force&);

	static void add_gravity_to(masscenter& obj, double other_mass);

	static void add_electrostatic_force_to(masscenter& obj, double other_q);


	std::list<force> forces() const;

	static masscenter merge(const std::vector<masscenter>& ms, const std::vector<double>& vs);

private:
	double q_;
	double e_;
	std::string name_;
	vec3D velosity_;
	vec3D acceleration_;
	std::list<force> forces_;

public:
	double q() const;

	double e() const;

	std::string name() const;

	vec3D velosity() const;

	friend bool operator==(const masscenter& lhs, const masscenter& rhs);

	friend bool operator!=(const masscenter& lhs, const masscenter& rhs);

	friend std::size_t hash_value(const masscenter& obj);

	class save_type
	{
	public:
		explicit save_type(const masscenter& o);

		vec3D position;
		vec3D v;
		std::list<force> foces;
	};
};

