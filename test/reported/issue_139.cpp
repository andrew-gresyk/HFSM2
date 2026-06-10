// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

// Issue reported in https://github.com/andrew-gresyk/HFSM2/issues/139

#if __cplusplus >= 201402L

#include <memory>
#include <hfsm2/machine.hpp>

namespace issue_134 {

////////////////////////////////////////////////////////////////////////////////

using M = hfsm2::MachineT<hfsm2::Config>;
using FSM = M::PeerRoot<struct A, struct B>;

struct A : FSM::State {};
struct B : FSM::State {};

void main() {
	auto p = std::make_unique<FSM::Instance>();
	p = std::make_unique<FSM::Instance>(); // error: call to 'swap' is ambiguous
}

////////////////////////////////////////////////////////////////////////////////

}

#endif
