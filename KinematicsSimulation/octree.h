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
		constexpr static collide_result create_empty_result()
		{
			return std::make_shared<std::list<std::pair<E, E>>>();
		}

		//utils
		constexpr static void add_to_result(collide_result re, E a, E b, const bool outer)
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
		static collide_result merge_result(collide_result a, collide_result b)
		{
			a->merge(*b);
			return a;
		}

		void warp(E obj);

		constexpr static vec3D calc_position(pn super, location subs);
		constexpr static vec3D calc_size(pn super, location subs);
	};

public:
	// template <typename Ele>
	// class octree_iter
	// {
	// public:
	// 	Ele operator*(const octree_iter&);
	//
	// 	bool operator==(const octree_iter& lhs, const octree_iter& rhs) = delete;
	//
	// 	bool operator!=(const octree_iter& lhs, const octree_iter& rhs)
	// 	{
	// 		return !(lhs == rhs);
	// 	}
	//
	// 	octree_iter operator++(int);
	//
	// 	octree_iter operator--(int);
	// };
	//
	// octree_iter<E> begin();
	//
	// octree_iter<E> end();

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
	//@Zhangxuanheng
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
		auto tag = false;
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
	//@kirakira666
}

//utils
#define SIZE(e) ((e)->size())
#define POS(e) (e)->position()
#define BOXC(a,b) (a)->is_box_collide(*(b))
#define all_in(a,b) \
(SIZE(a).x() - SIZE(obj).x() >= 2.0 * abs(POS(obj).x() - POS(a).x())\
&& SIZE(a).y() - SIZE(obj).y() >= 2.0 * abs(POS(obj).y() - POS(a).y())\
&& SIZE(a).z() - SIZE(obj).z() >= 2.0 * abs(POS(obj).z() - POS(a).z()))

template <typename E>
constexpr vec3D octree<E>::node::calc_position(pn super, location subs)
{
	//@YujiangZhou TODO
}

template <typename E>
constexpr vec3D octree<E>::node::calc_size(pn super, location subs)
{
	//@YujiangZhou TODO
}

template <typename E>
typename octree<E>::location octree<E>::node::test_range(E obj)
{
	if (all_in(this, obj))
	{
		for (auto i = 0; i < 8; ++i)
			if (all_in(subs_[i], obj))
				return location(i);
		return AXIS;
	}
	return OVER;
}

template <typename E>
typename octree<E>::collide_result octree<E>::node::forward_test_collide() const
{
	//@kirakira666 TODO
}

template <typename E>
typename octree<E>::collide_result octree<E>::node::self_test_collide() const
{
	//@kirakira666 TODO
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
	//TODO calc warp
}
