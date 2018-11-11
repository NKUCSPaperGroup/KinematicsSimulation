#pragma once
#include <list>
#include <memory>
#include "vec3D.h"
#include "basebox.h"

//The ptr to obj of basebox
template <typename E>
class octree
{
#define SIZE(e) ((e)->size())
#define POS(e) ((e)->position())
#define BOXC(a,b) ((a)->is_box_collide(*(b)))

	using collide_result = std::list<std::pair<E, E>>;
	class node;
	using pn = std::shared_ptr<node>;

	const static vec3D inti_size{4096, 4096, 4096};
	const static vec3D inti_pos{0, 0, 0};
public:
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
		NNN = 7
	};

	class node : public basebox
	{
	public:

		node(location location, pn super,
		     std::list<E> objs);

		void add(E);
		void remove(E);
		size_t size() const;
		void clear();
		static collide_result merge_result(collide_result&, collide_result&);
		collide_result test_collide() const;

	protected:
		location location_;
		pn super_;
		pn subs_;
		int depth_;
		std::list<E> objs_;

		int test_range(E);
		pn warp(E);
		collide_result forward_test_collide() const;
		collide_result self_test_collide() const;
	};

	class octree_iter
	{
	public:
		E operator*() const;
		bool operator!=(octree_iter);
		void operator++();
	};

	pn root_;
public:
	void add(E);
	bool remove(E);
	void clear(E);
	size_t size() const;
	collide_result test_collide() const;

	octree_iter begin();
	octree_iter end();
private:
	size_t size_ = 0;
};
