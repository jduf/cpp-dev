#include "IOFiles.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

void write_bin(){
	std::cout<<"écriture d'un fichier binaire"<<std::endl;
	Matrix<double> M(5,3,2.5);
	std::complex<double> c(1.2,2);
	Vector<std::complex<double> > v(3,c);
	double a(12.2);
	std::string slt("salut");
	IOFiles write("data.jdbin",true);
	write<<M<<c<<v<<slt;
	write("test",a);
}

void read_bin(){
	std::cout<<"lecture d'un fichier binaire"<<std::endl;
	IOFiles read("data.jdbin",false);
	Matrix<double> M;
	Vector<std::complex<double> > v;
	std::complex<double> c;
	double a;
	std::string slt;
	read>>M>>c>>v>>slt>>a;
	std::cout<<read.get_header()<<std::endl;
	std::cout<<a<<std::endl;
	std::cout<<M<<std::endl;
	std::cout<<c<<std::endl;
	std::cout<<v<<std::endl;
	std::cout<<slt<<std::endl;

}

void write_txt(){
	std::cout<<"écriture d'un fichier text"<<std::endl;
	IOFiles write_single("single.dat",true);
	IOFiles write_mat("matrice.dat",true);
	IOFiles write_matc("matrice-complex.dat",true);

	double a(12.3);
	Matrix<double> M(5,5,2.5);
	std::complex<double> c(3.1,0.20);
	Matrix<std::complex<double> > C(2,3,c);

	write_single<<a<<" "<<c<<IOFiles::endl;
	write_mat <<M<<IOFiles::endl;
	write_matc<<C<<IOFiles::endl;
}

void read_txt(){
	std::cout<<"lecture d'un fichier text"<<std::endl;
	IOFiles read_single("single.dat",false);
	IOFiles read_mat("matrice.dat",false);
	IOFiles read_matc("matrice-complex.dat",false);

	double a(0);
	Matrix<double> M1(5,5);
	std::complex<double> c(0.0);
	Matrix<std::complex<double> > C1;

	read_single >> a >> c;
	std::cout<<a<<" "<<c<<std::endl;

	read_mat >> M1;
	std::cout<<M1<<std::endl;

	read_matc >> C1;
	std::cout<<C1<<std::endl;
}

int main(){
	write_bin();
	read_bin();
	write_txt();
	read_txt();
}
