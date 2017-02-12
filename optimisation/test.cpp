#include "ACiD.hpp"
#include "Gnuplot.hpp"

int main(){
	unsigned int N(2);
	unsigned int iter(0);
	Vector<double> xa(N,-5.0);
	Vector<double> xb(N,5.0);

	for(unsigned int i(0);i<100;i++){
		ACiD min(N,xa,xb);
		double bf(1e10);
		min.run(3000,bf);
		iter += min.get_iter();
	}
	std::cout<<iter<<std::endl;

	Gnuplot gp("./","plot");
	gp.range("x",-5,5);
	gp.range("y",-5,5);
	//gp += "f(x,y) = (1-x)*(1-x)+100*(x-y*y)*(x-y*y)";
	//gp += "splot 'data.dat' u 1:2:3 notitle, f(x,y) notitle";
	//gp += "plot 'data.dat' u 1:2 w lp ps 1 pt 7";
	gp += "plot 'data.dat' u 1:2 w lp ps 1 pt 7,\\";
	gp += "     'data.dat' u 3:4 w lp ps 1 pt 5";
	//gp += "     'data.dat' u 6:7 w lp";
	gp.save_file();
	gp.create_image(true);
}
