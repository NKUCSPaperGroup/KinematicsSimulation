#pragma once
#include "field.h"
#include <map>
#include "tracker.h"

class physics_sence
{
	using result = tracker<masscenter, masscenter_save>;
	using result_map = std::map<masscenter, result>;
public:
	void add_field(const field&);
	void add_mass_center(masscenter&, bool = false);
	bool remove(const masscenter& o) { return remove(o.name()); }
	bool remove(const field&);
	bool remove(const std::string&);
	masscenter& get_masscenter(std::string) const;
	void set_st(double = 0);
	void set_dt(double);
	void set_et(double);
	void run();
	void set_SPF(size_t);
	result_map get_result_map() const;
	result get_result() const;
};
