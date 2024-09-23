#ifndef COMP6771_WORD_LADDER_H
#define COMP6771_WORD_LADDER_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace word_ladder {
// Given a file path to a newline-separated list of words...
// Loads those words into an unordered set and returns it.
auto read_lexicon(std::string const& path) -> std::unordered_set<std::string>;

// Given a start word and destination word, returns all the shortest possible paths from the
// start word to the destination, where each word in an individual path is a valid word per the
// provided lexicon.
// Preconditions:
// - from.size() == to.size()
// - lexicon.contains(from)
// - lexicon.contains(to)
auto generate(
    std::string const& from,
    std::string const& to,
    std::unordered_set<std::string> const& lexicon) -> std::vector<std::vector<std::string>>;


class graph {
public:
	/**
	 * @brief Constructs a graph object with starting and word lexicon.
	 *
	 * @param from The starting word.
	 * @param lexicon The words list to find path.
	 */
	graph(std::string const& from, std::unordered_set<std::string> const& lexicon);

	// Default destructor
	~graph() = default;

	/**
	 * @brief Finds all sortest path from the starting word to the ending word.
	 *
	 * @param to The ending word to find the path
	 *
	 * @return A vector of vectors, where each inner vector represents a
	 *         shortest path from the starting word to the ending word.
	 */
	auto path_search(std::string const& to) -> std::vector<std::vector<std::string>> const&;

private:
	// All sortest path from starting word to ending word
	std::vector<std::vector<std::string>> ladders_;

	// Adjacency list representing the graph
	std::unordered_map<std::string, std::unordered_set<std::string>> adjacency_list_;

	// Starting word
	std::string from_;

	// Save the matching lexicon
	std::unordered_set<std::string> lexicon_;

	/**
	 * @brief Filters the lexicon to include only words that are the same length
	 * 		  as the starting word.
	 *
	 * @param from The starting word.
	 * @param lexicon The set of words to be filtered.
	 * @return A filtered set of words that are the same length as the starting word.
	 */
	static auto lexicon_filter (
		std::string const& from,
		std::unordered_set<std::string> const& lexicon
	) -> std::unordered_set<std::string>;

	/**
	 * @brief Adds edges to the graph based on the given lexicon.
	 *        Each word in the lexicon becomes a vertex, and edges are added
	 *        between words that differ by exactly one character.
	 */
	auto add_edges() -> void;

	/**
	 * @brief Checks if two words differ by exactly one character.
	 *
	 * @param word_1 The first word.
	 * @param word_2 The second word.
	 * @return True if the words differ by exactly one character, false otherwise.
	 */
	static auto is_one_char_diff(
		std::string const& word_1,
		std::string const& word_2
	) -> bool;

	/**
	 * @brief Performs a breadth-first search to find the depth of
	 *		  each word from the starting word.
	 *
	 * @param to The ending word.
	 * @param depth A hash table to store the depth of each word from the starting word.
	 * @return True if the ending word is found, false otherwise.
	 */
	auto bfs_find_depth(
		std::string const& to,
		std::unordered_map<std::string, std::size_t>& depth
	) const -> bool;


	/**
	 * @brief Use the depth hash table and adjacent list to recursive find all the
	 *		  path from starting word to ending word.
	 *
	 * @param from The starting word.
	 * @param to The ending word.
	 * @param depth A hash table off all words depth from the starting word which can visited.
	 * @param current Temporary current path.
	 */
	auto dfs_find_path(
		std::string const& from,
		std::string const& to,
		std::unordered_map<std::string, std::size_t> const& depth,
		std::vector<std::string>& current
	) -> void;
};
} // namespace word_ladder

#endif // COMP6771_WORD_LADDER_H
