#include "test_shared.hpp"

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

TEST_CASE("ObjectPool<> test", "[lib]") {
	ObjectPool objectPool;
	static constexpr auto ITEM_COUNT = ObjectPool::ITEM_COUNT;

	REQUIRE(objectPool.emplace<Material>(1) == 0);
	REQUIRE(objectPool[0].value() == 1);

	REQUIRE(objectPool.emplace<Material>(2) == 1);
	REQUIRE(objectPool[1].value() == 2);

	REQUIRE(objectPool.emplace<Material>(4) == 2);
	REQUIRE(objectPool[2].value() == 4);

	REQUIRE(objectPool.emplace<Material>(8) == 3);
	REQUIRE(objectPool[3].value() == 8);
}

////////////////////////////////////////////////////////////////////////////////
