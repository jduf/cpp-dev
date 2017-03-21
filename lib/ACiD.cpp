#include "ACiD.hpp"

ACiD::ACiD(unsigned int const& N, double const& beta):
	N_(N),
	k_suc_(1.5),
	k_uns_(0.5),
	p_(N_),
	w_(N_),
	xmean_(N_),
	sigma_(N_),
	d_(new Vector<double>[N_+1]),
	B_(N_,N_),
	C_(N_,N_,0.0)
{
	double lnmu(0);
	for(unsigned int d(1);d<N_+1;d++){ lnmu += log(d); }
	lnmu = N_*log(N_+1) - lnmu;

	//double tmp(0.0);
	RandGaussian rnd;
	Vector<double> v(N_);
	for(unsigned int i(0);i<N_;i++){
		d_[i].set(N_);
		p_(i) = 0.0;
		C_(i,i) = 1.0;
		w_(i) = (log(N_+1)-log(i+1))/lnmu;

		//for(unsigned int i(0);i<N_;i++){ v(i) = rnd(); }
		//for(unsigned int i(0);i<N_;i++){
			//for(unsigned int j(0);j<i;j++){
				//tmp = 0.0;
				//for(unsigned int k(0);k<N_;k++){ tmp += v(k)*B_(k,j); }
				//for(unsigned int k(0);k<N_;k++){ v(k) -= tmp*B_(k,j); }
			//}
		//}
		//tmp = v.norm();
		//for(unsigned int j(0);j<N_;j++){ B_(j,d) = v(j)/tmp; }
		for(unsigned int j(0);j<N_;j++){ B_(i,j) = rnd(); }
	}
	//invB_ = B_.transpose();
	invB_=B_;
	Lapack<double>(invB_,false,'G').inv();
	
	double muw(1.0/w_.norm_squared());
	if(muw<1){
		std::cerr<<"weight problem : "<<muw<<std::endl;
	}

	cmu_ = beta*(muw - 2.0 + 1.0/muw)/((N_+2)*(N_+2)+0.2*muw);
	c1_ = beta/((N+1.3)*(N+1.3)+muw);
	cp_ = 1.0/sqrt(N_);
	while( c1_+cmu_ > 1){
		c1_ *= 0.9;
		cmu_ *= 0.9;
		std::cerr<<"Warning : c1 and cmu have been changed : "<<c1_<<" "<<cmu_<<std::endl;
	}
}

ACiD::ACiD(IOFiles& in):
	N_(in.read<unsigned int>()),
	k_suc_(in.read<double>()),
	k_uns_(in.read<double>()),
	c1_(in.read<double>()),
	cp_(in.read<double>()),
	cmu_(in.read<double>()),
	bf_(in.read<double>()),
	x_min_(in),
	p_(in),
	w_(in),
	xmean_(in),
	sigma_(in),
	B_(in),
	invB_(B_),
	C_(in)
{ Lapack<double>(invB_,false,'G').inv(); }

ACiD::~ACiD(){
	delete[] d_;
	d_ = NULL;
}

void ACiD::run(unsigned int const& maxiter){
	double bfold(bf_);
	double f1(0.0);
	double f2(0.0);
	Vector<double> x1(N_);
	Vector<double> x2(N_);

	Vector<double> allf(2*N_);
	Matrix<double> allx(N_,2*N_);
	Matrix<double> pop(N_,N_);

	unsigned int direction(0);
	unsigned int iter(0);
	bool improved(true);
	bool improved_overall(true);
	while(iter++<maxiter && keepon(improved_overall)){
		for(unsigned int i(0);i<N_;i++){
			x1(i) = x_min_(i) + sigma_(direction)*B_(i,direction);
			x2(i) = x_min_(i) - sigma_(direction)*B_(i,direction);
		}

		f1 = function(x1);
		f2 = function(x2);

		bool optimized(false);
		if(f1 < bf_){
			bf_ = f1;
			x_min_ = x1;
			optimized = true;
		}
		if(f2 < bf_){
			bf_ = f2;
			x_min_ = x2;
			optimized = true;
		}
		if(optimized){
			sigma_(direction) *= k_suc_;
			improved = true;
		} else {
			sigma_(direction) *= k_uns_;
		}

		unsigned int d1(2*direction);
		unsigned int d2(2*direction+1);
		allf(d1) = f1;
		allf(d2) = f2;
		for(unsigned int i(0);i<N_;i++){
			allx(i,d1) = x1(i);
			allx(i,d2) = x2(i);
		}

		direction++;
		if(direction == N_){
			direction = 0;
			if(improved){
				improved = false;
				Vector<unsigned int> index;
				allf.sort(std::less_equal<double>(),index);
				for(unsigned int i(0);i<N_;i++){
					for(unsigned int j(0);j<N_;j++){
						pop(i,j) = allx(i,index(j));
					}
				}
				ACD_update(pop);

				improved_overall = !my::are_equal(bfold,bf_,1e-7,1e-7);
				bfold = bf_;
			}
		}
	}
}

void ACiD::save(IOFiles& out) const {
	out<<N_ <<k_suc_ <<k_uns_ <<c1_ <<cp_ <<cmu_;
	out.write("f(x_min)",bf_);
	out.write("x_min",x_min_);
	out<<p_ <<w_ <<xmean_ <<sigma_ <<B_ <<C_;
}

void ACiD::ACD_update(Matrix<double> const& pop){
	/*in this algorithm mu=N because for each direction d in [1,N] two
	 * measurments are made*/
	Vector<double> l(N_);
	for(unsigned int i(0);i<N_;i++){
		for(unsigned int j(0);j<N_;j++){
			d_[i](j) = pop(j,i) - xmean_(j);
		}
		l(i) = sqrt((invB_*d_[i]).norm_squared());
	}
	d_[N_] = pop*w_-xmean_;
	xmean_+= d_[N_];

	double norm_squared((invB_*d_[N_]).norm_squared());
	double lmedian(l.median());
	double alpha(sqrt(N_/norm_squared));

	p_ *= (1.0 - cp_);
	p_ += d_[N_]*alpha*sqrt(cp_*(2.0-cp_));

	Matrix<double> Cmu(N_,N_,0.0);
	for(unsigned int i(0);i<N_;i++){
		alpha = sqrt(N_)/std::max(l(i)/2.0,lmedian);
		Cmu += d_[i]^d_[i]*w_(i)*alpha*alpha;
	}

	if(norm_squared<1e-10){
		std::cerr<<__PRETTY_FUNCTION__<<" : risk of division by 0 : "<<norm_squared<<std::endl;
	} else {
		C_ *= (1.0 - c1_ - cmu_);
		C_ += p_^p_*c1_;
		C_ += Cmu*cmu_;

		Matrix<double> Bo(C_);
		Lapack<double> diag(Bo,false,'S');
		Vector<double> D;
		diag.eigensystem(D,true);

		for(unsigned int i(0);i<N_;i++){ D(i) = sqrt(D(i)); }
		for(unsigned int i(0);i<N_;i++){
			for(unsigned int j(0);j<N_;j++){
				B_(i,j) = Bo(i,j)*D(j);
				invB_(i,j) = Bo(i,j)/D(i);
			}
		}
	}
}
