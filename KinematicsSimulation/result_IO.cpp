#include "result_IO.h"
#include "physics_sence.h"
#include <ostream>
#include "vec3D.h"

inline std::ostream& write_3d_date(std::ostream& os, const vec3D& v)
{
	os << v.x() << '\t' << v.y() << '\t' << v.z() << '\t';
	return os;
}

inline double next_double(const std::string& s, int& p1, int& p2)
{
	double d;
	p2 = s.find('\t', p1 + 1);
	d = atof(s.substr(p1 + 1, p2).c_str());
	p1 = p2;
	return d;
}

void write_result(std::ostream& os, std::pair<physics_sence::time_squence, physics_sence::result> r)
{
	for (size_t i = 0; i < r.first.size(); ++i)
	{
		os << r.first[i];
		os << '\t';
		write_3d_date(os, r.second->history()->at(i)->position);
		os << '\t';
		write_3d_date(os, r.second->history()->at(i)->v);
		os << '\n';
	}
}

void write_result(std::ostream& os, std::tuple<physics_sence::time_squence, std::vector<vec3D>, std::vector<vec3D>> r)
{
	physics_sence::time_squence ts{};
	std::vector<vec3D> ps{};
	std::vector<vec3D> vs{};
	std::tie(ts, ps, vs) = r;
	for (size_t i = 0; i < ts.size(); ++i)
	{
		os << ts.at(i);
		os << '\t';
		write_3d_date(os, ps.at(i));
		os << '\t';
		write_3d_date(os, vs.at(i));
		os << '\n';
	}
}

std::tuple<physics_sence::time_squence, std::vector<vec3D>, std::vector<vec3D>> read_result(std::istream& is)
{
	physics_sence::time_squence ts{};
	std::vector<vec3D> ps{};
	std::vector<vec3D> vs{};
	while (!is.eof())
	{
		std::string s;
		int p1 = -1, p2;
		std::getline(is, s);
#define D next_double(s,p1,p2)
		ts.push_back(D);
		ps.emplace_back(D,D,D);
		vs.emplace_back(D,D,D);
#undef D
	}
	return {ts, ps, vs};
}
