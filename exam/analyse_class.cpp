#include "Directory.hpp"
#include "Class.hpp"

int main(int argc,char* argv[]){
	Parseur P(argc,argv);
	unsigned int i;
	if(P.find("file",i,false)){
		IOFiles data(P.get<std::string>(i),true,false);
		Class c(data);
		c.summary();
	} else {
		Class c;
		c.init(P.get<std::string>("class"),P.get<std::string>("exa"),P.get<std::string>("tp"));
		c.save();
	}
}
