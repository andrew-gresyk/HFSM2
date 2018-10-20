#define HFSM_ENABLE_ASSERT
//#include <hfsm2/machine.hpp>
#include <hfsm2/machine_dev.hpp>

#include <algorithm>
#include <vector>

#include "deprecated_test.hpp"

using namespace deprecated_test;

////////////////////////////////////////////////////////////////////////////////

void deprecated() {
	Context _;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		FSM::Instance machine(_);

		const Status created[] = {
			status<A>(Event::ENTER),
			status<A_1>(Event::ENTER),
		};
		_.assertHistory(created);

		HFSM_ASSERT( machine.isActive<A>());
		HFSM_ASSERT( machine.isActive<A_1>());
		HFSM_ASSERT(!machine.isActive<A_2>());
		HFSM_ASSERT(!machine.isActive<A_2_1>());
		HFSM_ASSERT(!machine.isActive<A_2_2>());
		HFSM_ASSERT(!machine.isActive<B>());
		HFSM_ASSERT(!machine.isActive<B_1>());
		HFSM_ASSERT(!machine.isActive<B_1_1>());
		HFSM_ASSERT(!machine.isActive<B_1_2>());
		HFSM_ASSERT(!machine.isActive<B_2>());
		HFSM_ASSERT(!machine.isActive<B_2_1>());
		HFSM_ASSERT(!machine.isActive<B_2_2>());

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.react(Action{});

		const Status reacted1[] = {
			status<A>(Event::REACT_REQUEST),
			status<A>(Event::REACT),
			status<A_1>(Event::REACT_REQUEST),
		};
		_.assertHistory(reacted1);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		const Status update1[] = {
			status<A>(Event::UPDATE),
			status<A_1>(Event::UPDATE),

			status<A_2>(Event::RESTART),

			status<A_2>(Event::GUARD),
			status<A_2_1>(Event::GUARD),

			status<A_1>(Event::EXIT),

			status<A_2>(Event::ENTER),
			status<A_2_1>(Event::ENTER),
		};
		_.assertHistory(update1);

		HFSM_ASSERT(!machine.isResumable<A>());
		HFSM_ASSERT( machine.isResumable<A_1>());
		HFSM_ASSERT(!machine.isResumable<A_2>());
		HFSM_ASSERT(!machine.isResumable<A_2_1>());
		HFSM_ASSERT(!machine.isResumable<A_2_2>());
		HFSM_ASSERT(!machine.isResumable<B>());
		HFSM_ASSERT(!machine.isResumable<B_1>());
		HFSM_ASSERT(!machine.isResumable<B_1_1>());
		HFSM_ASSERT(!machine.isResumable<B_1_2>());
		HFSM_ASSERT(!machine.isResumable<B_2>());
		HFSM_ASSERT(!machine.isResumable<B_2_1>());
		HFSM_ASSERT(!machine.isResumable<B_2_2>());

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.react(Action{});

		const Status reacted2[] = {
			status<A>(Event::REACT_REQUEST),
			status<A>(Event::REACT),
			status<A_2>(Event::REACT_REQUEST),
			status<A_2_1>(Event::REACT_REQUEST),
			status<A_2_1>(Event::REACT),
		};
		_.assertHistory(reacted2);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		const Status update2[] = {
			status<A>(Event::UPDATE),
			status<A_2>(Event::UPDATE),
			status<B_2_2>(Event::RESTART),

			status<A_2_1>(Event::UPDATE),

			status<B>(Event::GUARD),
			status<B_1>(Event::GUARD),
			status<B_1_1>(Event::GUARD),
			status<B_2>(Event::GUARD),
			status<B_2_2>(Event::GUARD),

			status<A_2_1>(Event::EXIT),
			status<A_2>(Event::EXIT),
			status<A>(Event::EXIT),

			status<B>(Event::ENTER),
			status<B_1>(Event::ENTER),
			status<B_1_1>(Event::ENTER),
			status<B_2>(Event::ENTER),
			status<B_2_2>(Event::ENTER),
		};
		_.assertHistory(update2);

		HFSM_ASSERT(!machine.isActive<A>());
		HFSM_ASSERT(!machine.isActive<A_1>());
		HFSM_ASSERT(!machine.isActive<A_2>());
		HFSM_ASSERT(!machine.isActive<A_2_1>());
		HFSM_ASSERT(!machine.isActive<A_2_2>());
		HFSM_ASSERT( machine.isActive<B>());
		HFSM_ASSERT( machine.isActive<B_1>());
		HFSM_ASSERT( machine.isActive<B_1_1>());
		HFSM_ASSERT(!machine.isActive<B_1_2>());
		HFSM_ASSERT( machine.isActive<B_2>());
		HFSM_ASSERT(!machine.isActive<B_2_1>());
		HFSM_ASSERT( machine.isActive<B_2_2>());

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.react(Action{});

		const Status reacted3[] = {
			status<B>(Event::REACT_REQUEST),
			status<B>(Event::REACT),
			status<B_1>(Event::REACT_REQUEST),
			status<B_1_1>(Event::REACT_REQUEST),
			status<B_2>(Event::REACT_REQUEST),
			status<B_2_2>(Event::REACT_REQUEST),
		};
		_.assertHistory(reacted3);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		const Status update3[] = {
			status<B>(Event::UPDATE),
			status<B_1>(Event::UPDATE),
			status<B_1_1>(Event::UPDATE),
			status<B_2>(Event::UPDATE),
			status<B_2_2>(Event::UPDATE),

			status<A>(Event::RESUME),

			status<A>(Event::GUARD),
			status<A_2>(Event::GUARD),
			status<A_2_1>(Event::GUARD),

			status<B_1_1>(Event::EXIT),
			status<B_1>(Event::EXIT),
			status<B_2_2>(Event::EXIT),
			status<B_2>(Event::EXIT),
			status<B>(Event::EXIT),

			status<A>(Event::ENTER),
			status<A_2>(Event::ENTER),
			status<A_2_1>(Event::ENTER),
		};
		_.assertHistory(update3);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		const Status update4[] = {
			status<A>(Event::UPDATE),
			status<A_2>(Event::UPDATE),
			status<B>(Event::RESUME),
			status<A_2_1>(Event::UPDATE),

			status<B>(Event::GUARD),
			status<B_1>(Event::GUARD),
			status<B_1_1>(Event::GUARD),
			status<B_2>(Event::GUARD),
			status<B_2_2>(Event::GUARD),

			status<A_2_1>(Event::EXIT),
			status<A_2>(Event::EXIT),
			status<A>(Event::EXIT),

			status<B>(Event::ENTER),
			status<B_1>(Event::ENTER),
			status<B_1_1>(Event::ENTER),
			status<B_2>(Event::ENTER),
			status<B_2_2>(Event::ENTER),
		};
		_.assertHistory(update4);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		const Status update5[] = {
			status<B>(Event::UPDATE),
			status<B_1>(Event::UPDATE),
			status<B_1_1>(Event::UPDATE),
			status<B_2>(Event::UPDATE),
			status<B_2_2>(Event::UPDATE),

			status<B>(Event::RESTART),

			status<B_2_1>(Event::GUARD),
			status<B_2_2>(Event::RESUME),
		};
		_.assertHistory(update5);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		const Status update6[] = {
			status<B>(Event::UPDATE),
			status<B_1>(Event::UPDATE),
			status<B_1_1>(Event::UPDATE),
			status<B_2>(Event::UPDATE),
			status<B_2_2>(Event::UPDATE),

			status<A_2_2>(Event::SCHEDULE),
			status<A>(Event::RESUME),

			status<A>(Event::GUARD),
			status<A_2>(Event::GUARD),
			status<A_2_2>(Event::GUARD),

			status<B_1_1>(Event::EXIT),
			status<B_1>(Event::EXIT),
			status<B_2_2>(Event::EXIT),
			status<B_2>(Event::EXIT),
			status<B>(Event::EXIT),

			status<A>(Event::ENTER),
			status<A_2>(Event::ENTER),
			status<A_2_2>(Event::ENTER),
		};
		_.assertHistory(update6);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}

	const Status destroyed[] = {
		status<A_2_2>(Event::EXIT),
		status<A_2>(Event::EXIT),
		status<A>(Event::EXIT),
	};
	_.assertHistory(destroyed);
}

////////////////////////////////////////////////////////////////////////////////
