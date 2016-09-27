#include"FuncPSO.hpp"
#include"Parseur.hpp"

void generate_surface();
void run_plots(unsigned int N,unsigned int maxiter, bool random);
void run_stats(unsigned int N,unsigned int maxiter, bool random);

int main(int argc,char* argv[]){
	Parseur P(argc,argv);

	//run_plots(P.get<unsigned int>("N"),P.get<unsigned int>("maxiter"),P.get<bool>("random"));
	run_stats(P.get<unsigned int>("N"),P.get<unsigned int>("maxiter"),P.get<bool>("random"));
}

void generate_surface(){
	FuncPSO s(1,1,0);
	unsigned int n(100);
	double dx(8.0/n);
	IOFiles data("data.dat",true,false);
	Vector<double> x(2);
	for(unsigned int i(0);i<n;i++){
		x(0) = -4.0+i*dx;
		for(unsigned int j(0);j<n;j++){
			x(1) = -4.0+j*dx;
			data<<x<<" "<<s.f(x)<<IOFiles::endl;
		}
		data<<IOFiles::endl;
	}
}

void run_plots(unsigned int N,unsigned int maxiter, bool random){
	FuncPSO s(N,maxiter,random);

	s.init_PSO(200.0);
	s.minimize();
	//s.result();

	IOFiles measure("measure.dat",true,false);
	List<Measure>* m(s.get_m());
	m->set_target();
	while(m->target_next()){
		measure<<m->get().get_x()<<" "<<m->get().get_fx()<<" "<<m->get().get_dfx()<<" "<<m->get().get_N()<<IOFiles::endl;
	}

	Gnuplot gpe("./","evolution");
	//gp+="set logscale y";
	gpe.multiplot();
	gpe.margin("0.1","0.95","0.95","0.5");
	for(unsigned int i(0);i<N;i++){
		gpe+=std::string(i==0?"plot":"    ")+" 'evolution.dat' u ($1=="+my::tostring(i)+"?sqrt($2*$2+$3*$3):1/0) notitle"+(i!=N-1?",\\":"");
	}
	gpe.margin("0.1","0.95","0.5","0.1");
	gpe.range("y","-1","14");
	for(unsigned int i(0);i<N;i++){
		gpe+=std::string(i==0?"plot":"    ")+" 'evolution.dat' u ($1=="+my::tostring(i)+"?$4:1/0) notitle"+(i!=N-1?",\\":"");
	}
	gpe.save_file();
	gpe.create_image(true,"png");

	Gnuplot gp("./","plot");
	//gp+="set xyplane at 0";
	gp+="set cntrparam level auto 15";
	gp+="set contour base";
	gp+="set view map";
	gp+="splot 'data.dat' u 1:2:3 w pm3d notitle,\\";
	gp+="      'measure.dat' u 1:2:(0) w p pt 7 lc rgb 'white' notitle";
	gp.save_file();
	gp.create_image(true,"png");
}

void run_stats(unsigned int N,unsigned int maxiter, bool random){
	for(unsigned int i(0);i<1000;i++){
		FuncPSO s(N,maxiter,random);

		s.init_PSO(200.0);
		s.minimize();

		List<Measure>::Node* best(NULL);
		List<Measure>* m(s.get_m());
		m->set_target();
		while(m->target_next()){
			if(!best || m->get().get_fx()<best->get()->get_fx()){ best = m->get_target(); }
		}
		std::cerr<<best->get()->get_x()<<" "<<best->get()->get_fx()<<std::endl;
	}
}
