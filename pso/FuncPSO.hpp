#ifndef DEF_FUNCPSO
#define DEF_FUNCPSO

#include "Gnuplot.hpp"
#include "ParticleOnGrid.hpp"

class FuncPSO : public Swarm<ParticleOnGrid> {
	public:
		FuncPSO(unsigned int const& Nparticles, unsigned int const& maxiter, bool const& random);
		~FuncPSO() = default;
		void result();
		List<Measure>* get_m() { return &m_; }

		double f(Vector<double> const& x);

	protected:
		List<Measure> m_;

	private:
		IOFiles out_;
		bool evaluate(unsigned int const& p); 
		bool random_;
		Rand<double> rnd_;
};
#endif
