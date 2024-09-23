#include "word_ladder.h"

#include <catch2/catch.hpp>
#include <fstream>

TEST_CASE("at -> it") {
	auto const& lexicon = std::unordered_set<std::string>{
		"at",
		"it"
	};

	auto const& expected = std::vector<std::vector<std::string>>{
		{"at", "it"}
	};

	auto const& ladders = word_ladder::generate("at", "it", lexicon);

	CHECK(ladders == expected);
}

TEST_CASE("Basic test cases for word ladder") {
	auto const& lexicon = std::unordered_set<std::string>{"cat", "bat", "bet", "bot", "bog", "dog"};

	auto const& expected = std::vector<std::vector<std::string>>{
		{"cat", "bat", "bot", "bog", "dog"}
	};

	auto const& ladders = word_ladder::generate("cat", "dog", lexicon);

	CHECK(ladders.size() == 1);
	REQUIRE(ladders == expected);
}

TEST_CASE("word_ladder::generate") {
    auto const& lexicon = std::unordered_set<std::string>{
        "hit", "hot", "dot", "dog", "cog", "log", "lot", "hog"
    };

    SECTION("simple case with one possible path") {
        auto constexpr& from = "hit";
        auto constexpr& to = "cog";
        auto const& result = word_ladder::generate(from, to, lexicon);

        auto const& expected = std::vector<std::vector<std::string>>{
            {"hit", "hot", "hog", "cog"}
        };

        REQUIRE(result == expected);
    }

    SECTION("simple case with multiple possible paths") {
        auto constexpr& from = "hit";
        auto constexpr& to = "log";
        auto const& result = word_ladder::generate(from, to, lexicon);

        auto const& expected = std::vector<std::vector<std::string>>{
            {"hit", "hot", "hog", "log"},
            {"hit", "hot", "lot", "log"}
        };

        REQUIRE(result == expected);
    }

    SECTION("case with no possible path (disconnected lexicon)") {
        auto const& disconnected_lexicon = std::unordered_set<std::string>{
            "hit", "hot", "dog", "cog", "log"
        };
        auto constexpr& from = "hit";
        auto constexpr& to = "cog";
        auto const& result = word_ladder::generate(from, to, disconnected_lexicon);

        auto const& expected = std::vector<std::vector<std::string>>{};

        REQUIRE(result == expected);
    }

    SECTION("shortest path with multiple paths of same length") {
        auto const& lexicon_with_multiple_paths = std::unordered_set<std::string>{
            "hit", "hot", "dot", "dog", "cog", "log", "lot", "hog", "cot"
        };
        auto constexpr& from = "hit";
        auto constexpr& to = "cog";
        auto const& result = word_ladder::generate(from, to, lexicon_with_multiple_paths);

        auto const& expected = std::vector<std::vector<std::string>>{
            {"hit", "hot", "cot", "cog"},
            {"hit", "hot", "hog", "cog"},
        };

        REQUIRE(result == expected);
    }

    SECTION("no possible path when lexicon does not include end word") {
        auto const& incomplete_lexicon = std::unordered_set<std::string>{
            "hit", "hot", "dot", "dog", "log", "lot", "hog"
        }; // "cog" is missing
        auto constexpr& from = "hit";
        auto constexpr& to = "cog";
        auto const& result = word_ladder::generate(from, to, incomplete_lexicon);

        auto constexpr expected = std::vector<std::vector<std::string>>{};

        REQUIRE(result == expected);
    }
}

TEST_CASE("Test for english.txt") {
    auto const& english_words = ::word_ladder::read_lexicon("./english.txt");

    SECTION("aa -> ba") {
        auto const& ladders = word_ladder::generate("aa", "ba", english_words);

        auto const& expected = std::vector<std::vector<std::string>>{
                {"aa", "ba"}
        };
        CHECK(not ladders.empty());
        REQUIRE(ladders == expected);
    }

    SECTION("awake -> sleep") {
        auto const& expected = std::vector<std::vector<std::string>>{
            {"awake","aware","sware","share","sharn","shawn","shewn","sheen","sheep","sleep"},
            {"awake","aware","sware","share","shire","shirr","shier","sheer","sheep","sleep"}
        };

        auto const& ladders = ::word_ladder::generate("awake", "sleep", english_words);

        CHECK(not ladders.empty());
        REQUIRE(ladders == expected);
    }

	SECTION("airplane -> tricycle, no path") {
    	auto constexpr expected = std::vector<std::vector<std::string>>{};
    	auto const& ladders = ::word_ladder::generate("airplane", "tricycle", english_words);

    	CHECK(ladders.empty());
    	REQUIRE(ladders == expected);
    }

	SECTION("work -> play") {
		auto const& expected = std::vector<std::vector<std::string>>{
			{"work", "fork", "form", "foam", "flam", "flay", "play"},
			{"work", "pork", "perk", "peak", "pean", "plan", "play"},
			{"work", "pork", "perk", "peak", "peat", "plat", "play"},
			{"work", "pork", "perk", "pert", "peat", "plat", "play"},
			{"work", "pork", "porn", "pirn", "pian", "plan", "play"},
			{"work", "pork", "port", "pert", "peat", "plat", "play"},
			{"work", "word", "wood", "pood", "plod", "ploy", "play"},
			{"work", "worm", "form", "foam", "flam", "flay", "play"},
			{"work", "worn", "porn", "pirn", "pian", "plan", "play"},
			{"work", "wort", "bort", "boat", "blat", "plat", "play"},
			{"work", "wort", "port", "pert", "peat", "plat", "play"},
			{"work", "wort", "wert", "pert", "peat", "plat", "play"}
		};

    	auto const& ladders = word_ladder::generate("work", "play", english_words);

    	CHECK_FALSE(ladders.empty());
    	REQUIRE(ladders == expected);
	}
}
