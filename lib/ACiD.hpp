#ifndef ACID
#define ACID

#include "Lapack.hpp"
#include "Rand.hpp"

class ACiD{
	public:
		/*!Constructor*/
		ACiD(unsigned int const& N, double const& beta);
		/*!Constructor that reads from file*/
		ACiD(IOFiles& in);
		/*!Default destructor*/
		virtual ~ACiD();
		/*{Forbidden*/
		ACiD() = delete;
		ACiD(ACiD const&) = delete;
		ACiD(ACiD&&) = delete;
		ACiD& operator=(ACiD const&);
		/*}*/

		void run(unsigned int const& maxsteps);
		virtual double function(Vector<double> const& x) = 0;
		virtual bool keepon(bool const& improve_overall) const { return improve_overall; }

		void save(IOFiles& out) const;

	protected:
		unsigned int const N_;
		double const k_suc_;
		double const k_uns_;
		double c1_;
		double cp_;
		double cmu_;
		double bf_;				//!< function(x_min_)
		Vector<double> x_min_;	//!< best location found by ACiD
		Vector<double> p_;
		Vector<double> w_;
		Vector<double> xmean_;
		Vector<double> sigma_;
		Vector<double>* d_;
		Matrix<double> B_;
		Matrix<double> invB_;
		Matrix<double> C_;
		
		void ACD_update(Matrix<double> const& pop);
};
#endif
