#define CATCH_CONFIG_RUNNER

#include "test_shared.hpp"

//------------------------------------------------------------------------------

void deprecated();

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
	deprecated();

	return Catch::Session().run(argc, argv);
}

////////////////////////////////////////////////////////////////////////////////
