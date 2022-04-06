#include "nonogram.hpp"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

// nonogram.hpp
// enum class State {
// 	UNKNOWN,
// 	FILLED,
// 	EMPTY
// };

// struct FilledEmptyPair
// {
// 	bool always_filled = true;
// 	bool always_empty = true;
// };

// struct SolveReturn
// {
// 	unsigned best_index = -1u;
// 	unsigned num_solutions;
// };

// class Nonogram
// {
// public:
// 	Nonogram(const std::string& filename);

// 	void solve() noexcept;
// 	void print(std::ostream& out) const noexcept;

// private:
// 	std::vector<std::vector<State>> _board;
// 	std::vector<std::vector<unsigned>> _row_clues;
// 	std::vector<std::vector<unsigned>> _col_clues;
// 	unsigned remaining_squares;

// 	unsigned solve(bool do_row) noexcept;
//	unsigned solve(bool do_row, unsigned index) noexcept;
//	unsigned solve(std::vector<State>& aisle, const std::vector<unsigned>& clues, std::vector<FilledEmptyPair>& aisle_pairs, unsigned index = 0u, unsigned clue_on = 0u, unsigned num_consecutive = 0u) noexcept;
// };

// std::ostream operator<<(std::ostream& out, const Nonogram& nonogram) noexcept;

Nonogram::Nonogram(const std::string& filename)
{
	std::ifstream in(filename);
	if (!in)
	{
		std::cerr << "Could not open file " << filename << std::endl;
		return;
	}

	unsigned rows, cols;
	in >> rows >> cols;

	remaining_squares = rows * cols;

	_board.resize(rows);
	_row_clues.resize(rows);
	_col_clues.resize(cols);

	std::string line;
	std::getline(in, line);

	for (unsigned i = 0u; i < rows; ++i)
	{
		_board[i].resize(cols);
		std::getline(in, line);
		size_t last = 0u, next = 0u;

		while ((next = line.find(' ', last)) != std::string::npos)
		{
			_row_clues[i].push_back(std::stoi(line.substr(last, next - last)));
			last = next + 1;
		}

		if (line.length() > 0)
		{
			_row_clues[i].push_back(std::stoi(line.substr(last)));
		}
	}

	for (unsigned i = 0u; i < cols; ++i)
	{
		std::getline(in, line);
		size_t last = 0u, next = 0u;

		while ((next = line.find(' ', last)) != std::string::npos)
		{
			_col_clues[i].push_back(std::stoi(line.substr(last, next - last)));
			last = next + 1;
		}

		if (line.length() > 0)
		{
			_col_clues[i].push_back(std::stoi(line.substr(last)));
		}
	}
}

void Nonogram::solve() noexcept
{
	while (remaining_squares > 0u)
	{
		unsigned before_remaining = remaining_squares;
		SolveReturn r1 = solve(true);
		SolveReturn r2 = solve(false);

		if (remaining_squares == before_remaining)
		{
			printf("ERROR: Must start guessing!\n");
			break;
		}
	}
}

SolveReturn Nonogram::solve(bool do_row) noexcept
{
	SolveReturn ret;

	for (unsigned index = 0u; index < (do_row ? _board.size() : _board[0].size()); ++index)
	{
		unsigned num_solutions = solve(do_row, index);

		if (ret.best_index == -1u || num_solutions < ret.num_solutions)
		{
			ret.best_index = index;
			ret.num_solutions = num_solutions;
		}
	}

	return ret;
}

unsigned Nonogram::solve(bool do_row, unsigned index) noexcept
{
	// initialize
	unsigned x = do_row ? index : 0u;
	unsigned y = do_row ? 0u : index;
	const std::vector<unsigned>& clues = do_row ? _row_clues[index] : _col_clues[index];

	std::vector<State> aisle(do_row ? _board.size() : _board[0].size());
	std::vector<FilledEmptyPair> aisle_pairs(aisle.size());

	// fill the aisle
	for (unsigned i = 0u; i < aisle.size(); ++i)
	{
		aisle[i] = _board[x + (do_row ? 0u : i)][y + (do_row ? i : 0u)];
	}

	// solve the aisle
	unsigned num_solutions = solve(aisle, clues, aisle_pairs);

	// update the board
	for (unsigned i = 0u; i < aisle.size(); ++i)
	{
		if (aisle[i] != State::UNKNOWN)
		{
			continue;
		}

		if (aisle_pairs[i].always_empty)
		{
			_board[x + (do_row ? 0u : i)][y + (do_row ? i : 0u)] = State::EMPTY;
			--remaining_squares;
		}
		else if (aisle_pairs[i].always_filled)
		{
			_board[x + (do_row ? 0u : i)][y + (do_row ? i : 0u)] = State::FILLED;
			--remaining_squares;
		}
	}

	return num_solutions;
}

unsigned Nonogram::solve(std::vector<State>& aisle, const std::vector<unsigned>& clues, std::vector<FilledEmptyPair>& aisle_pairs, unsigned index, unsigned clue_on, unsigned num_consecutive) noexcept
{
	// base cases
	if (clue_on == clues.size())
	{
		for (unsigned i = index; i < aisle.size(); ++i)
		{
			if (aisle[i] == State::FILLED)
			{
				return 0u;
			}
		}

		for (unsigned i = index; i < aisle.size(); ++i)
		{
			aisle_pairs[i].always_filled = false;
		}

		return 1u;
	}

	if (index == aisle.size())
	{
		return (clue_on == clues.size() - 1u && num_consecutive == clues[clue_on]) ? 1u : 0u;
	}

	// enforce previous knowledge
	if (aisle[index] != State::UNKNOWN)
	{
		if (num_consecutive == clues[clue_on])
		{
			if (aisle[index] == State::FILLED)
			{
				return 0u;
			}
		}
		else if (num_consecutive > 0u)
		{
			if (aisle[index] == State::EMPTY)
			{
				return 0u;
			}
		}
		else if (aisle[index] == State::FILLED)
		{
			return solve(aisle, clues, aisle_pairs, index + 1u, clue_on, 1u);
		}
		else
		{
			return solve(aisle, clues, aisle_pairs, index + 1u, clue_on, 0u);
		}
	}

	// recursive case
	if (num_consecutive == clues[clue_on])
	{
		unsigned num_solutions = solve(aisle, clues, aisle_pairs, index + 1u, clue_on + 1u, 0u);
		if (num_solutions > 0u)
		{
			aisle_pairs[index].always_filled = false;
		}
		return num_solutions;
	}

	if (num_consecutive > 0u)
	{
		unsigned num_solutions = solve(aisle, clues, aisle_pairs, index + 1u, clue_on, num_consecutive + 1u);
		if (num_solutions > 0u)
		{
			aisle_pairs[index].always_empty = false;
		}
		return num_solutions;
	}

	// multiple options
	unsigned num_solutions_empty = solve(aisle, clues, aisle_pairs, index + 1u, clue_on, 0u);
	unsigned num_solutions_filled = solve(aisle, clues, aisle_pairs, index + 1u, clue_on, 1u);

	if (num_solutions_empty > 0u)
	{
		aisle_pairs[index].always_filled = false;
	}
	if (num_solutions_filled > 0u)
	{
		aisle_pairs[index].always_empty = false;
	}
	
	return num_solutions_empty + num_solutions_filled;	
}

void Nonogram::print(std::ostream& out) const noexcept
{
	out << " ";
	for (unsigned i = 0u; i < _board.size(); ++i)
	{
		if (i > 0u && i % 5u == 0u)
		{
			out << " |";
		}

		out << " " << (i % 10);
	}

	out << "\n";

	for (unsigned j = 0u; j < _board[0u].size(); ++j)
	{
		if (j > 0u && j % 5u == 0u)
		{
			out << "  ‒";
			for (unsigned i = 1u; i < _board.size(); ++i)
			{
				if (i > 0u && i % 5u == 0u)
				{
					out << "‒+";
				}
				out << "‒‒";
			}
			out << "\n";
		}

		out << j % 10;
		for (unsigned i = 0u; i < _board.size(); ++i)
		{
			if (i > 0u && i % 5u == 0u)
			{
				out << " |";
			}

			switch (_board[i][j])
			{
			case State::UNKNOWN:
				out << " ?";
				break;
			case State::FILLED:
				out << " #";
				break;
			case State::EMPTY:
				out << " .";
				break;
			}
		}
		out << "\n";
	}
}

std::ostream& operator<<(std::ostream& out, const Nonogram& nonogram) noexcept
{
	nonogram.print(out);
	return out;
}
