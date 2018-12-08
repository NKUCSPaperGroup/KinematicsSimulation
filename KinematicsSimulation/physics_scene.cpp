/**************************************************************
 *  Copyright: Copyright (c) 2018
 *  Created on 2018-12
 *  Author: NKUCSPaperGroup
 *  At: https://github.com/NKUCSPaperGroup
 *  Email: hamiguazzz@qq.com
 **************************************************************/
#include "physics_scene.h"
#include <future>

void physics_scene::add_mass_center(const masscenter& o, const bool b)
{
	const auto p = std::make_shared<masscenter>(o);
	this->objs_.push_back(p);
	this->treed_objs_.add(p);
	if (b)
	{
		auto ps = std::make_shared<tracker<masscenter, masscenter::save_type>>();
		ps->bind(*p);
		ps->trace();
		this->result_.second.insert({ p->name(), ps });
	}
}

void physics_scene::remove(const std::string& name)
{
	objs_.remove_if([=](const pm e)-> bool { return e->name() == name; });
	this->treed_objs_.remove(name);
}

void physics_scene::run()
{
	this->run_inti();
	update_time_sequence(current_time());
	while (this->current_time() <= this->setting_.end_time)
	{
		this->next_frame();
		if (this->time_to_save())
		{
			update_time_sequence(current_time());
			update_track();
		}
	}
}

physics_scene::result_map physics_scene::get_result_map() const
{
	return result_;
}

std::pair<physics_scene::time_sequence, physics_scene::result> physics_scene::get_result(const std::string& name) const
{
	return { result_.first, result_.second.at(name) };
}

void physics_scene::frame::run()
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

physics_scene::frame::frame(physics_scene& scene)
	: time_(scene.setting().start_time), objs_(scene.objs_), scene_(scene)
{
	colliding_ = std::make_shared<std::map<std::string, collide_reaction>>();
}

physics_scene::frame::frame(const frame& f)
	: time_(f.time_), objs_(f.objs_), colliding_(f.colliding_), scene_(f.scene_)
{
}

physics_scene::pf physics_scene::frame::next_frame()
{
	return build_next_frame(this->scene_.setting().delta_time);
}

void physics_scene::frame::clear_force()
{
	for (auto mc : this->objs_)
	{
		mc->clear_force();
	}
}

void physics_scene::frame::update_extra_force()
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

void physics_scene::frame::update_internal_force()
{
	for (auto& pm1 : this->objs_)
	{
		for (auto& pm2 : this->objs_)
		{
			if (*pm1 != *pm2)
			{
				masscenter::add_electrostatic_force_to(*pm2, pm1->q(), pm1->position());
				masscenter::add_gravity_to(*pm2, pm1->mass(), pm1->position());
			}
		}
	}
}

physics_scene::frame::collide_reaction::
collide_reaction(const pm masscenter, const vec3D& fv, const int action_time, const int frozen_times)
	: masscenter(masscenter), f(std::make_shared<force>(collide, fv)), action_times(action_time),
	frozen_times(frozen_times)
{
}

void physics_scene::frame::colliding_attach() const
{
	if (colliding_->empty())return;
	for (auto& p : *colliding_)
	{
		if (p.second.action_times > 0)
		{
			p.second.masscenter->set_velocity
			(
				p.second.masscenter->velocity()
				+ *p.second.f
				* (scene_.setting().delta_time / p.second.masscenter->mass())
			);
			p.second.action_times--;
		}
		p.second.frozen_times--;
	}
}

void physics_scene::frame::update_collide_force() const
{
	remove_overtime_colliding();
	add_new_colliding();
	colliding_attach();
}

void physics_scene::frame::remove_overtime_colliding() const
{
	std::vector<decltype(colliding_->begin())> ps;
	for (auto i = colliding_->begin(); i != colliding_->end(); ++i)
	{
		if (i->second.action_times <= 0 && i->second.frozen_times <= 0)
		{
			ps.push_back(i);
		}
	}
	for (const auto& iterator : ps)
	{
		colliding_->erase(iterator);
	}
}

void physics_scene::frame::add_new_colliding() const
{
	const auto res = scene_.treed_objs_.test_collide();
	if (res->empty())return;
	for (auto& pair : *res)
		if (not_this_colliding_added(pair))
			build_new_colliding(pair);
}

bool physics_scene::frame::not_this_colliding_added(const std::pair<pm, pm>& p) const
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
	const auto rr = (pair.second->position() - pair.first->position()).unit();
	const auto u1 = rr * pair.first->velocity();
	const auto u2 = rr * pair.second->velocity();
	const auto m1 = pair.first->mass();
	const auto m2 = pair.second->mass();
	const auto v1_r = (m1 * u1 + m2 * u2) / (m1 + m2) - m2 * e * (u1 - u2) / (m1 + m2);

	return rr * ((v1_r - u1) * m1 / -dt);
}

void physics_scene::frame::build_new_colliding(const std::pair<pm, pm> pair) const
{
	const auto dd = int(ceil(scene_.setting().reaction_time / scene_.setting().delta_time));
	const auto reaction_time = dd * scene_.setting().delta_time;
	const auto fv = calc_collide_force(pair, reaction_time);
	this->colliding_->insert({
		pair.first->name(), collide_reaction{pair.second, fv, dd, scene_.setting().frozen_times}
		});
}

void physics_scene::frame::update_objs()
{
	for (const auto& p : this->objs_)
	{
		p->move(scene_.setting_.delta_time);
	}
}

physics_scene::pf physics_scene::frame::build_next_frame(const double dt)
{
	auto p = std::make_shared<frame>(*this);
	p->time_ = this->time_ + dt;
	return p;
}