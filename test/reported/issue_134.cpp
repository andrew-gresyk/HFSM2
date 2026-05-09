// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

// Issue reported in https://github.com/andrew-gresyk/HFSM2/issues/134

#define HFSM2_ENABLE_PLANS
#include "../tools.hpp"

#include <string>
#include <vector>
#include <cstdio>
#include <cstring>

using namespace test_tools;

namespace issue_134 {

////////////////////////////////////////////////////////////////////////////////

struct Context {};

struct Payload
{
	std::string label;              // stack-use-after-return
	std::vector<int> data;          // memory leak
};

using Config = hfsm2::Config::ContextT<Context>
							::PayloadT<Payload>
							::TaskCapacityN<256>;

using M = hfsm2::MachineT<Config>;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::Root<S(Root),
				S(StateA),
				S(StateB)
			>;

#undef S

////////////////////////////////////////////////////////////////////////////////

struct Root
	: FSM::State
{};

//------------------------------------------------------------------------------

struct StateA
	: FSM::State
{
	void enter(PlanControl& control) {
		Payload p;
		p.label = "";
		p.data  = {1, 2, 3, 4, 5};
		control.plan().changeWith<StateA, StateB>(p);
	}

	void update(FullControl& control) {
		control.succeed();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct StateB
	: FSM::State
{
	void enter(PlanControl& control) {
		const auto& transitions = control.currentTransitions();
		const Payload* p = transitions[0].payload();
		std::string copy = p->label;
		std::printf("[StateB] data.size() = %zu, data[0] = %d\n",
					p->data.size(),
					p->data.empty() ? -1 : p->data[0]);
	}
};

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("FSM.Reported.Issue_134") {
	Context ctx;
	FSM::Instance fsm{ctx};

	fsm.update();
	fsm.update();
}

////////////////////////////////////////////////////////////////////////////////

}
