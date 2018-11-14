#pragma once
#include <list>
#include <memory>
#include "vec3D.h"
#include "basebox.h"

//The ptr to obj of basebox
template <typename E>
class octree
{
	using collide_result = std::shared_ptr<std::list<std::pair<E, E>>>;
	class node;
	using pn = std::shared_ptr<node>;

	const static vec3D inti_size{4096, 4096, 4096};
	const static vec3D inti_pos{0, 0, 0};
public:
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
		AXIS = 8
	};

	class node : public basebox
	{
	public:

		node();

		node(location location, pn super,
		     std::list<E> objs);

		void add(E);
		bool remove(E);
		void clear();
		collide_result test_collide() const;

	protected:
		location location_;
		pn super_;
		pn subs_;
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

		//utils
		static collide_result merge_result(collide_result a, collide_result b)
		{
			a->merge(*b);
			return a;
		}

		constexpr static vec3D calc_position(pn super, location subs);
		constexpr static vec3D calc_size(pn super, location subs);
	};

public:
	template <typename Ele>
	class octree_iter
	{
	public:
		Ele operator*(const octree_iter&);

		bool operator==(const octree_iter& lhs, const octree_iter& rhs);

		bool operator!=(const octree_iter& lhs, const octree_iter& rhs)
		{
			return !(lhs == rhs);
		}

		octree_iter operator++(int);

		octree_iter operator--(int);
	};

	octree_iter<E> begin();

	octree_iter<E> end();

	void add(E obj)
	{
		root_->add(obj);
		size_++;
	}

	bool remove(E obj)
	{
		return root_->remove(obj) ? size_--, true : false;
	}

	void clear() { root_->clear(); }

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
octree<E>::node::node(location location, pn super, std::list<E> objs)
	: basebox(calc_position(super, location), calc_size(super, location))
	  , location_(location), super_(super), depth_(super->depth_ + 1), objs_(objs)
{
}

template <typename E>
void octree<E>::node::add(E obj)
{
	//@Zhangxuanheng
}

template <typename E>
bool octree<E>::node::remove(E obj)
{
	//@Zhangxuanheng
}

template <typename E>
void octree<E>::node::clear()
{
	//@Zhangxuanheng
}

template <typename E>
typename octree<E>::collide_result octree<E>::node::test_collide() const
{
	//@kirakira666
}

//utils
#define SIZE(e) ((e)->size())
#define POS(e) ((e)->position())
#define BOXC(a,b) ((a)->is_box_collide(*(b)))

template <typename E>
constexpr vec3D octree<E>::node::calc_position(pn super, location subs)
{
	//@YujiangZhou
}

template <typename E>
constexpr vec3D octree<E>::node::calc_size(pn super, location subs)
{
	//@YujiangZhou
}

template <typename E>
typename octree<E>::location octree<E>::node::test_range(E obj)
{
	//@YujiangZhou
}

template <typename E>
typename octree<E>::collide_result octree<E>::node::forward_test_collide() const
{
	//@kirakira666
}

template <typename E>
typename octree<E>::collide_result octree<E>::node::self_test_collide() const
{
	//@kirakira666
}
