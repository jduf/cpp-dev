#include "Vector.hpp"
#include "Rand.hpp"

int main(){
	//Rand<double> r(0,100);
	//Rand<unsigned int> ur(10,100);
	//Vector<std::complex<double> > a(ur.get());
	//Vector<std::complex<double> > b(a.size());
	//for(unsigned int i(0);i<a.size();i++){
		//a(i) = std::complex<double>(r.get(),r.get());
		//b(i) = std::complex<double>(r.get(),r.get());
	//}

	//Matrix<std::complex<double> > a(3,31);
	//Vector<std::complex<double> > b(31);
	//for(unsigned int i(0);i<b.size();i++){
		//a(1,i) = std::complex<double>(0,1.*i);
		//b(i) = std::complex<double>(1.*i,0);
	//}
	//a(1,0) = std::complex<double>(10,11);
	//a(1,1) = std::complex<double>(12,13);

	//std::cout<<BLAS::dot(b.size(),a.ptr(),true,a.row(),1,b.ptr(),true,1,0)<<std::endl;
	//
	//std::string s("2,1,3,4,1,2,1,3,4,1,2");
	//std::string ss("2,1");
	//std::cout<<my::count_substring(s,ss)<<std::endl;

	std::cout<<my::tostring(1.234567890,5)<<std::endl;
	std::cout<<my::tostring(1.34567890,5)<<std::endl;
	std::cout<<my::tostring(1.4567890,5)<<std::endl;
	std::cout<<my::tostring(1.567890,5)<<std::endl;
	std::cout<<my::tostring(1.00890,1)<<std::endl;
	std::cout<<my::tostring(1.00890,0)<<std::endl;
	std::cout<<my::tostring(1.78,1)<<std::endl;
	std::cout<<my::tostring(1.78,0)<<std::endl;
	std::cout<<std::endl;

	std::cout<<my::tostring(0,7)<<std::endl;
	std::cout<<my::tostring(1.23,7)<<std::endl;
	std::cout<<my::tostring(12.34,7)<<std::endl;
	std::cout<<my::tostring(123.456,7)<<std::endl;
	std::cout<<my::tostring(1234.5,7)<<std::endl;

}
