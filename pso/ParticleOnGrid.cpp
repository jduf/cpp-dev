#include "ParticleOnGrid.hpp"

unsigned int Measure::sort(Measure const& a, Measure const& b) {
	for(unsigned int i(0);i<a.x_.size();i++){
		if(a.x_(i) - b.x_(i) > 1e-10 ){ return 0; }
		if(a.x_(i) - b.x_(i) <-1e-10 ){ return 1; }
	}
	return 2;
}

void Measure::merge(std::shared_ptr<Measure> const& new_elem){
	//std::cout<<"merge "<<x_<<" "<<new_elem->get_x()<<std::endl;
	N_++;
	s_ += new_elem.get()->get_fx();
	ss_+= new_elem.get()->get_fx()*new_elem.get()->get_fx();
}

std::ostream& operator<<(std::ostream& flux, Measure const& m){
	m.print(flux);
	return flux;
}

void ParticleOnGrid::move(Vector<double> const& bx_all){
	Particle::move(bx_all);
	if(dx_>0.0){
		for(unsigned int j(0);j<dof_;j++){
			if(std::abs(x_(j))<dx_/2){ x_(j) = 0; }
			else if(std::abs(x_(j)-min_(j))<dx_/2){ x_(j) = min_(j); }
			else if(std::abs(x_(j)-max_(j))<dx_/2){ x_(j) = max_(j); }
			else { x_(j) = std::round(x_(j)/dx_)*dx_; }
		}
	}
	if(v_.norm_squared()<dx_*dx_){ init_Particle(100); }
}

bool ParticleOnGrid::update(std::shared_ptr<Measure> const& new_elem){
	if(!history_.find_in_sorted_list(new_elem,Measure::sort)){
		history_.add_after_target(new_elem);
	}

	fbx_ = 1e6;
	Vector<double> x;
	history_.set_target();
	while(history_.target_next()){
		if(!x.ptr() || history_.get().get_fx()<fbx_){
			x = history_.get().get_x();
			fbx_ = history_.get().get_fx();
		}
	}

	if(x.ptr() && !my::are_equal(x_,x)){
		bx_ = x;
		return true;
	} else { return false; }
}

void ParticleOnGrid::print() const {
	Particle::print();
	//history_.set_target();
	//while(history_.target_next()){
		//std::cout<<history_.get()<<std::endl;
	//}
}
