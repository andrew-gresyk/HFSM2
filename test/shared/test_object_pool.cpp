#include "../shared.hpp"

////////////////////////////////////////////////////////////////////////////////

struct Base {
	virtual int value() const = 0;
};

struct Material
	: Base
{
	Material(const int i)
		: _value{i}
	{}

	int value() const override {
		return _value;
	}

	const int _value;
};

using ObjectPool = hfsm2::detail::ObjectPool<Base, 4, sizeof(Material) * 4>;

//------------------------------------------------------------------------------

TEST_CASE("ObjectPool<>", "[shared]") {
	ObjectPool objectPool;

	REQUIRE(objectPool.emplace<Material>(1) == 0); //-V521
	REQUIRE(objectPool[0].value() == 1); //-V521

	REQUIRE(objectPool.emplace<Material>(2) == 1); //-V521
	REQUIRE(objectPool[1].value() == 2); //-V521

	REQUIRE(objectPool.emplace<Material>(4) == 2); //-V521
	REQUIRE(objectPool[2].value() == 4); //-V521

	REQUIRE(objectPool.emplace<Material>(8) == 3); //-V521
	REQUIRE(objectPool[3].value() == 8); //-V521
}

////////////////////////////////////////////////////////////////////////////////
