#include "physics_sence.h"
#include <future>

void physics_sence::add_mass_center(const masscenter& o, const bool b)
{
	const auto p = std::make_shared<masscenter>(o);
	this->objs_.push_back(p);
	this->treed_objs_.add(p);
	if (b)
	{
		auto ps = std::make_shared<tracker<masscenter, masscenter::save_type>>();
		ps->bind(*p);
		ps->trace();
		this->result_.second.insert({p->name(), ps});
	}
}

void physics_sence::remove(const std::string& name)
{
	objs_.remove_if([=](const pm e)-> bool { return e->name() == name; });
	this->treed_objs_.remove(name);
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
	for (auto& m : this->objs_)
	{
		m->clear_force();
	}
	auto task1 = std::async([this]()-> void { this->update_extra_force(); });
	auto task2 = std::async([this]()-> void { this->update_internal_force(); });
	auto task3 = std::async([this]()-> void { this->update_collide_force(); });
	task1.get();
	task2.get();
	task3.get();
	this->update_objs();
}

physics_sence::frame::frame(physics_sence& scene)
	: time_(scene.setting().start_time), objs_(scene.objs_), scene_(scene)
{
	colliding_ = std::make_shared<std::map<std::string, collide_reaction>>();
}

physics_sence::frame::frame(const frame& f)
	: time_(f.time_), objs_(f.objs_), colliding_(f.colliding_), scene_(f.scene_)
{
}

physics_sence::pf physics_sence::frame::next_frame()
{
	return build_next_frame(this->scene_.setting().delta_time);
}

void physics_sence::frame::clear_force()
{
	for (auto mc : this->objs_)
	{
		mc->clear_force();
	}
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
				masscenter::add_electrostatic_force_to(*pm2, pm1->q(),pm1->position());
				masscenter::add_gravity_to(*pm2, pm1->mass(),pm1->position());
			}
		}
	}
}

physics_sence::frame::collide_reaction::
collide_reaction(const pm masscenter, const vec3D& fv, const double action_time)
	: masscenter(masscenter), f(std::make_shared<force>(collide, fv)), action_time(action_time)
{
}

void physics_sence::frame::colliding_attach() const
{
	for (auto& p : *colliding_)
	{
		p.second.masscenter->add_force(*p.second.f);
	}
}

void physics_sence::frame::update_collide_force() const
{
	//this is hard
	remove_overtime_colliding();
	add_new_colliding();
	colliding_attach();
}

void physics_sence::frame::remove_overtime_colliding() const
{
	std::vector<decltype(colliding_->begin())> ps;
	for (auto i = colliding_->begin(); i != colliding_->end(); ++i)
	{
		i->second.action_time -= scene_.setting_.delta_time;
		if (i->second.action_time <= 0)
		{
			ps.push_back(i);
		}
	}
	for (const auto& iterator : ps)
	{
		colliding_->erase(iterator);
	}
}

void physics_sence::frame::add_new_colliding() const
{
	const auto res = scene_.treed_objs_.test_collide();
	for (auto& pair : *res)
		if (isnot_this_colliding_added(pair))
			build_new_colliding(pair);
}

bool physics_sence::frame::isnot_this_colliding_added(const std::pair<pm, pm>& p) const
{
	if (colliding_->count(p.first->name()))
	{
		return *(*colliding_)[p.first->name()].masscenter == *p.second;
	}
	return true;
}

vec3D calc_collide_force(const std::pair<std::shared_ptr<masscenter>, std::shared_ptr<masscenter>>& pair, double dt)
{
	const auto e = pair.first->e() * pair.second->e();
	const auto rr = pair.second->position() - pair.first->position();
	const auto v1_r =
		-(rr.unit() * pair.second->velosity())
		* ((1 + e) / 2)
		+ (rr.unit() * pair.first->velosity())
		* (e * pair.second->mass() - pair.first->mass()) / (2 * pair.first->mass());
	return
		rr.unit()
		* (
			(v1_r - rr.unit() * pair.first->velosity())
			* pair.first->mass()
			/ dt
		);
}

void physics_sence::frame::build_new_colliding(const std::pair<pm, pm> pair) const
{
	const auto fv = calc_collide_force(pair, scene_.setting().reaction_time);
	const auto reaction_time = this->scene_.setting().reaction_time;
	this->colliding_->insert({pair.first->name(), collide_reaction{pair.second, fv, reaction_time}});
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
