#include "FuncPSO.hpp"

FuncPSO::FuncPSO(unsigned int const& Nparticles, unsigned int const& maxiter, bool const& random):
	Swarm<ParticleOnGrid>(Nparticles,maxiter,2,2.1,2.1),
	out_("evolution.dat",true,false),
	random_(random),
	rnd_(-0.1,0.1)
{
	for(unsigned int i(0);i<2;i++){ Particle::set_limit(i,-4,4); }
}

double FuncPSO::f(Vector<double> const& x){
	return (-20*exp(-0.2*sqrt(0.5*x.norm_squared())) - exp(0.5*(cos(2*M_PI*x(0))+cos(2*M_PI*x(1)))) + exp(1) + 20);
}

bool FuncPSO::evaluate(unsigned int const& p){
	std::shared_ptr<ParticleOnGrid> particle(std::dynamic_pointer_cast<ParticleOnGrid>(particle_[p]));
	std::shared_ptr<Measure> sim(std::make_shared<Measure>(particle->get_x(),(random_?rnd_():0.0) + f(particle->get_x())));
#pragma omp critical(List__global)
	{
		List<Measure>::Node* sample(NULL);
		if(!m_.find_in_sorted_list(sim,sample,Measure::sort)){
			m_.set_target(sample);
			m_.add_after_target(sim);
			m_.set_target();
		} else {
			sample->get()->merge(sim);
			sim = sample->get();
		}
	}

	bool updated(particle->update(sim));
#pragma omp critical(Write)
	{
		out_<<p<<" "<<particle->get_bx()<<" "<<particle->get_fbx()<<" "<<sim.get()->get_N()<<IOFiles::endl;
	}

	return updated;
}

void FuncPSO::result(){
	std::cout<<"best for each particle"<<std::endl;
	print();

	//for(unsigned int i(0);i<Nparticles_;i++){
		//std::cout<<"history particle "<<i<<std::endl;
		//particle_[i]->print();
	//}
}
