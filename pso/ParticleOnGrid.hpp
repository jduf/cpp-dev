#ifndef DEF_PARTICLEONGRID
#define DEF_PARTICLEONGRID

#include "List.hpp"
#include "PSO.hpp"

class Measure{
	public:
		Measure(Vector<double> const& x, double const& fx):x_(x),s_(fx),ss_(0.0),N_(1){}

		void print(std::ostream& flux) const
		{ flux<<x_<<" : "<<get_fx()<<" "<<get_dfx()<<" "<<N_<<" "<<this; }

		static unsigned int sort(Measure const& a, Measure const& b);
		Vector<double> const&  get_x() const { return x_; }
		double get_fx() const { return s_/N_; }
		double get_dfx() const { return (ss_-(s_*s_)/N_)/(N_-1); }
		unsigned int get_N() const { return N_; }
		void merge(std::shared_ptr<Measure> const& new_elem);

	private:
		Vector<double> x_;
		double s_;
		double ss_;
		unsigned int N_;
};

std::ostream& operator<<(std::ostream& flux, Measure const& m);

class ParticleOnGrid: public Particle {
	public:
		ParticleOnGrid() = default;
		~ParticleOnGrid() = default;

		void move(Vector<double> const& bx_all);
		bool update(std::shared_ptr<Measure> const& new_elem);
		void print() const;

	protected:
		double dx_ = 0.01;
		List<Measure> history_;
};
#endif
