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
	using result = std::shared_ptr<tracker<masscenter, masscenter::save_type>>;
	using time_squence = std::vector<double>;
	using result_map = std::pair<time_squence, std::map<std::string, result>>;
public:

	template <typename Field>
	void add_field(const Field& f)
	{
		this->fields_.push_back(std::make_shared<Field>(f));
	}

	void add_mass_center(const masscenter&, bool = false);

	void remove(const masscenter& o) { return remove(o.name()); }

	void remove(const field& f)
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

	void remove(const std::string&);

	struct run_setting
	{
		double start_time = 0;
		double end_time = 1;
		double delta_time = 1e-4;
		double reaction_time = 3e-4;
		double save_duration = 20;
	};

	run_setting& setting()
	{
		return setting_;
	}

	void run();

	double current_time() const
	{
		return this->present_->get_time();
	}

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
		double get_time() const { return time_; }
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
		};

		void colliding_attach() const;

		void update_collide_force() const;

		void remove_overtime_colliding() const;

		void add_new_colliding() const;

		bool isnot_this_colliding_added(const std::pair<pm,pm>&) const;

		void build_new_colliding(const std::pair<pm, pm>)const;

		pf build_next_frame(double);

		double time_;

		std::list<pm> objs_;

		std::shared_ptr<std::map<std::string, collide_reaction>> colliding_;

		physics_sence& scene_;
	};

	void run_inti()
	{
		this->inti_ = std::make_shared<frame>(*this);
		this->present_ = std::make_shared<frame>(*inti_);
		update_time_squence(setting_.start_time);
	}

	void next_frame()
	{
		this->present_ = this->present_->next_frame();
		this->present_->run();
	}

	bool time_to_save()
	{
		return current_time() - result_.first[result_.first.size() - 1] >= setting_.save_duration;
	}

	void update_time_squence(const double time)
	{
		result_.first.push_back(time);
	}

	void update_track()
	{
		for (auto pair : result_.second)
		{
			pair.second->trace();
		}
	}

	run_setting setting_;

	pf inti_;
	pf present_;

	std::list<std::shared_ptr<field>> fields_;
	std::list<pm> objs_;
	octree<pm> treed_objs_;

	result_map result_;
};
