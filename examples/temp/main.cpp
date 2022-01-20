#define HFSM2_ENABLE_UTILITY_THEORY
#include <iostream>
#include <hfsm2/machine.hpp>

struct Context {
	uint8_t cycleCount = 0;
};

using M = hfsm2::MachineT<hfsm2::Config::ContextT<Context>>;

#define S(s) struct s
using FSM = M::PeerRoot<
	M::Utilitarian<S(Red),
		S(Yellow),
		S(Green)
	>,
	S(Off)
>;
#undef S

struct On
	: FSM::State
{
	void enter(Control& control) {
		control.context().cycleCount = 0;
		std::cout << "On" << std::endl;
	}
};

struct Red
	: FSM::State
{
	void enter(Control& control) {
		++control.context().cycleCount;
		std::cout << "  Red" << std::endl;
	}

	void update(FullControl& control) {
		if (control.context().cycleCount > 3)
			control.changeTo<Off>();
		else
			control.changeTo<Green>();
	}
};

struct Yellow
	: FSM::State
{
	void enter(Control&) { std::cout << "    Yellow ^" << std::endl; }
	void update(FullControl& control) { control.changeTo<Red>(); }
	///
	Utility utility(const Control& control) { return 0.5f + control.context().cycleCount; }
	///
};

struct Green
	: FSM::State
{
	void enter(Control&) { std::cout << "      Green" << std::endl; }
	void update(FullControl& control) { control.changeTo<Yellow>(); }
};

struct Off
	: FSM::State
{
	void enter(Control&) { std::cout << "Off" << std::endl; }
};

int
main() {
	Context context;
	FSM::Instance machine{ context };

	while (machine.isActive<Off>() == false)
		machine.update();

	return 0;
}
