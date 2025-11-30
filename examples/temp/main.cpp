// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <hfsm2/machine.hpp>

struct Context {};
using Config = hfsm2::Config ::ContextT<Context&>;
using M = hfsm2::MachineT<Config>;

using FSM = M::PeerRoot<
				struct Off,
				M::Composite<struct Work,
					struct On
				>
			>;

struct CloseEvent {};

struct Off : FSM::State {
	void entryGuard(GuardControl& control) {
		printf("/Off entry guard: change to On\n");
		control.changeTo<On>();
	}
	void enter(PlanControl& /*control*/) { printf("  /Off enter\n"); }
	void exitGuard(GuardControl& /*control*/) { printf("\\Off exit guard\n"); }
	void exit(PlanControl& /*control*/) { printf("  \\Off exit\n"); }
};

struct Work : FSM::State {
	void entryGuard(GuardControl& /*control*/) { printf("/Work entry guard\n"); }
	void enter(PlanControl& /*control*/) { printf("  /Work enter\n"); }
	void exitGuard(GuardControl& /*control*/) { printf("\\Work exit guard\n"); }
	void exit(PlanControl& /*control*/) { printf("  \\Work exit\n"); }
};

struct On : FSM::State {
	void entryGuard(GuardControl& /*control*/) { printf("/On entry guard\n"); }
	void enter(PlanControl& /*control*/) { printf("  /On enter\n"); }
	void reenter(PlanControl& /*control*/) { printf("  |On reenter\n"); }
	void exitGuard(GuardControl& /*control*/) { printf("\\On exit guard\n"); }
	void exit(PlanControl& /*control*/) { printf("  \\On exit\n"); }

	void react(const CloseEvent& /*event*/, EventControl& control) {
		printf("    -On react: change to Off\n");
		control.changeTo<Off>();
	}
};

int main() {
	Context context;
	FSM::Instance machine{context};
	printf("init finished\n");

	machine.react(CloseEvent{});
	printf("react finished\n");

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
