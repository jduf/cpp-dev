#include "Latex.hpp"

Latex::Latex(std::string const& path, std::string const& filename, std::string const& documentclass, std::string const& options):
	path_(my::ensure_trailing_slash(path)),
	filename_(filename),
	latex_("\\documentclass["+options+"]{"+documentclass+"}\n")
{}

void Latex::package(std::string const& s, std::string const& options){
	latex_+="\\usepackage[" + options + "]{" + s + "}\n";
}
void Latex::begin(std::string const& s, std::string const& options){
	latex_+="\\begin{" + s + "}" + options + "\n";
}
void Latex::end(std::string const& s){
	latex_+="\\end{" + s + "}\n";
}
void Latex::command(std::string const& s){
	latex_+="\\" + s + "\n";
}
void Latex::item(std::string const& s){
	latex_+="\\item " + s + "\n";
}
void Latex::histogram(std::vector<double> const& data, unsigned int const& nbins, double const& min, double const& max){
	latex_+= "\\addplot +[ hist={bins="
		+my::tostring(nbins)+ ", data min="
		+my::tostring(min)+ ", data max=+"
		+my::tostring(max)+ "} ] table [y index=0] {\n";
	for(auto const& d:data){ latex_ += my::tostring(d) + "\n"; }
	latex_+="};";
}

void Latex::operator=(std::string const& s){ latex_ = s + "\n"; }
void Latex::operator+=(std::string const& s){ latex_ += s + "\n"; }

void Latex::save_file(){
	IOFiles w_latex(path_ + filename_+".tex",true,false);
	w_latex<<latex_<<IOFiles::endl;
}

void Latex::pdflatex(bool const& silent, bool const& clean){
	Linux command;
	command(Linux::pdflatex(path_,filename_),silent);
	if(command.status()){ std::cerr<<__PRETTY_FUNCTION__<<" : Linux::pdflatex("<<path_<<","<<filename_<<") returned an error ("<<command.status()<<")"<<std::endl; }
	else {
		if(clean){
			command("rm " + path_ + filename_ + ".aux " + path_ + filename_ + ".log " + path_ + filename_ + ".toc " + path_ + filename_ + ".out "+ path_ + filename_ + ".tex ",silent);
			if(command.status()){ std::cerr<<__PRETTY_FUNCTION__<<" : failed to remove auxiliary tex files"<<std::endl; }
		}
	}
}
