#pragma once
#include <list>
#include <memory>
#include "vec3D.h"
#include "basebox.h"
#include <algorithm>
#include <future>
#include <utility>

const vec3D inti_size = {256, 256, 256};
const vec3D inti_pos = {0, 0, 0};
const size_t split_size = 10;

//The ptr to obj of basebox
template <typename E>
class octree
{
	class node;
	using pn = std::shared_ptr<node>;
public:
	using collide_result = std::shared_ptr<std::list<std::pair<E, E>>>;
	template <typename Ele>
	friend class octree_iter;

	octree();

protected:

	enum location
	{
		PPP = 0,
		PPN = 1,
		PNP = 2,
		PNN = 3,
		NPP = 4,
		NPN = 5,
		NNP = 6,
		NNN = 7,
		AXIS = 8,
		OVER = 9,
	};

	class node : public basebox
	{
	public:
		friend octree;
		node();

		node(location location, pn super,
		     std::list<E> objs);

		void add(E);
		bool remove(std::string);
		void clear();
		collide_result test_collide() const;

	protected:
		location location_;
		pn super_;
		std::vector<pn> subs_;
		int depth_;
		std::list<E> objs_;
	private:

		location test_range(E);

		void split(pn this_node);

		//utils
		static collide_result create_empty_result();

		//utils
		static void add_to_result(collide_result re, E a, E b, const bool outer);

		collide_result forward_test_collide() const;
		collide_result self_test_collide() const;

		void refresh();

		//utils
		static void merge_result(collide_result to, collide_result from);

		void warp(E obj);

		static vec3D calc_position(pn super, location subs);
		static vec3D calc_size(pn super, location subs);
	};

public:
	void add(E obj);

	bool remove(std::string obj);

	void clear();

	void refresh();

	size_t size() const;

	collide_result test_collide() const;

protected:
	pn root_;
private:
	size_t size_ = 0;
};

template <typename E>
octree<E>::octree(): root_(std::make_shared<node>())
{
	root_->split(root_);
}

template <typename E>
typename octree<E>::collide_result octree<E>::node::create_empty_result()
{
	return std::make_shared<std::list<std::pair<E, E>>>();
}

template <typename E>
void octree<E>::node::add_to_result(collide_result re, E a, E b, const bool outer)
{
	re->push_back(std::make_pair(a, b));
	if (outer)
	{
		re->push_back(std::make_pair(b, a));
	}
}

template <typename E>
void octree<E>::node::merge_result(collide_result to, collide_result from)
{
	to->merge(*from);
}

template <typename E>
void octree<E>::add(E obj)
{
	root_->add(obj);
	size_++;
}

template <typename E>
bool octree<E>::remove(std::string obj)
{
	return root_->remove(obj) ? size_--, true : false;
}

template <typename E>
void octree<E>::clear()
{
	root_->clear();
}

template <typename E>
void octree<E>::refresh()
{
	root_->refresh();
}

template <typename E>
size_t octree<E>::size() const
{
	return size_;
}

template <typename E>
typename octree<E>::collide_result octree<E>::test_collide() const
{
	return root_->test_collide();
}

template <typename E>
octree<E>::node::node()
	: basebox(inti_pos, inti_size)
	  , location_(PPP), super_(nullptr), depth_(0), objs_()
{
}

template <typename E>
octree<E>::node::node(const location location, pn super, std::list<E> objs)
	: basebox(calc_position(super, location), calc_size(super, location))
	  , location_(location), super_(super), depth_(super->depth_ + 1), objs_(std::move(objs))
{
}

template <typename E>
void octree<E>::node::add(E obj)
{
	if (this->objs_.size() > split_size && this->subs_.empty())
	{
		this->split(super_->subs_.at(location_));
	}
	auto lo = test_range(obj);
	switch (lo)
	{
	case OVER:
		if (this->super_ != nullptr)
		{
			this->super_->add(obj);
		}
		else { this->warp(obj); }
		break;
	case AXIS:
		this->objs_.push_back(obj);
		break;
	default:
		this->subs_[lo]->add(obj);
		break;
	}
}

template <typename E>
bool octree<E>::node::remove(const std::string obj)
{
	auto fi = std::find_if(this->objs_.begin(), this->objs_.end(), [=](auto& o)-> bool { return o->name() == obj; });
	if (fi == objs_.end())
	{
		for (pn& p : subs_)
		{
			if (p->remove(obj))
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		objs_.erase(fi);
		return true;
	}
}

template <typename E>
void octree<E>::node::clear()
{
	this->objs_.clear();
	std::for_each(this->subs_.begin(), this->subs_.end(), [=](pn& p)-> void { p->clear(); });
}

template <typename E>
typename octree<E>::collide_result octree<E>::node::test_collide() const
{
	auto re = create_empty_result();
	if (this->super_ == nullptr)
	{
		std::list<std::future<collide_result>> tasks;
		for (const pn& sub : subs_)
		{
			tasks.push_back(std::async([&]()-> collide_result { return sub->test_collide(); }));
		}
		merge_result(re, this->self_test_collide());
		for (std::future<collide_result>& task : tasks)
		{
			merge_result(re, task.get());
		}
		return re;
	}
	else
	{
		for (const pn& p : this->subs_)
		{
			merge_result(re, p->test_collide());
		}
		merge_result(re, this->self_test_collide());
		merge_result(re, this->forward_test_collide());
		return re;
	}
}

//utils
#define SIZE(e) ((e)->size())
#define POS(e) (e)->position()
#define BOXC(a,b) (a)->is_box_collide(*(b))
#define ALL_IN(a,b) \
(SIZE(a).x() - SIZE(obj).x() >= 2.0 * abs(POS(obj).x() - POS(a).x())\
&& SIZE(a).y() - SIZE(obj).y() >= 2.0 * abs(POS(obj).y() - POS(a).y())\
&& SIZE(a).z() - SIZE(obj).z() >= 2.0 * abs(POS(obj).z() - POS(a).z()))

template <typename E>
vec3D octree<E>::node::calc_position(pn super, const location subs)
{
	return vec3D{
		subs & 4
			? super->position().x() - super->size().x() / 4
			: super->position().x() + super->size().x() / 4,
		subs & 2
			? super->position().y() - super->size().y() / 4
			: super->position().y() + super->size().y() / 4,
		subs & 1
			? super->position().z() - super->size().z() / 4
			: super->position().z() + super->size().z() / 4,
	};
}

template <typename E>
vec3D octree<E>::node::calc_size(pn super, location subs)
{
	return vec3D{super->size().x() / 2, super->size().y() / 2, super->size().z() / 2};
}

template <typename E>
typename octree<E>::location octree<E>::node::test_range(E obj)
{
	if (ALL_IN(this, obj))
	{
		for (auto i = 0; i < 8; ++i)
			if (ALL_IN(subs_[i], obj))
				return location(i);
		return AXIS;
	}
	return OVER;
}

template <typename E>
void octree<E>::node::split(pn this_node)
{
	for (int i = 0; i < 8; ++i)
	{
		this->subs_.push_back(std::make_shared<node>(location(i), this_node, std::list<E>{}));
	}
	refresh();
}

template <typename E>
typename octree<E>::collide_result octree<E>::node::forward_test_collide() const
{
	auto empty_result = create_empty_result();
	for (auto np = this->super_; np != nullptr; np = np->super_)
		for (auto& e1 : np->objs_)
			for (auto& e2 : this->objs_)
				if (BOXC(e1, e2))
					add_to_result(empty_result, e1, e2, true);
	return empty_result;
}

template <typename E>
typename octree<E>::collide_result octree<E>::node::self_test_collide() const
{
	auto rep = create_empty_result();
	for (auto& e1 : this->objs_)
	{
		for (auto& e2 : this->objs_)
		{
			if (e1 != e2)
			{
				if (BOXC(e1,e2))
				{
					add_to_result(rep, e1, e2, false);
				}
			}
		}
	}
	return rep;
}

template <typename E>
void octree<E>::node::refresh()
{
	std::for_each(this->subs_.begin(), this->subs_.end(), [=](pn& p)-> void { p->refresh(); });
	std::vector<decltype(this->objs_.begin())> re;
	for (auto i = this->objs_.begin(); i != this->objs_.end(); ++i)
	{
		const location lo = test_range(*i);
		if (lo != AXIS)
		{
			re.push_back(i);
		}
	}
	for (auto re1 : re)
	{
		this->add(*re1);
		this->objs_.erase(re1);
	}
}

template <typename E>
void octree<E>::node::warp(E obj)
{
	//TODO
	this->objs_.push_back(obj);
}
