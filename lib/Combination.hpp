#ifndef DEF_COMBINATION
#define DEF_COMBINATION

#include "Vector.hpp"

/*!see : http://compprog.wordpress.com/2007/10/17/generating-combinations-1/ */
class Combination{
	public:
		Combination(unsigned int const& n, unsigned int const& k);

		/*!Generates the next combination of n elements as k after comb*/
		bool next();
		/*!Returns the total number of combination*/
		unsigned int number() const;
		/*!Returns current combination*/
		Vector<unsigned int> const& get_combination() const { return comb_; }

	private:
		unsigned int const k_;//!size of the subsets to generate
		unsigned int const n_;//!size of the original set
		Vector<unsigned int> comb_; //!previous combination ( use (0, 1, 2, ..., k) for first)
};
#endif
