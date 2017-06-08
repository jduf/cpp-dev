/*! @file rst2pdf.cpp*/

#include "Linux.hpp"
#include "Miscellaneous.hpp"

int main(int argc,char* argv[]){
	const std::string ext(".rst");
	if(argc==2){
		std::string rstfile(argv[1]);
		if ( rstfile != ext && rstfile.size() > ext.size() && rstfile.substr(rstfile.size() - ext.size()) == ext ) {
			rstfile = rstfile.substr(0, rstfile.size() - ext.size());
			std::string path("");

			std::vector<std::string> tmp(my::string_split(path+rstfile,'/'));
			for(unsigned int i(0);i<tmp.size()-1;i++){ path += tmp[i] + "/"; }
			rstfile = tmp[tmp.size()-1];

			bool silent(true);
			Linux command;
			if(path[0] != '/'){ path = command.pwd() + path; }
			std::string texfile("/tmp/rst2latex_tmp");
			command(Linux::rst2latex(path+rstfile,texfile),silent);
			if(!command.status()){
				command("sed -i 's/\\[width=.*\\]/\\[width=1\\\\linewidth\\]/g' "+texfile+".tex",silent);
				command(Linux::pdflatex("/tmp/",texfile),silent);
				command("mv " + texfile + ".pdf " + path + rstfile + ".pdf",silent);
				command("rm " + texfile + "*",silent);
			} else { std::cerr<<__PRETTY_FUNCTION__<<" : the script contains mistakes"<<std::endl; }
		} else { std::cerr<<__PRETTY_FUNCTION__<<" : the reStructrueText file must have the '"<<ext<<"' extension"<<std::endl; }
	} else { std::cerr<<__PRETTY_FUNCTION__<<" : a reStructrueText file (*"<<ext<<") must be given as argument "<<std::endl; }
}
