/*! @file gnuplot2image.cpp*/

#include "Linux.hpp"
#include "Miscellaneous.hpp"

int main(int argc,char* argv[]){
	const std::string ext(".gp");
	if(argc==2 || argc==3){
		std::string filename(argv[1]);
		std::string option(argc==3?argv[2]:"");
		bool silent(option!="debug");

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
				command(Linux::pdflatex("/tmp/",texfile),silent);
				if(!command.status()){
					command("mv /tmp/" + texfile + ".pdf " + path + filename + ".pdf",silent);
					command("rm /tmp/" + texfile + "*",silent);
					if(argc==3){
						if(option=="png"){ command(Linux::pdf2png(path + filename, path + filename),silent); }
						else if(option=="jpg"){ command(Linux::pdf2png(path + filename, path + filename),silent); }
						else if(option!="debug"){ std::cerr<<__PRETTY_FUNCTION__<<" : unknown option (only possible options are 'png', 'jpg' or debug)"<<std::endl; }
					}
				} else { std::cerr<<__PRETTY_FUNCTION__<<" : can't compile latex file"<<std::endl; }
			} else { std::cerr<<__PRETTY_FUNCTION__<<" : the .gp script contains mistakes"<<std::endl; }
		} else { std::cerr<<__PRETTY_FUNCTION__<<" : the filename must have a '"<<ext<<"' extension"<<std::endl; }
	} else { 
		std::cerr<<argv[0]<<" : gnuplot2image file.gp [optional argument]"<<std::endl; 
		std::cerr<<std::string(std::string(argv[0]).size(),' ')<<" : the mandatory argument 'file.gp' is a Gnuplot script"<<std::endl;
		std::cerr<<std::string(std::string(argv[0]).size(),' ')<<" : the optional argument can be 'png', 'jpg' OR 'debug'"<<std::endl; 
		std::cerr<<std::string(std::string(argv[0]).size(),' ')<<" : if the optional argument is 'png' or 'jpg', a 'png' or 'jpg' image will be generated in addition to the default 'pdf' one"<<std::endl; 
		std::cerr<<std::string(std::string(argv[0]).size(),' ')<<" : if the optional argument is 'debug', the compilation log will be displayed"<<std::endl; 
	}
}
