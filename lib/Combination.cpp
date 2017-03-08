#include "Combination.hpp"

Combination::Combination(unsigned int const& n, unsigned int const& k):
	k_(k),
	n_(n),
	comb_(k_)
{
	for(unsigned int i(0);i<k_;i++){ comb_(i) = i; }
}

bool Combination::next(){
	unsigned int i(k_-1);
	++comb_(i);
	/*the condition here may be problematic. I use the fact that i is an
	 * unsigned int and therefore whe i-1<0 is give a number that is very
	 * likely bigger than k. The original condition was with int i and i>=0*/
	while((i-1 < k_) && (comb_(i) >= n_-k_+1+i)){
		--i;
		++comb_(i);
	}
	/* Combination (n-k, n-k+1, ..., n) reached */
	/* No more combinations can be generated */
	if(comb_(0) > n_-k_){ return false; }
	/* comb now looks like (..., x, n, n, n, ..., n).
	   Turn it into (..., x, x + 1, x + 2, ...) */
	for(unsigned int j(i+1);j<k_;++j){ comb_(j) = comb_(j-1) + 1;}

	return true;
}

unsigned int Combination::number() const{
	unsigned int p(1);
	for(unsigned int i(0);i<n_-k_;i++){ p *= n_-i; }
	for(unsigned int i(1);i<n_-k_+1;i++){ p /= i; }
	return p;
}
