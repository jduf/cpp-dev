#include "Directory.hpp"
#include "TP.hpp"

int main(int argc,char* argv[]){
	Parseur P(argc,argv);
	if(P.find("n")){
		TP tp(P);
		tp.display();
		tp.save();
	} else {
	unsigned int i(0);
		if(P.find("class",i,false)){
			Directory dir;
			dir.search_files(P.get<std::string>(i),"./tp",false,false,false);
			dir.sort();

			IOFiles ftp(dir.last(),false,false);
			TP tp(ftp);
			if(P.find("a")){
				tp.add();
				tp.save();
				tp.pdf();
				tp.clean();
			}
			if(P.find("e")){
				tp.edit();
				tp.save();
				tp.pdf();
				tp.clean();
			}
			if(P.find("s")){
				tp.display();
				tp.pdf();
				tp.clean();
			}
			if(P.find("d")){
				tp.display();
			}
		}
	}
}
