// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_PLANS
#include "../tools.hpp"

namespace test_task_list {

////////////////////////////////////////////////////////////////////////////////

static unsigned s_constructed = 0;
static unsigned s_destructed  = 0;
static unsigned s_copied      = 0;
static unsigned s_moved       = 0;

//------------------------------------------------------------------------------

struct Tracked final {
	explicit Tracked(const unsigned value_) noexcept
		: value{value_}
	{
		++s_constructed;
	}

	Tracked(const Tracked& other) noexcept
		: value{other.value}
	{
		++s_constructed;
		++s_copied;
	}

	Tracked(Tracked&& other) noexcept
		: value{other.value}
	{
		other.value = 0;
		++s_constructed;
		++s_moved;
	}

	~Tracked() noexcept {
		++s_destructed;
	}

	unsigned value = 0;
};

//------------------------------------------------------------------------------

using TaskList  = hfsm2::detail::TaskListT<Tracked, 5, 3>;
using EmptyList = hfsm2::detail::TaskListT<Tracked, 0, 2>;

static constexpr hfsm2::RegionID REGION_0 = 0;
static constexpr hfsm2::RegionID REGION_1 = 1;

//------------------------------------------------------------------------------

static void resetCounters() noexcept {
	s_constructed = 0;
	s_destructed  = 0;
	s_copied      = 0;
	s_moved       = 0;
}

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Shared.TaskListT<Tracked,5,3>.InsertRemoveAndClearRegion") {
	resetCounters();

	TaskList tasks;

	CHECK(tasks.empty());
	CHECK(tasks.count() == 0);
	CHECK(!tasks.bounds(REGION_0));
	CHECK(!tasks.bounds(REGION_1));

	const TaskList::Index a = tasks.emplace(REGION_0, 10u);
	const TaskList::Index b = tasks.emplace(REGION_0, 20u);
	const TaskList::Index c = tasks.emplace(REGION_0, 30u);
	const TaskList::Index d = tasks.emplace(REGION_1, 40u);

	CHECK(a == 0);
	CHECK(b == 1);
	CHECK(c == 2);
	CHECK(d == 3);
	CHECK(tasks.count() == 4);
	CHECK(!tasks.empty());
	CHECK(s_constructed == 4);
	CHECK(s_destructed  == 0);

	CHECK(tasks.bounds(REGION_0).first == a);
	CHECK(tasks.bounds(REGION_0).last  == c);
	CHECK(tasks.next(a) == b);
	CHECK(tasks.prev(b) == a);
	CHECK(tasks.next(b) == c);
	CHECK(tasks.prev(c) == b);
	CHECK(tasks.next(c) == TaskList::invalid());

	CHECK(tasks.bounds(REGION_1).first == d);
	CHECK(tasks.bounds(REGION_1).last  == d);
	CHECK(tasks.prev(d) == TaskList::invalid());
	CHECK(tasks.next(d) == TaskList::invalid());

	tasks.remove(REGION_0, b);

	CHECK(tasks.count() == 3);
	CHECK(!tasks.occupied(b));
	CHECK(s_destructed == 1);
	CHECK(tasks.bounds(REGION_0).first == a);
	CHECK(tasks.bounds(REGION_0).last  == c);
	CHECK(tasks.next(a) == c);
	CHECK(tasks.prev(c) == a);

	const TaskList::Index e = tasks.emplace(REGION_1, 50u);

	CHECK(e == b);
	CHECK(tasks.count() == 4);
	CHECK(tasks[e].value == 50u);
	CHECK(tasks.bounds(REGION_1).first == d);
	CHECK(tasks.bounds(REGION_1).last  == e);
	CHECK(tasks.next(d) == e);
	CHECK(tasks.prev(e) == d);

	tasks.clearRegion(REGION_0);

	CHECK(tasks.count() == 2);
	CHECK(!tasks.bounds(REGION_0));
	CHECK(tasks.bounds(REGION_1).first == d);
	CHECK(tasks.bounds(REGION_1).last  == e);
	CHECK(s_destructed == 3);

	tasks.clear();

	CHECK(tasks.empty());
	CHECK(tasks.count() == 0);
	CHECK(!tasks.bounds(REGION_0));
	CHECK(!tasks.bounds(REGION_1));
	CHECK(s_destructed == s_constructed);
}

//------------------------------------------------------------------------------

TEST_CASE("Shared.TaskListT<Tracked,5,3>.CopyMoveAndCapacity") {
	resetCounters();

	TaskList source;
	const TaskList::Index a = source.emplace(REGION_0, 10u);
	const TaskList::Index b = source.emplace(REGION_1, 20u);

	TaskList copy{source};

	CHECK(copy.count() == 2);
	CHECK(copy.bounds(REGION_0).first == a);
	CHECK(copy.bounds(REGION_1).first == b);
	CHECK(copy[a].value == 10u);
	CHECK(copy[b].value == 20u);
	CHECK(s_copied == 2);

	TaskList moved{hfsm2::move(copy)};

	CHECK(copy.empty());
	CHECK(moved.count() == 2);
	CHECK(moved.bounds(REGION_0).first == a);
	CHECK(moved.bounds(REGION_1).first == b);
	CHECK(moved[a].value == 10u);
	CHECK(moved[b].value == 20u);
	CHECK(s_moved == 2);

	CHECK(moved.emplace(REGION_0, 30u) != TaskList::invalid());
	CHECK(moved.emplace(REGION_0, 40u) != TaskList::invalid());
	CHECK(moved.emplace(REGION_0, 50u) != TaskList::invalid());
	CHECK(moved.emplace(REGION_0, 60u) == TaskList::invalid());

	source.clear();
	copy  .clear();
	moved .clear();

	CHECK(s_destructed == s_constructed);
}

//------------------------------------------------------------------------------

TEST_CASE("Shared.TaskListT<Tracked,5,3>.SelfAssignment") {
	resetCounters();

	TaskList tasks;
	const TaskList::Index a = tasks.emplace(REGION_0, 10u);
	const TaskList::Index b = tasks.emplace(REGION_1, 20u);

	// Self-assign through a reference so the compiler doesn't flag the
	// direct `tasks = tasks` form. We're specifically checking the runtime
	// `if (this == &other) return *this;` guards in operator=.
	TaskList& selfRef = tasks;

	tasks = selfRef;
	CHECK(tasks.count() == 2);
	CHECK(tasks[a].value == 10u);
	CHECK(tasks[b].value == 20u);

	tasks = hfsm2::move(selfRef);
	CHECK(tasks.count() == 2);
	CHECK(tasks[a].value == 10u);
	CHECK(tasks[b].value == 20u);

	tasks.clear();
	CHECK(s_destructed == s_constructed);
}

////////////////////////////////////////////////////////////////////////////////

}
