#ifndef ROSENBOCKACID
#define ROSENBOCKACID

#include "ACiD.hpp"

class RosenbockACiD: public ACiD{
	public:
		/*!Constructor*/
		RosenbockACiD(unsigned int const& N, Vector<double> const& xa, Vector<double> const& xb, double const& beta)
			: ACiD(N,beta),
			data_("data.dat",true,false) {
				Rand<double> rnd(0.0,1.0);
				for(unsigned int d(0);d<N;d++){
					sigma_(d) = (xb(d) - xa(d))/4.0;
					xmean_(d) = xa(d) + rnd()*(xb(d) - xa(d));
				}
				x_min_ = xmean_;
				bf_ = function(x_min_);
			}
		/*!Default destructor*/
		~RosenbockACiD()=default;
		/*{Forbidden*/
		RosenbockACiD() = delete;
		RosenbockACiD(RosenbockACiD const&) = delete;
		RosenbockACiD(RosenbockACiD&&) = delete;
		RosenbockACiD& operator=(RosenbockACiD);
		/*}*/

		double function(Vector<double> const& x){
			data_<<x<<" "<<iter_<<" "<<xmean_<<IOFiles::endl;
			double a(1.0);
			double b(100.0);
			iter_++;
			return (a-x(0))*(a-x(0))+b*(x(1)-x(0)*x(0))*(x(1)-x(0)*x(0));
		}

		unsigned int iter_ = 0;

	private:
		IOFiles data_;
};
#endif
