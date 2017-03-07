#include "ACiD.hpp"

ACiD::ACiD(unsigned int const& N, Vector<double> const& xa, Vector<double> const& xb, double const& beta):
	N_(N),
	k_suc_(1.5),
	k_uns_(0.5),
	p_(N_),
	w_(N_),
	xmean_(N_),
	sigma_(N_),
	B_(N_,N_),
	C_(N_,N_,0.0)
{
	RandGaussian rndG;
	Rand<double> rnd(0.0,1.0);

	double lnmu(0);
	for(unsigned int d(1);d<N_+1;d++){ lnmu += log(d); }
	lnmu = N_*log(N_+1) - lnmu;

	//double tmp(0.0);
	Vector<double> v(N_);
	for(unsigned int d(0);d<N_;d++){
		p_(d) = 0.0;
		C_(d,d) = 1.0;
		w_(d) = (log(N_+1)-log(d+1))/lnmu;
		sigma_(d) = (xb(d) - xa(d))/4.0;
		xmean_(d) = xa(d) + rnd()*(xb(d) - xa(d) - sigma_(d));

		//for(unsigned int i(0);i<N_;i++){ v(i) = rndG(); }
		//for(unsigned int i(0);i<N_;i++){
			//for(unsigned int j(0);j<i;j++){
				//tmp = 0.0;
				//for(unsigned int k(0);k<N_;k++){ tmp += v(k)*B_(k,j); }
				//for(unsigned int k(0);k<N_;k++){ v(k) -= tmp*B_(k,j); }
			//}
		//}
		//tmp = v.norm();
		//for(unsigned int j(0);j<N_;j++){ B_(j,d) = v(j)/tmp; }
		for(unsigned int j(0);j<N_;j++){ B_(d,j) = rndG(); }
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
	x_(in),
	p_(in),
	w_(in),
	xmean_(in),
	xmeanold_(in),
	sigma_(in),
	B_(in),
	invB_(B_),
	C_(in)
{ Lapack<double>(invB_,false,'G').inv(); }

void ACiD::run(unsigned int const& maxiter){

	x_ = xmean_;
	bf_ = function(x_);
	double bfold(bf_);
	double f1(0.0);
	double f2(0.0);
	Vector<double> x1(N_);
	Vector<double> x2(N_);

	Vector<double> allf(2*N_);
	Matrix<double> allx(N_,2*N_);
	Matrix<double> pop(N_,N_);

	//IOFiles data("data.dat",true,false);
	unsigned int d(0);
	unsigned int iter(0);
	bool improved(true);
	bool improved_overall(true);
	while(iter++<maxiter && stop(improved_overall)){
		for(unsigned int i(0);i<N_;i++){
			x1(i) = x_(i) + sigma_(d)*B_(i,d);
			x2(i) = x_(i) - sigma_(d)*B_(i,d);
		}

		f1 = function(x1);
		f2 = function(x2);

		//data<<x<<" "<<iter_<<" "<<xmean_<<IOFiles::endl;
		//data<<x1<<" "<<iter_<<" "<<xmean_<<IOFiles::endl;
		//data<<x2<<" "<<iter_<<" "<<xmean_<<IOFiles::endl;

		bool optimized(false);
		if(f1 < bf_){
			bf_ = f1;
			x_ = x1;
			optimized = true;
		}
		if(f2 < bf_){
			bf_ = f2;
			x_ = x2;
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
		allf(d1) = f1;
		allf(d2) = f2;
		for(unsigned int i(0);i<N_;i++){
			allx(i,d1) = x1(i);
			allx(i,d2) = x2(i);
		}

		d++;
		if(d == N_){
			d = 0;
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
	out.write("x_min",x_);
	out<<p_ <<w_ <<xmean_ <<xmeanold_ <<sigma_ <<B_ <<C_;
}

void ACiD::ACD_update(Matrix<double> const& pop){
	/*in this algorithm mu=N because for each direction d in [1,N] two
	 * measurments are made*/
	xmeanold_ = xmean_;
	xmean_ = pop*w_;

	Matrix<double> Cmu(N_,N_,0.0);
	Vector<double> l(N_);
	Vector<double>* d(new Vector<double>[N_]);

	for(unsigned int i(0);i<N_;i++){
		d[i].set(N_,0.0);
		for(unsigned int j(0);j<N_;j++){
			d[i](j) = pop(j,i) - xmeanold_(j);
		}
		l(i) = sqrt((invB_*d[i]).norm_squared());
	}

	double norm_squared((invB_*(xmean_-xmeanold_)).norm_squared());
	double lmedian(l.median());
	double alpha(sqrt(N_/norm_squared));

	p_ *= (1.0 - cp_);
	p_ += d[N_]*alpha*sqrt(cp_*(2.0-cp_));

	for(unsigned int i(0);i<N_;i++){
		alpha = sqrt(N_)/std::max(l(i)/2.0,lmedian);
		Cmu += d[i]^d[i]*w_(i)*alpha*alpha;
	}

	if(norm_squared<1e-10){
		std::cerr<<__PRETTY_FUNCTION__<<" : risk of division by 0 : "<<norm_squared<<std::endl;
	} else {
		C_ *= (1.0 - c1_ - cmu_);
		C_ += p_^p_*c1_;
		C_ += Cmu * cmu_;

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
