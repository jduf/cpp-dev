#include "ACiD.hpp"

ACiD::ACiD(unsigned int const& N, Vector<double> const& xa, Vector<double> const& xb):
	iter_(0),
	N_(N),
	c1_(0.5/N_),
	cc_(1.0/sqrt(N_)),
	k_suc_(1.8),
	k_uns_(0.5),
	pc_(N_,0.0),
	weights_(N_,1.0/N_),
	pop_mean_(N_),
	sigma_(N_),
	D_(N_,1.0),
	C_(N_,N_,0.0),
	Cold_(N_,N_,0.0),
	B_(N_,N_),
	Bo_(N_,N_),
	invB_(N_,N_),
	pop_(N_,N_),
	allx_(N_,2*N_),
	allf_(2*N_)
{
	Rand<double> rnd(0.0,1.0);
	Vector<double> v(N_);
	double tmp(0.0);
	for(unsigned int d(0);d<N_;d++){
		C_(d,d) = 1.0;
		Cold_(d,d) = 1.0;
		pop_mean_(d) = xa(d) + rnd()*(xb(d) - xa(d));
		sigma_(d) = (xb(d) - xa(d))/4.0;

		for(unsigned int i(0);i<N_;i++){ v(i) = rnd(); }
		for(unsigned int i(0);i<N_;i++){
			for(unsigned int j(0);j<i;j++){
				tmp = 0.0;
				for(unsigned int k(0);k<N_;k++){ tmp += v(k)*B_(k,j); }
				for(unsigned int k(0);k<N_;k++){ v(k) -= tmp*B_(k,j); }
			}
		}
		tmp = v.norm();
		for(unsigned int j(0);j<N_;j++){ B_(j,d) = v(j)/tmp; }
	}
	invB_ = B_.transpose();

	B_.set(N_,N_,0.0);
	B_(0,0) = 1.0;
	B_(1,1) = 1.0;
	invB_ = B_;

	//std::cout<<B_<<std::endl;
	//std::cout<<std::endl;
	//std::cout<<invB_<<std::endl;
	//std::cout<<std::endl;
	//std::cout<<(invB_*B_).chop()<<std::endl;
}

void ACiD::run(unsigned int const& maxiter, double& bf){
	unsigned int iter(0);
	bool improved_overall(true);
	bool improved(true);
	unsigned int d(0);
	double f1(0.0);
	double f2(0.0);
	pop_mean_(0) = -3.1;
	pop_mean_(1) = -4.1;
	Vector<double> x(pop_mean_);
	Vector<double> x1(N_);
	Vector<double> x2(N_);
	double bfold(bf);
	IOFiles data("data.dat",true,false);
	while(iter++<maxiter && improved_overall){
		for(unsigned int i(0);i<N_;i++){
			x1(i) = x(i) + sigma_(d)*B_(i,d);
			x2(i) = x(i) - sigma_(d)*B_(i,d);
		}

		//data<<x<<" "<<x1<<" "<<f1<<" "<<x2<<" "<<std::endl;
		data<<x<<" "<<pop_mean_<<IOFiles::endl;
		data<<x1<<" "<<pop_mean_<<IOFiles::endl;
		data<<x<<" "<<pop_mean_<<IOFiles::endl;
		data<<x2<<" "<<pop_mean_<<IOFiles::endl;

		//x1 = x;
		//x2 = x;
		//x1(d) += sigma_(d);
		//x2(d) -= sigma_(d);

		f1 = function(x1);
		f2 = function(x2);

		bool optimized(false);
		if(f1 < bf){
			bf = f1;
			x = x1;
			optimized = true;
		}
		if(f2 < bf){
			bf = f2;
			x = x2;
			optimized = true;
		}
		if(optimized){
			sigma_(d) *= k_suc_;
			improved = true;
		} else {
			sigma_(d) *= k_uns_;
		}

		unsigned int d1(2*d);
		unsigned int d2(2*d+1);
		allf_(d1) = f1;
		allf_(d2) = f2;
		for(unsigned int i(0);i<N_;i++){
			allx_(i,d1) = x1(i);
			allx_(i,d2) = x2(i);
		}

		d++;
		if(d == N_){
			d = 0;
			if(improved){
				improved = false;
				Vector<unsigned int> index;
				allf_.sort(std::less_equal<double>(),index);
				for(unsigned int i(0);i<N_;i++){
					for(unsigned int j(0);j<N_;j++){
						pop_(i,j) = allx_(i,index(j));
					}
				}
				ACD_update();

				improved_overall = !my::are_equal(bfold,bf,1e-7,1e-7);
				bfold = bf;
			}
		}
	}
	std::cout<<x<<" -> "<<bf<<" : "<<iter_<<std::endl;
}

void ACiD::ACD_update(){
	Vector<double> move(pop_*weights_ - pop_mean_);
	pop_mean_ = pop_*weights_;
	double norm_squared((invB_*move).norm_squared());
	Vector<double> z(move*sqrt(N_/norm_squared));

	pc_ *= (1.0 - cc_);
	if(norm_squared<1e-10){
		std::cout<<"warning 1 : "<<norm_squared<<std::endl;
		std::cout<<"move : "<<move<<std::endl;
		z.set(N_,0);
	} else { pc_ += z*sqrt(cc_*(2.0-cc_)); }

	C_ *= (1.0 - c1_);
	C_ += pc_^pc_*c1_;

	Bo_ = C_;
	Lapack<double> diag(Bo_,false,'S');
	diag.eigensystem(D_,true);

	for(unsigned int i(0);i<N_;i++){ D_(i) = sqrt(D_(i)); }
	for(unsigned int i(0);i<N_;i++){ 
		for(unsigned int j(0);j<N_;j++){ 
			B_(i,j) = Bo_(i,j)*D_(j);
			invB_(i,j) = Bo_(i,j)/D_(i);
		}
	}
	Cold_ = C_;

	//std::cout<<"----"<<std::endl;
	//std::cout<<"pop"<<std::endl;
	//std::cout<<pop_<<std::endl;
	//std::cout<<"move"<<std::endl;
	//std::cout<<move<<std::endl;
	//std::cout<<"z"<<std::endl;
	//std::cout<<z<<std::endl;
	//std::cout<<"B"<<std::endl;
	//std::cout<<B_<<std::endl;
	//std::cout<<"invB"<<std::endl;
	//std::cout<<invB_<<std::endl;
	//std::cout<<"xmean"<<std::endl;
	//std::cout<<xmean_<<std::endl;
	//std::cout<<"pc"<<std::endl;
	//std::cout<<pc_<<std::endl;
	//std::cout<<"C"<<std::endl;
	//std::cout<<C_<<std::endl;
	//std::cout<<"----"<<std::endl;
}
