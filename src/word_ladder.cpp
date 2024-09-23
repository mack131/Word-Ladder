#include "word_ladder.h"

#include <fstream>
#include <algorithm>
#include <queue>
#include <ranges>

auto word_ladder::read_lexicon(std::string const& path) -> std::unordered_set<std::string> {
	auto lexicon = std::unordered_set<std::string>{};
	auto file = std::ifstream{path};

	if (not file.is_open()) throw std::runtime_error{"Failed to open file: " + path};

	for (auto word = std::string{}; file >> word;) {
		if (file.bad()) throw std::runtime_error{"Error reading file: " + path};

		lexicon.insert(word);
	}
	if (not file.eof()) throw std::runtime_error{"Error: Did not reach end of file: " + path};

	return lexicon;
}

auto word_ladder::generate(
	std::string const& from,
	std::string const& to,
	std::unordered_set<std::string> const& lexicon
) -> std::vector<std::vector<std::string>> {

	auto new_graph = graph{from, lexicon};
	return new_graph.path_search(to);
}

word_ladder::graph::graph(std::string const& from, std::unordered_set<std::string> const& lexicon)
	: from_ { from }
, lexicon_ { lexicon_filter(from, lexicon) } {
	add_edges();
}

auto word_ladder::graph::path_search(
	std::string const& to
) -> std::vector<std::vector<std::string>> const& {

	auto depth = std::unordered_map<std::string, std::size_t>{{from_, 0}};
	if (not bfs_find_depth(to, depth)) {
		ladders_.clear();
		return ladders_;
	}
	auto path = std::vector<std::string>{};
	dfs_find_path(from_, to, depth, path);

	std::ranges::sort(ladders_);
	return ladders_;
}

auto word_ladder::graph::bfs_find_depth(
	std::string const& to,
	std::unordered_map<std::string, std::size_t>& depth
) const -> bool {
	auto queue = std::queue<std::string>{};
	queue.push(from_);

	while (not queue.empty()) {
		auto const& pop_word = queue.front();
		queue.pop();
		for (auto const& word : adjacency_list_.at(pop_word)) {
			if (not depth.contains(word)) {
				depth[word] = depth[pop_word] + 1;
				queue.push(word);
				if (word == to) return true;
			}
		}
	}
	return false;
}

auto word_ladder::graph::dfs_find_path(
	std::string const& from,
	std::string const& to,
	std::unordered_map<std::string, std::size_t> const& depth,
	std::vector<std::string>& current
) -> void {
	if (not depth.contains(from)) return;

	current.emplace_back(from);
	if (from != to) {
		for (auto const& word: adjacency_list_.at(from)) {
			if (depth.contains(word) and depth.at(from) + 1 == depth.at(word)) {
				dfs_find_path(word, to, depth, current);
			}
		}
	} else {
		ladders_.emplace_back(current);
	}
	current.pop_back();
}

auto word_ladder::graph::lexicon_filter (
	std::string const& from,
	std::unordered_set<std::string> const& lexicon
) -> std::unordered_set<std::string> {
	auto new_lexicon_set = std::unordered_set<std::string>{};
	new_lexicon_set.reserve(lexicon.size());

	for (auto const& word : lexicon) {
		if (word.size() == from.size()) {
			new_lexicon_set.insert(word);
		}
	}

	return new_lexicon_set;
}

auto word_ladder::graph::add_edges() -> void {
	auto all_combo_dict = std::unordered_map<std::string, std::vector<std::string>>{};
	for (auto const& word : lexicon_) {
		for (auto const& i : std::views::iota(static_cast<std::size_t>(0), word.size())) {
			all_combo_dict[word.substr(0, i) + '*' + word.substr(i + 1)].push_back(word);
		}
	}
	std::ranges::for_each(lexicon_, [this](std::string const& word) {
		adjacency_list_.emplace(word, std::unordered_set<std::string>{});
	});

	for (auto const& transformations : std::views::values(all_combo_dict)) {
		for (auto it1 = transformations.begin(); it1 != transformations.end(); ++it1) {
			for (auto it2 = std::next(it1); it2 != transformations.end(); ++it2) {
				adjacency_list_[*it1].emplace(*it2);
				adjacency_list_[*it2].emplace(*it1);
			}
		}
	}
}

auto word_ladder::graph::is_one_char_diff(
	std::string const& word_1,
	std::string const& word_2
) -> bool {
	if (word_1.size() != word_2.size()) return false;

	int diff_count = 0;
	for (auto it_1 = word_1.begin(), it_2 = word_2.begin();
		 it_1 != word_1.end() and it_2 != word_2.end();
		 ++it_1, ++ it_2) {

		if (*it_1 != *it_2 and ++diff_count > 1) return false;
		 }
	return diff_count == 1;
}
