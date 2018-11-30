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

masscenter::masscenter(const std::string name, const basebox& box, const double mass): masscenter(
	name, box, vec3D{}, mass, 0,
	1)
{
}

std::list<force> masscenter::forces() const
{
	return forces_;
}

masscenter masscenter::merge(const std::vector<masscenter>& ms, const std::vector<double>& vs)
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

double masscenter::q() const
{
	return q_;
}

double masscenter::e() const
{
	return e_;
}

std::string masscenter::name() const
{
	return name_;
}

vec3D masscenter::velosity() const
{
	return velosity_;
}

masscenter::save_type::save_type(const masscenter& o): position(o.position()), v(o.velosity()), foces(o.forces())
{
}

void masscenter::calculate_acceleration()
{
	vec3D sum;
	for (const auto& f : forces_)
	{
		sum = f + sum;
	}
	acceleration_ = sum / this->mass();
}

void masscenter::move(const double dt)
{
	this->calculate_acceleration();
	this->position_ += velosity_ * dt + acceleration_ * (0.5 * dt * dt);
	this->velosity_ += acceleration_ * dt;
}

void masscenter::clear_force()
{
	this->forces_.clear();
}

void masscenter::add_force(force& f)
{
	this->forces_.push_back(f);
}

void masscenter::add_gravity_to(masscenter& obj, const double other_mass)
{
	//@kirakira666
	//TODO
}

void masscenter::add_electrostatic_force_to(masscenter& obj, const double other_q)
{
	//@kirakira666
	//TODO
}
