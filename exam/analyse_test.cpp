#include "Directory.hpp"
#include "Examen.hpp"

int main(int argc,char* argv[]){
	Parseur P(argc,argv);
	if(P.find("n")){
		Examen exa(P);
		exa.edit();
		exa.summary();
		exa.save();
		exa.clean();
	} else {
		unsigned int i(0);
		if(P.find("class",i,false)){
			Directory dir;
			dir.search_files(P.get<std::string>(i),"./exa",false,false,false);
			dir.sort();

			IOFiles fexa(dir.last(),false,false);
			Examen exa(fexa);
			if(P.find("e")){
				exa.edit();
				exa.summary();
				exa.save();
				exa.clean();
			}
			if(P.find("s")){
				exa.analyse();
				exa.display();
				exa.summary();
				//exa.clean();
			}
			if(P.find("d")){
				exa.display();
			}
		}
	}
}