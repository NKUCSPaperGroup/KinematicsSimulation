#include "physics_sence.h"
#include <future>

void physics_sence::add_field(const field& field)
{
	this->fields_.push_back(std::make_shared<decltype(field)>(field));
}

void physics_sence::add_mass_center(const masscenter& o, const bool b)
{
	const auto p = std::make_shared<masscenter>(o);
	this->objs_.push_back(p);
	this->treed_objs_.add(p);
	if (b)
	{
		auto ps = std::make_shared<tracker<masscenter, masscenter_save>>();
		ps->bind(*p);
		ps->trace();
		this->result_.second.insert({p->name(), ps});
	}
}

void physics_sence::remove(const field& f)
{
	this->fields_.remove_if([=](const std::shared_ptr<field> ef)-> bool { return (*ef) == f; });
}

void physics_sence::remove(const std::string& name)
{
	const auto p = std::find_if(objs_.begin(), objs_.end(), [=](const pm e)-> bool { return e->name() == name; });
	this->treed_objs_.remove(*p);
	if (p != objs_.end())
	{
		this->objs_.erase(p);
	}
}

void physics_sence::run()
{
	this->run_inti();
	update_time_squence(current_time());
	while (this->current_time() <= this->setting_.end_time)
	{
		this->next_frame();
		if (this->time_to_save())
		{
			update_time_squence(current_time());
			update_track();
		}
	}
}

physics_sence::result_map physics_sence::get_result_map() const
{
	return result_;
}

std::pair<physics_sence::time_squence, physics_sence::result> physics_sence::get_result(const std::string& name) const
{
	return {result_.first, result_.second.at(name)};
}

void physics_sence::frame::run()
{
	auto task1 = std::async([this]()-> void { this->update_extra_force(); });
	auto task2 = std::async([this]()-> void { this->update_internal_force(); });
	auto task3 = std::async([this]()-> void { this->update_collide_force(); });
	task1.get();
	task2.get();
	task3.get();
	this->update_objs();
}

physics_sence::frame::frame(physics_sence& scene)
	: time_(scene.current_time()), objs_(scene.objs_), scene_(scene)
{
}

physics_sence::frame::frame(const frame& f)
	: time_(f.time_), objs_(f.objs_), scene_(f.scene_)
{
}

physics_sence::pf physics_sence::frame::next_frame()
{
	return build_next_frame(this->scene_.setting().delta_time);
}

void physics_sence::frame::update_extra_force()
{
	for (auto& pf : this->scene_.fields_)
	{
		for (auto& pm : this->objs_)
		{
			if (pf->is_in_field(this->time_, *pm))
			{
				pf->add_force(this->time_, *pm);
			}
		}
	}
}

void physics_sence::frame::update_internal_force()
{
	for (auto& pm1 : this->objs_)
	{
		for (auto& pm2 : this->objs_)
		{
			if (*pm1 != *pm2)
			{
				pm1->add_electrostatic_force_to(*pm2, pm1->q());
				pm1->add_gravity_to(*pm2, pm1->mass());
			}
		}
	}
}

void physics_sence::frame::update_collide_force()
{
	//this is hard
}

void physics_sence::frame::update_objs()
{
	for (const auto& p : this->objs_)
	{
		p->move(scene_.setting_.delta_time);
	}
}

physics_sence::pf physics_sence::frame::build_next_frame(const double dt)
{
	auto p = std::make_shared<frame>(*this);
	p->time_ = this->time_ + dt;
	return p;
}
