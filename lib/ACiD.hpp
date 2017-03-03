#ifndef ACID
#define ACID

#include "Lapack.hpp"
#include "Rand.hpp"

class ACiD{
	public:
		/*!Constructor*/
		ACiD(unsigned int const& N, Vector<double> const& xa, Vector<double> const& xb, double const& beta);
		/*!Constructor that reads from file*/
		ACiD(IOFiles& in);
		/*!Default destructor*/
		virtual ~ACiD() = default;
		/*{Forbidden*/
		ACiD() = delete;
		ACiD(ACiD const&) = delete;
		ACiD(ACiD&&) = delete;
		ACiD& operator=(ACiD const&);
		/*}*/

		void run(unsigned int const& maxiter);
		virtual double function(Vector<double> const& x) = 0;
		unsigned int iter_ = 0;

		void save(IOFiles& out) const;

	protected:
		unsigned int const N_;
		double const k_suc_;
		double const k_uns_;
		double c1_;
		double cp_;
		double cmu_;
		double bf_;
		Vector<double> x_;
		Vector<double> p_;
		Vector<double> w_;
		Vector<double> xmean_;
		Vector<double> xmeanold_;
		Vector<double> sigma_;
		Matrix<double> B_;
		Matrix<double> invB_;
		Matrix<double> C_;
		
		void ACD_update(Matrix<double> const& pop);
};
#endif
