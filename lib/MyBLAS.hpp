#ifndef DEF_MYBLAS
#define DEF_MYBLAS

namespace BLAS{
	/*{BLAS level 1*/
	extern "C" double ddot_(unsigned int const& N, double const* const dx, unsigned int const& ix, double const* const dy, unsigned int const& iy);
	inline double dot(
			unsigned int const& N,
			double const* const a,
			bool const& ar, //true : multiply a row of a
			unsigned int const& arow,// 1 for Vector
			unsigned int aidx, // 0 for Vector
			double const* const b,
			bool const& br, //true : multiply a row of b
			unsigned int const& brow,// 1 for Vector
			unsigned int bidx// 0 for Vector
			)
	{
		if(ar){//multiply a row of a
			if(br){ return ddot_(N,/**/a+aidx,     arow,/**/b+bidx,     brow); }
			else  { return ddot_(N,/**/a+aidx,     arow,/**/b+bidx*brow,1); }
		} else {
			std::cout<<__PRETTY_FUNCTION__<<" : need to be checked"<<std::endl;
			if(br){ return ddot_(N,/**/a+aidx*arow,1,   /**/b+bidx,     brow); }
			else  { return ddot_(N,/**/a+aidx*arow,1,   /**/b+bidx*brow,1); }
		}
	}

#ifdef MY_BLAS_ZDOTU
#include <immintrin.h>
#include <emmintrin.h>
	/*!need to redefine this method because for some reason, with an intel
	 * compiler (icpc) the blas::zdotu_ doesn't work properly (at least with
	 * mkl). with gcc, it seems to work fine.*/
	extern "C" {
		inline std::complex<double> zdotu_(unsigned int const& N, std::complex<double> const* const dx, unsigned int ix, std::complex<double> const* const dy, unsigned int iy){
			double const* x = reinterpret_cast<double const*>(dx);
			double const* y = reinterpret_cast<double const*>(dy);

			__m256d dmm0;
			__m256d dmm1;
			__m256d dmm4(_mm256_setzero_pd());
			__m256d dmm5(_mm256_setzero_pd());
			if(ix==1 && iy==1){
				for(unsigned int i(0);i<2*N-2;i+=4){
					dmm0 = _mm256_loadu_pd(x + i);
					dmm1 = _mm256_loadu_pd(y + i);

					/*{would work on Haswell processors
					  dmm4 = _mm256_fmadd_pd(dmm1, dmm0, dmm4);
					  dmm2 = _mm256_permute_pd(dmm1, 0x5);
					  dmm5 = _mm256_fmadd_pd(dmm2, dmm0, dmm5);
					  }*/

					dmm4 = _mm256_add_pd(_mm256_mul_pd(dmm1,dmm0), dmm4);
					dmm1 = _mm256_permute_pd(dmm1, 0x5);
					dmm5 = _mm256_add_pd(_mm256_mul_pd(dmm1,dmm0), dmm5);
				}
			} else {
				for(unsigned int i(0);i<2*N-2;i+=4){
					dmm0 = _mm256_set_pd(x[ix*(i+2)+1], x[ix*(i+2)], x[ix*i+1], x[ix*i]);
					dmm1 = _mm256_set_pd(y[iy*(i+2)+1], y[iy*(i+2)], y[iy*i+1], y[iy*i]);

					/*{would work on Haswell processors
					  dmm4 = _mm256_fmadd_pd(dmm1, dmm0, dmm4);
					  dmm2 = _mm256_permute_pd(dmm1, 0x5);
					  dmm5 = _mm256_fmadd_pd(dmm2, dmm0, dmm5);
					  }*/

					dmm4 = _mm256_add_pd(_mm256_mul_pd(dmm1,dmm0), dmm4);
					dmm1 = _mm256_permute_pd(dmm1, 0x5);
					dmm5 = _mm256_add_pd(_mm256_mul_pd(dmm1,dmm0), dmm5);
				}
			}
			double* re = (double*)&dmm4;
			double* im = (double*)&dmm5;
			std::complex<double> out(re[0]-re[1]+re[2]-re[3],im[0]+im[1]+im[2]+im[3]);
			if(N%2){ out += dx[(N-1)*ix]*dy[(N-1)*iy]; }
			return out;
		}
	}
#else
	extern "C" std::complex<double> zdotu_(unsigned int const& N, std::complex<double> const* const dx, unsigned int const& ix, std::complex<double> const* const dy, unsigned int const& iy);
#endif
	inline std::complex<double> dot(
			unsigned int const& N,
			std::complex<double> const* const a,
			bool const& ar, //true : multiply a row of a
			unsigned int const& arow,// 1 for Vector
			unsigned int aidx, // 0 for Vector
			std::complex<double> const* const b,
			bool const& br,  //true : multiply a row of b
			unsigned int const& brow,// 0 for Vector
			unsigned int bidx// 1 for Vector
			)
	{
		if(ar){
			if(br){ return zdotu_(N,/**/a+aidx,     arow,/**/b+bidx,     brow); }
			else  { return zdotu_(N,/**/a+aidx,     arow,/**/b+bidx*brow,1); }
		} else {
			std::cout<<__PRETTY_FUNCTION__<<" : need to be checked"<<std::endl;
			if(br){ return zdotu_(N,/**/a+aidx*arow,1,   /**/b+bidx,     brow); }
			else  { return zdotu_(N,/**/a+aidx*arow,1,   /**/b+bidx*brow,1); }
		}
	}
	/*}*/

	/*{BLAS level 2*/
	extern "C" void dgemv_(char const& trans, unsigned int const& N, unsigned int const& M, double const& alpha, double const* const a, unsigned int const& lda, double const* const x, unsigned int const& incx, double const& beta, double const* y, unsigned int const& incy);
	inline void gemv(
			char const& trans,
			unsigned int const& N,
			unsigned int const& M,
			double const* const a,
			double const* const x,
			unsigned int const& incx,
			double const* y
			)
	{
		dgemv_(trans,N,M,1.0,a,M,x,incx,0.0,y,1);
	}

	extern "C" void zgemv_(char const& trans, unsigned int const& N, unsigned int const& M, std::complex<double> const& alpha, std::complex<double> const* const a, unsigned int const& lda, std::complex<double> const* const x, unsigned int const& incx, std::complex<double> const& beta, std::complex<double> const* y, unsigned int const& incy);
	inline void gemv(
			char const& trans,
			unsigned int const& N,
			unsigned int const& M,
			std::complex<double> const* const a,
			std::complex<double> const* const x,
			unsigned int const& incx,
			std::complex<double> const* y
			)
	{
		zgemv_(trans,N,M,1.0,a,M,x,incx,0.0,y,1);
	}
	/*}*/
}
#endif
