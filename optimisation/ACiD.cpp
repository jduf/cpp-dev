#include "ACiD.hpp"

ACiD::ACiD(unsigned int const& N, Vector<double> const& xa, Vector<double> const& xb):
	iter_(0),
	N_(N),
	k_suc_(1.8),
	k_uns_(0.5),
	cmu_(0.0),
	muw_(0.0),
	p_(N_,0.0),
	w_(N_),
	m_(N_),
	sigma_(N_),
	D_(N_,1.0),
	C_(N_,N_,0.0),
	Cmu_(N_,N_),
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
	double lnmu(0);
	for(unsigned int d(1);d<N_+1;d++){ lnmu += log(d); }
	lnmu = N_*log(N_+1) - lnmu;

	for(unsigned int d(0);d<N_;d++){
		w_(d) = (log(N_+1)-log(d+1))/lnmu;
		C_(d,d) = 1.0;
		m_(d) = xa(d) + rnd()*(xb(d) - xa(d));
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

	//std::cout<<B_<<std::endl;
	//std::cout<<std::endl;
	//std::cout<<invB_<<std::endl;
	//std::cout<<std::endl;
	//std::cout<<(invB_*B_).chop()<<std::endl;
	
	muw_ = 1./w_.norm_squared();
	cmu_ = 0.2*(muw_ - 2.0 + 1.0/muw_)/((N_+2)*(N_+2)+0.2*muw_);
	c1_ = 0.2/((N+1.3)*(N+1.3)+muw_);
	cp_ = 1.0/sqrt(N_);

	while( c1_+cmu_ > 1){
		c1_ *= 0.9;
		cmu_ *= 0.9;
		std::cerr<<"Warning : c1 and cmu have been changed : "<<c1_<<" "<<cmu_<<std::endl;
	}

	if(muw_<1){ 
		std::cerr<<"weight problem : "<<1./w_.norm_squared()<<std::endl;
	}
}

void ACiD::run(unsigned int const& maxiter, double& bf){
	unsigned int iter(0);
	bool improved_overall(true);
	bool improved(true);
	unsigned int d(0);
	double f1(0.0);
	double f2(0.0);
	Vector<double> x(m_);
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
		data<<x<<" "<<m_<<IOFiles::endl;
		data<<x1<<" "<<m_<<IOFiles::endl;
		data<<x<<" "<<m_<<IOFiles::endl;
		data<<x2<<" "<<m_<<IOFiles::endl;

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
	/*in this algorithm mu=N because for each direction d in [1,N] two
	 * measurments are made*/
	Vector<double>* d_(new Vector<double>[N_+1]);
	Vector<double> alpha_(N_+1);
	Vector<double> l_(N_);

	mold_ = m_;
	m_ = pop_*w_;
	d_[N_] = m_-mold_;
	double norm_squared((invB_*d_[N_]).norm_squared());
	if(norm_squared<1e-10){
		std::cout<<"warning 1 : risk of division by 0 : "<<norm_squared<<std::endl;
	}

	for(unsigned int i(0);i<N_;i++){
		d_[i].set(N_,0.0);
		for(unsigned int j(0);j<N_;j++){
			d_[i](j) = pop_(i,j) - mold_(j);
		}
		l_(i) = sqrt((invB_*d_[i]).norm_squared());
	}
	double l(l_.median());
	for(unsigned int i(0);i<N_;i++){
		alpha_(i) = sqrt(N_)*std::max(l_(i)/2.0,l);
	}
	alpha_(N_) = sqrt(N_/norm_squared);

	p_ *= (1.0 - cp_);
	p_ += d_[N_]*alpha_(N_)*sqrt(cp_*(2.0-cp_)); 

	Cmu_.set(N_,N_,0.0);
	for(unsigned int i(0);i<N_;i++){
		Cmu_ += d_[i]^d_[i]*w_(i)*alpha_(i)*alpha_(i);
	}

	C_ *= (1.0 - c1_ - cmu_);
	C_ += p_^p_*c1_;
	C_ += Cmu_ * cmu_;

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

	//std::cout<<"----"<<std::endl;
	//std::cout<<"pop"<<std::endl;
	//std::cout<<pop_<<std::endl;
	//std::cout<<"d"<<std::endl;
	//std::cout<<d<<std::endl;
	//std::cout<<"z"<<std::endl;
	//std::cout<<z<<std::endl;
	//std::cout<<"B"<<std::endl;
	//std::cout<<B_<<std::endl;
	//std::cout<<"invB"<<std::endl;
	//std::cout<<invB_<<std::endl;
	//std::cout<<"xmean"<<std::endl;
	//std::cout<<xmean_<<std::endl;
	//std::cout<<"pc"<<std::endl;
	//std::cout<<p_<<std::endl;
	//std::cout<<"C"<<std::endl;
	//std::cout<<C_<<std::endl;
	//std::cout<<"----"<<std::endl;
}
