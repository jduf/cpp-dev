/*! @file gpctrparamcustom.cpp*/

#include "Miscellaneous.hpp"

int main(int argc, char* argv[]){
	if(argc==3){
		std::vector<std::string> tmp(my::string_split(argv[1],':'));
		std::string type(argv[2]);
		if(type=="linear"){
			double xa(my::string2type<double>(tmp[0]));
			unsigned int n(my::string2type<unsigned int>(tmp[1]));
			double xb(my::string2type<double>(tmp[2]));
			double dx(2.0*(xb-xa)/n);
			unsigned int i(0);
			while(-xb+i*dx<xb){
				std::cout<<-xb+(i*dx)<<",";
				i++;
			}
			std::cout<<xb<<std::endl;
		} else if(type=="quadratic"){
			double xa(sqrt(my::string2type<double>(tmp[0])));
			unsigned int n(my::string2type<unsigned int>(tmp[1]));
			double xb(sqrt(my::string2type<double>(tmp[2])));
			double dx((xb-xa)/n);
			for(unsigned int i(n);i>=1;i--){ std::cout<<-(i*dx)*(i*dx)<<","; }
			for(unsigned int i(0);i<n;i++){ std::cout<<(i*dx)*(i*dx)<<","; }
			std::cout<<xb*xb<<std::endl;
		} else { std::cerr<<__PRETTY_FUNCTION__<<" : undefined type of contour parameter"<<std::endl; }
	}
}
