#pragma once
#include "field.h"
#include <map>
#include "tracker.h"
#include "octree.h"

class physics_sence
{
	class frame;
	friend frame;
	using pf = std::shared_ptr<frame>;
	using pm = std::shared_ptr<masscenter>;
public:
	using result = std::shared_ptr<tracker<masscenter, masscenter::save_type>>;
	using time_squence = std::vector<double>;
	using result_map = std::pair<time_squence, std::map<std::string, result>>;

	template <typename Field>
	void add_field(const Field& f);

	void add_mass_center(const masscenter&, bool = false);

	void remove(const masscenter& o);

	void remove(const field& f);

	void remove(const std::string&);

	struct run_setting
	{
		double start_time = 0;
		double end_time = 1;
		double delta_time = 1e-4;
		double reaction_time = 3e-4;
		double save_duration = 1e-2;
	};

	run_setting& setting();

	void run();

	double current_time() const;

	result_map get_result_map() const;
	std::pair<time_squence, result> get_result(const std::string&) const;

private:
	class frame
	{
	public:
		void run();
		explicit frame(physics_sence& scene);
		frame(const frame&);

		pf next_frame();
		double get_time() const;
	private:

		void clear_force();

		void update_extra_force();

		void update_internal_force();


		void update_objs();

		struct collide_reaction
		{
			pm masscenter;
			std::shared_ptr<force> f;
			double action_time;

			collide_reaction(const pm masscenter, const vec3D& fv, const double action_time);
			collide_reaction() = default;
		};

		void colliding_attach() const;

		void update_collide_force() const;

		void remove_overtime_colliding() const;

		void add_new_colliding() const;

		bool isnot_this_colliding_added(const std::pair<pm, pm>&) const;

		void build_new_colliding(const std::pair<pm, pm>) const;

		pf build_next_frame(double);

		double time_;

		std::list<pm> objs_;

		std::shared_ptr<std::map<std::string, collide_reaction>> colliding_;

		physics_sence& scene_;
	};

	void run_inti();

	void next_frame();

	bool time_to_save();

	void update_time_squence(const double time);

	void update_track();

	run_setting setting_;

	pf inti_;
	pf present_;

	std::list<std::shared_ptr<field>> fields_;
	std::list<pm> objs_;
	octree<pm> treed_objs_;

	result_map result_;
};

template <typename Field>
void physics_sence::add_field(const Field& f)
{
	this->fields_.push_back(std::make_shared<Field>(f));
}

inline void physics_sence::remove(const masscenter& o)
{
	return remove(o.name());
}

inline void physics_sence::remove(const field& f)
{
	for (auto&& field : this->fields_)
	{
		if (*field == f)
		{
			this->fields_.remove(field);
			return;
		}
	}
}

inline physics_sence::run_setting& physics_sence::setting()
{
	return setting_;
}

inline double physics_sence::current_time() const
{
	return this->present_->get_time();
}

inline double physics_sence::frame::get_time() const
{
	return time_;
}

inline void physics_sence::run_inti()
{
	this->inti_ = std::make_shared<frame>(*this);
	this->present_ = std::make_shared<frame>(*inti_);
}

inline void physics_sence::next_frame()
{
	this->present_ = this->present_->next_frame();
	this->present_->run();
}

inline bool physics_sence::time_to_save()
{
	return abs(current_time() - result_.first[result_.first.size() - 1] - setting_.save_duration) < setting_.delta_time/100.0;
}

inline void physics_sence::update_time_squence(const double time)
{
	result_.first.push_back(time);
}

inline void physics_sence::update_track()
{
	for (auto pair : result_.second)
	{
		pair.second->trace();
	}
}
