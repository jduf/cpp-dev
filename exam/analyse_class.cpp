#include "Directory.hpp"
#include "Class.hpp"

int main(int argc,char* argv[]){
	Parseur P(argc,argv);
	unsigned int i;
	if(P.find("file",i,false)){
		Directory dir;
		dir.search_files(P.get<std::string>(i),"./class",false,true,false);
		dir.sort();
		std::cout<<dir.last()<<std::endl;

		IOFiles data(dir.last(),false,false);
		Class c(data);
		c.summary();
		c.clean();
		c.save();
	}
}
