#pragma once
#include "field.h"
#include <map>
#include "tracker.h"

class physics_sence
{
	class frame;
	friend frame;
	using pf = std::shared_ptr<frame>;
	using result = tracker<masscenter, masscenter_save>;
	using time_squence = std::vector<double>;
	using result_map = std::pair<time_squence, std::map<std::string, result>>;
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
	void set_SPF(size_t);

	void run();

	result_map get_result_map() const
	{
		return result_;
	}

	std::pair<time_squence, result> get_result(const std::string& name) const
	{
		return std::make_pair(result_.first, result_.second.at(name));
	}

private:
	class frame
	{
		friend physics_sence;
	private:

		void update_extra_force();

		void update_internal_force();

		std::list<field> fields_;
		std::list<masscenter> objs_;
	};

	void update_time_squence(const double time)
	{
		result_.first.push_back(time);
	}

	void update_track()
	{
		for (auto pair : result_.second)
		{
			pair.second.trace();
		}
	}

	pf inti_;
	pf present_;
	std::vector<pf> frame_pool_;
	result_map result_;
};
