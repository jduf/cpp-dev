/*! @file gnuplot2image.cpp*/

#include "Linux.hpp"
#include "Miscellaneous.hpp"

int main(int argc,char* argv[]){
	const std::string ext(".gp");
	if(argc==2 || argc==3){
		std::string filename(argv[1]);
		if ( filename != ext && filename.size() > ext.size() && filename.substr(filename.size() - ext.size()) == ext ) {
			filename = filename.substr(0, filename.size() - ext.size());
			std::string path("");

			std::vector<std::string> tmp(my::string_split(path+filename,'/'));
			for(unsigned int i(0);i<tmp.size()-1;i++){ path += tmp[i] + "/"; }
			filename = tmp[tmp.size()-1];

			Linux command;
			if(path[0] != '/'){ path = command.pwd() + path; }
			std::string texfile("gp2latex_tmp");
			command(Linux::gp2latex(path,filename,"/tmp/",texfile),false);
			if(!command.status()){
				bool silent(true);
				command(Linux::pdflatex("/tmp/",texfile),silent);
				if(!command.status()){
					command("mv /tmp/" + texfile + ".pdf " + path + filename + ".pdf",silent);
					command("rm /tmp/" + texfile + "*",silent);
					if(argc==3){
						std::string option(argv[2]);
						if(option=="png"){ command(Linux::pdf2png(path + filename, path + filename),silent); }
						else if(option=="jpg"){ command(Linux::pdf2png(path + filename, path + filename),silent); }
						else { std::cerr<<__PRETTY_FUNCTION__<<" : unknown option (only possible is 'png' or 'jpg')"<<std::endl; }
					}
				} else { std::cerr<<__PRETTY_FUNCTION__<<" : can't compile latex file"<<std::endl; }
			} else { std::cerr<<__PRETTY_FUNCTION__<<" : the .gp script contains mistakes"<<std::endl; }
		} else { std::cerr<<__PRETTY_FUNCTION__<<" : the filename must have a '"<<ext<<"' extension"<<std::endl; }
	} else { std::cerr<<__PRETTY_FUNCTION__<<" : a Gnuplot file (*"<<ext<<") must be given as argument (will generate a 'png' file if the second argument is 'png') "<<std::endl; }
}
