#pragma once
#include <list>
#include <memory>
#include "vec3D.h"
#include "basebox.h"
#include <algorithm>
#include <future>

const vec3D inti_size;
const vec3D inti_pos = {0, 0, 0};

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

		//utils
		static collide_result create_empty_result()
		{
			return std::make_shared<std::list<std::pair<E, E>>>();
		}

		//utils
		static void add_to_result(collide_result re, E a, E b, const bool outer)
		{
			re->push_back(std::make_pair(a, b));
			if (outer)
			{
				re->push_back(std::make_pair(b, a));
			}
		}

		collide_result forward_test_collide() const;
		collide_result self_test_collide() const;

		void refresh();

		//utils
		static void merge_result(collide_result to, collide_result from)
		{
			to->merge(*from);
		}

		void warp(E obj);

		static vec3D calc_position(pn super, location subs);
		static vec3D calc_size(pn super, location subs);
	};

public:
	void add(E obj)
	{
		root_->add(obj);
		size_++;
	}

	bool remove(std::string obj)
	{
		return root_->remove(obj) ? size_--, true : false;
	}

	void clear() { root_->clear(); }

	void refresh() { root_->refresh(); }

	size_t size() const { return size_; }

	collide_result test_collide() const { return root_->test_collide(); }

protected:
	pn root_;
private:
	size_t size_ = 0;
};

template <typename E>
octree<E>::node::node()
	: basebox(inti_pos, inti_size)
	  , location_(PPP), super_(nullptr), depth_(0), objs_()
{
}

template <typename E>
octree<E>::node::node(const location location, pn super, std::list<E> objs)
	: basebox(calc_position(super, location), calc_size(super, location))
	  , location_(location), super_(super), depth_(super->depth_ + 1), objs_(objs)
{
}

template <typename E>
void octree<E>::node::add(E obj)
{
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
	default:
		this->subs_[lo]->add(obj);
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
	if (this->subs_.empty())
	{
		auto re = create_empty_result();
		merge_result(re, this->self_test_collide());
		merge_result(re, this->forward_test_collide());
		return re;
	}
	if (this->super_ == nullptr)
	{
		std::list<std::future<collide_result>> tasks;
		for (const pn& sub : subs_)
		{
			tasks.push_back(std::async([&]()-> collide_result { return sub->test_collide(); }));
		}
		auto re = create_empty_result();
		for (std::future<collide_result>& task : tasks)
		{
			merge_result(re, task.get());
		}
		return re;
	}
	else
	{
		auto re = create_empty_result();
		for (const pn& p : this->subs_)
		{
			merge_result(re, p->test_collide());
		}
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
			? super->position().x() - super->size().x() / 2
			: super->position().x() + super->size().x() / 2,
		subs & 2
			? super->position().y() - super->size().y() / 2
			: super->position().y() + super->size().y() / 2,
		subs & 1
			? super->position().z() - super->size().z() / 2
			: super->position().z() + super->size().z() / 2,
	};
}

template <typename E>
vec3D octree<E>::node::calc_size(pn super, location subs)
{
	return vec3D{super->size().x()};
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
