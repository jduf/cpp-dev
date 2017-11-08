#include "Examen.hpp"

int main(int argc,char* argv[]){
	Parseur P(argc,argv);
	if(P.find("n")){
		Examen exa(P);
		exa.edit();
		exa.analyse();
		exa.save();
		exa.histogram();
		exa.summary();
		exa.clean();
	}
	if(P.find("e")){
		IOFiles fexa(P.get<std::string>("class"),false,false);
		Examen exa(fexa);
		exa.edit();
		exa.analyse();
		exa.save();
		exa.histogram();
		exa.summary();
		exa.clean();
	}
	if(P.find("p")){
		IOFiles fexa(P.get<std::string>("class"),false,false);
		Examen exa(fexa);
		exa.analyse();
		exa.histogram();
		exa.summary();
		exa.clean();
	}
	if(P.find("d")){
		IOFiles fexa(P.get<std::string>("class"),false,false);
		Examen exa(fexa);
		exa.display();
	}
}
