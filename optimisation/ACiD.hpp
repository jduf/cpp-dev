#ifndef ACID
#define ACID

#include "Lapack.hpp"
#include "Rand.hpp"

class ACiD{
	public:
		/*!Constructor*/
		ACiD(unsigned int const& N, Vector<double> const& xa, Vector<double> const& xb, double const& beta);
		/*!Default destructor*/
		virtual ~ACiD();
		/*{Forbidden*/
		ACiD() = delete;
		ACiD(ACiD const&) = delete;
		ACiD(ACiD&&) = delete;
		ACiD& operator=(ACiD const&);
		/*}*/
		void run(unsigned int const& maxiter);

		virtual double function(Vector<double> const& x) = 0;
		unsigned int iter_ = 0;

	private:
		unsigned int const N_;
		double const k_suc_;
		double const k_uns_;
		double c1_;
		double cp_;
		double cmu_;
		double muw_;
		Vector<double>* d_;
		Vector<double> l_;
		Vector<double> p_;
		Vector<double> w_;
		Vector<double> D_;
		Vector<double> m_;
		Vector<double> mold_;
		Vector<double> sigma_;
		Vector<double> alpha_;
		Vector<double> allf_;
		Matrix<double> allx_;
		Matrix<double> B_;
		Matrix<double> Bo_;
		Matrix<double> invB_;
		Matrix<double> C_;
		Matrix<double> Cmu_;
		Matrix<double> Cold_;
		Matrix<double> pop_;

		void ACD_update();
};
#endif
