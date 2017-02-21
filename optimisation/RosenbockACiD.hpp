#ifndef ROSENBOCKACID
#define ROSENBOCKACID

#include "ACiD.hpp"

class RosenbockACiD: public ACiD{
	public:
		/*!Constructor*/
		RosenbockACiD(unsigned int const& N, Vector<double> const& xa, Vector<double> const& xb, double const& beta)
			:ACiD(N,xa,xb,beta){}
		/*!Default destructor*/
		~RosenbockACiD()=default;
		/*{Forbidden*/
		RosenbockACiD() = delete;
		RosenbockACiD(RosenbockACiD const&) = delete;
		RosenbockACiD(RosenbockACiD&&) = delete;
		RosenbockACiD& operator=(RosenbockACiD);
		/*}*/

		double function(Vector<double> const& x){
			double a(1.0);
			double b(100.0);
			iter_++;
			return (a-x(0))*(a-x(0))+b*(x(1)-x(0)*x(0))*(x(1)-x(0)*x(0));
		}
	private:
};
#endif
