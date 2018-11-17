#include "masscenter.h"

bool operator==(const masscenter& lhs, const masscenter& rhs)
{
	return lhs.name_ == rhs.name_;
}

bool operator!=(const masscenter& lhs, const masscenter& rhs)
{
	return !(lhs == rhs);
}

std::size_t hash_value(const masscenter& obj)
{
	std::size_t seed = 0x4257B3DD;
	seed ^= (seed << 6) + (seed >> 2) + 0x29248F08 + hash_value(obj.name_);
	return seed;
}

masscenter::masscenter(const std::string name, const vec3D& position, const vec3D& size, const vec3D& velosity,
                       const double mass,
                       const double q, const double e)
	: massbody(position, size, mass), q_(q), e_(e), name_(name), velosity_(velosity)
{
}

masscenter::masscenter(const std::string name, const basebox& box, const vec3D& velosity, const double mass,
                       const double q, const double e)
	: massbody(box, mass), q_(q), e_(e), name_(name), velosity_(velosity)
{
}

void masscenter::calculate_acceleration()
{
}

void masscenter::move(const double dt)
{
}

void masscenter::clear_force()
{
}

void masscenter::add_force()
{
}

void masscenter::add_gravity_to(masscenter&obj, const double other_mass, const vec3D& position)

{
	const double G = 6.67e-11;
	vec3D fvalue = (obj.position_ - position).unit()*((G * obj.mass*other_mass) /((obj.position_ - position)*(obj.position_ - position)));
	force f{ gravity,fvalue };
}

void masscenter::add_electrostatic_force_to(masscenter& obj, const double other_q, const vec3D& position)
{
	const double K = 9.0e-9;
	vec3D fvalue = (obj.position_ - position).unit()*((K* obj.q_*other_q) / ((obj.position_ - position)*(obj.position_ - position)));
	force f{ electrostatic,fvalue };
}
