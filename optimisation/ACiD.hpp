#ifndef ACID
#define ACID

#include "Lapack.hpp"
#include "Rand.hpp"

class ACiD{
	public:
		/*!Constructor for the creation of a .gp file in path/filename*/
		ACiD(unsigned int const& N, Vector<double> const& xa, Vector<double> const& xb);
		/*!Default destructor*/
		~ACiD() = default;
		/*{Forbidden*/
		ACiD() = delete;
		ACiD(ACiD const&) = delete;
		ACiD(ACiD&&) = delete;
		ACiD& operator=(ACiD);
		/*}*/
		void run(unsigned int const& maxiter, double& bf);


		double function(Vector<double> const& x){
			double a(1.0);
			double b(100.0);
			iter_++;
			return (a-x(0))*(a-x(0))+b*(x(1)-x(0)*x(0))*(x(1)-x(0)*x(0));
			//return 5*x(0)*x(0)-6*x(0)*x(1)+5*x(1)*x(1);
		}

	private:
		unsigned int iter_;
		unsigned int const N_;
		double const c1_;
		double const cc_;
		double const k_suc_;
		double const k_uns_;
		Vector<double> pc_;
		Vector<double> weights_;
		Vector<double> pop_mean_;
		Vector<double> sigma_;
		Vector<double> D_;
		Matrix<double> C_;
		Matrix<double> Cold_;
		Matrix<double> B_;
		Matrix<double> Bo_;
		Matrix<double> invB_;
		Matrix<double> pop_;
		Matrix<double> allx_;
		Vector<double> allf_;

		void ACD_update();
};
#endif
