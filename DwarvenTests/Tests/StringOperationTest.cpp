#include <catch2/catch.hpp>

#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/StringOperations.h>

TEST_CASE("String Operations") {

	SECTION("Find") {
		const df::String str = "Hello, world!";
		const auto idx_h = df::Find<char>(str, 'h');
		const auto idx_l = df::Find<char>(str, 'l');
		const auto idx_space = df::Find<char>(str, ' ');

		REQUIRE(idx_h == -1);
		REQUIRE(idx_l == 2);
		REQUIRE(idx_space == 6);
	}

	SECTION("Symbol count") {
		const df::String str = "Hello, world!";
		const size_t count_h = df::CountOf<char>(str, 'h');
		const size_t count_l = df::CountOf<char>(str, 'l');
		const size_t count_lo = df::CountOf<char>(str, "lo");

		REQUIRE(count_h == 0);
		REQUIRE(count_l == 3);
		REQUIRE(count_lo == 5);
	}

	SECTION("Tokenizer") {
		const df::String str = "C:\\Program Files\\Test\\File.txt";
		const df::StringView tokens[5] = {
			"C:","Program Files", "Test", "File", "txt"
		};

		int idx = 0;
		for (const auto token : df::Tokenize(str, "\\.")) {
			REQUIRE(token == tokens[idx++]);
		}
	}

	SECTION("Simple replace") {
		df::String str = "C:\\Program Files\\Test\\File.txt";
		df::Replace(str, '\\', '/');

		REQUIRE(str == "C:/Program Files/Test/File.txt");
	}

	SECTION("Replace To Symbol") {
		const df::String str = "GlobalUBO[#].Array[*]";
		const df::String str1 = df::Replaced(str, '#', '1');
		const df::String str2 = df::Replaced(str1, '*', '7');

		REQUIRE(str2 == "GlobalUBO[1].Array[7]");
	}

	SECTION("Replace To String") {
		const df::String str = "GlobalUBO#.Array*";
		const df::String str1 = df::Replaced(str, '#', "[1]");
		const df::String str2 = df::Replaced(str1, '*', "[100]");

		REQUIRE(str2 == "GlobalUBO[1].Array[100]");

		const df::String str3 = df::Replaced(str2, "[]", "");

		REQUIRE(str3 == "GlobalUBO1.Array100");

		const df::String str4 = df::Replaced(str, '#', "_");

		REQUIRE(str4 == "GlobalUBO_.Array*");
	}

	SECTION("Take Left") {
		const df::String str = "Test String";
		const df::StringView result = df::TakeLeft(str, 4);

		REQUIRE(result == "Test");
	}

	SECTION("Take Right") {
		const df::String str = "Test String";
		const df::StringView result = df::TakeRight(str, 6);

		REQUIRE(result == "String");
	}

	SECTION("Drop Left") {
		const df::String str = "Test String";
		const df::StringView result = df::DropLeft(str, 5);

		REQUIRE(result == "String");
	}

	SECTION("Drop Right") {
		const df::String str = "Test String";
		const df::StringView result = df::DropRight(str, 7);

		REQUIRE(result == "Test");
	}

	SECTION("Split 1") {
		const df::String str = "Test String";
		df::StringView left;
		df::StringView right;
		df::Split(str, 4, left, right);

		REQUIRE(left == "Test");
		REQUIRE(right == " String");
	}

	SECTION("Split 2") {
		const df::String str = "Test String";
		auto&& [left, right] = df::Split(str, 4);

		REQUIRE(left == "Test");
		REQUIRE(right == " String");
	}

	SECTION("Trim") {
		const df::String str = "+_S_tring__";
		const df::StringView result = df::Trim(str, "_+");

		REQUIRE(result == "S_tring");
	}

	SECTION("Equal Part") {
		const df::String str = "Hello, World!";
		const df::String str2 = "Oh, Hello!";
		const df::StringView substring = "Hello";
		const df::StringView substring2 = "Hello, World!!!";

		REQUIRE(df::EqualPart(str, substring));
		REQUIRE(!df::EqualPart(str, substring2));
		REQUIRE(!df::EqualPart(str2, substring));
	}

	SECTION("To Lower") {
		const df::String str = "Hello, World!";
		const df::String ref = "hello, world!";
		const df::String conv = df::ToLower<char>(str);

		REQUIRE(ref == conv);
	}

	SECTION("To Upper") {
		const df::String str = "Hello, World!";
		const df::String ref = "HELLO, WORLD!";
		const df::String conv = df::ToUpper<char>(str);

		REQUIRE(ref == conv);
	}
}