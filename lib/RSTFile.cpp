#include "RSTFile.hpp"

RSTFile::RSTFile(std::string const& path, std::string const& filename):
	path_(my::ensure_trailing_slash(path)),
	filename_(filename)
{}

void RSTFile::save(bool const& html, bool const& pdf, bool const& silent){
	{
		IOFiles w(path_+filename_+".rst",true,false);
		w<<rst_;
	}
	Linux command;
	if(html){
		command(Linux::rst2html(path_+filename_,path_+filename_),silent);
		if(command.status()){ std::cerr<<__PRETTY_FUNCTION__<<" : Linux::rst2html(path_,filename_) returned an error ("<<command.status()<<")"<<std::endl; } 
	}
	if(pdf){
		command(Linux::rst2latex(path_+filename_,path_+filename_),silent);
		//To get resize the image inside the pdf
		command("sed -i 's/\\[width=800bp]/\\[width=1\\\\linewidth\\]/g' "+path_+filename_+".tex",true);
		command(Linux::pdflatex(path_,filename_),silent);
		command("(cd " + path_ + "; rm " +filename_+".out "+filename_+".aux "+filename_+".log)",silent);
		if(command.status()){ std::cerr<<__PRETTY_FUNCTION__<<" : Linux::pdflatex(inpath,infile,outpath) returned an error ("<<command.status()<<")"<<std::endl; }
	}
}
