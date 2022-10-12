#include <catch2/catch.hpp>

#include <DwarvenCore/ObjectPool.h>

struct TestStruct {
	int Value;
	int _PADDING1;
	int _PADDING2;
	int _PADDING3;
};

TEST_CASE("ObjectPool") {

	df::ObjectPool<TestStruct> pool;

	auto val1 = pool.Create();
	val1->Value = 1;

	auto val2 = pool.Create();
	val2->Value = 2;

	auto val3 = pool.Create();
	val3->Value = 3;

	auto val4 = pool.Create();
	val4->Value = 4;

	REQUIRE(pool.Count() == 4);

	{
		int idx = 0;
		const int testValues[] = { 4, 3, 2, 1 };
		for (const auto val : pool) {
			REQUIRE(val->Value == testValues[idx++]);
		}
	}

	pool.Destroy(val2);

	{
		int idx = 0;
		const int testValues[] = { 4, 3, 1 };
		for (const auto val : pool) {
			REQUIRE(val->Value == testValues[idx++]);
		}
	}

	pool.Destroy(val1);

	{
		int idx = 0;
		const int testValues[] = { 4, 3 };
		for (const auto val : pool) {
			REQUIRE(val->Value == testValues[idx++]);
		}
	}

	auto val5 = pool.Create();
	val5->Value = 5;

	{
		int idx = 0;
		const int testValues[] = { 5, 4, 3 };
		for (const auto val : pool) {
			REQUIRE(val->Value == testValues[idx++]);
		}
	}

	REQUIRE(val3->Value == 3);
	pool.Destroy(val3);

	{
		int idx = 0;
		const int testValues[] = { 5, 4 };
		for (const auto val : pool) {
			REQUIRE(val->Value == testValues[idx++]);
		}
	}

	REQUIRE(val4->Value == 4);
	pool.Destroy(val4);

	{
		int idx = 0;
		const int testValues[] = { 5 };
		for (const auto val : pool) {
			REQUIRE(val->Value == testValues[idx++]);
		}
	}

	REQUIRE(val5->Value == 5);
	pool.Destroy(val5);

	{
		int idx = 0;
		for ([[maybe_unused]] const auto val : pool) {
			idx++;
		}
		REQUIRE(idx == 0);
	}

	REQUIRE(pool.Count() == 0);


	val1 = pool.Create();
	val1->Value = 1;

	val2 = pool.Create();
	val2->Value = 2;

	val3 = pool.Create();
	val3->Value = 3;

	val4 = pool.Create();
	val4->Value = 4;

	REQUIRE(pool.Count() == 4);

	{
		int idx = 0;
		const int testValues[] = { 4, 3, 2, 1 };
		for (const auto val : pool) {
			REQUIRE(val->Value == testValues[idx++]);
		}
	}

	pool.Clear();
	REQUIRE(pool.Count() == 0);



	for (int i = 0; i < df::ObjectPool<TestStruct>::PageSize * 2; ++i) {
		pool.Create();
	}

	pool.Clear();
	REQUIRE(pool.Count() == 0);
}