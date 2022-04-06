#ifndef NONOGRAM_HPP
#define NONOGRAM_HPP

#include <functional>
#include <iostream>
#include <string>
#include <vector>

enum class State {
	UNKNOWN,
	FILLED,
	EMPTY
};

struct FilledEmptyPair
{
	bool always_filled = true;
	bool always_empty = true;
};

struct SolveReturn
{
	unsigned best_index = -1u;
	unsigned num_solutions;
};

typedef std::function<State(unsigned)> GetState;

class Nonogram
{
public:
	Nonogram(const std::string& filename);

	void solve() noexcept;
	void print(std::ostream& out) const noexcept;

	// should not really be public, used for testing
	SolveReturn solve(bool do_row) noexcept;
	unsigned solve(bool do_row, unsigned index) noexcept;

private:
	std::vector<std::vector<State>> _board;
	std::vector<std::vector<unsigned>> _row_clues;
	std::vector<std::vector<unsigned>> _col_clues;
	unsigned remaining_squares;

	
	unsigned solve(const GetState& get_state, const std::vector<unsigned>& clues, std::vector<FilledEmptyPair>& aisle_pairs, unsigned aisle_length, unsigned index = 0u, unsigned clue_on = 0u, unsigned num_consecutive = 0u) noexcept;
};

std::ostream& operator<<(std::ostream& out, const Nonogram& nonogram) noexcept;

#endif
