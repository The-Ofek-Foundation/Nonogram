# include "src/nonogram.hpp"

#include <iostream>

// nonogram.hpp
// enum class State {
// 	UNKNOWN,
// 	FILLED,
// 	EMPTY
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

// 	unsigned solve(unsigned x, unsigned y, unsigned dx, unsigned dy, const std::vector<unsigned>& clues) noexcept;
// };

// std::ostream operator<<(std::ostream& out, const Nonogram& nonogram) noexcept;

int main()
{
	Nonogram nonogram("test.txt");
	nonogram.solve();
	// nonogram.solve(true);
	// nonogram.solve(true);
	// nonogram.solve(false);

	// std::cout << "\n\n\n\n\n";

	// nonogram.solve(true, 11u);

	// nonogram.solve(true);

	std::cout << nonogram << std::endl;

	return 0;
}