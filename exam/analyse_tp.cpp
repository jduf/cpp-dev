#include "TP.hpp"

int main(int argc,char* argv[]){
	Parseur P(argc,argv);
	if(P.find("n")){
		TP tp(P);
		tp.display();
		tp.save();
	}
	if(P.find("a")){
		IOFiles ftp(P.get<std::string>("class"),false,false);
		TP tp(ftp);
		tp.add();
		tp.save();
		tp.pdf();
		tp.clean();
	}
	if(P.find("e")){
		IOFiles ftp(P.get<std::string>("class"),false,false);
		TP tp(ftp);
		tp.edit();
		tp.save();
		tp.pdf();
		tp.clean();
	}
	if(P.find("s")){
		IOFiles ftp(P.get<std::string>("class"),false,false);
		TP tp(ftp);
		tp.display();
		tp.pdf();
		tp.clean();
	}
}
