#include "PSTricks.hpp"

PSTricks::PSTricks(std::string const& path, std::string const& filename):
	path_(my::ensure_trailing_slash(path)),
	filename_(filename),
	s_(""),
	begin_end_(false)
{
	s_ += "\\documentclass[pstricks]{standalone}\n" ;
	s_ += "\\usepackage{pstricks-add}\n";
}

void PSTricks::begin(double const& xbl, double const& ybl, double const& xtr, double const& ytr, std::string const& imagename){
	s_ += "\\begin{document}\n";
	s_ += "\\begin{pspicture}("+my::tostring(my::chop(xbl))+","+my::tostring(my::chop(ybl))+")("+my::tostring(my::chop(xtr))+","+my::tostring(my::chop(ytr))+")%"+imagename+"\n";
	begin_end_ = true;
}

void PSTricks::add(std::string const& s){
	s_ += s + "\n";
}

void PSTricks::line(std::string const& linetype, double const& x0, double const& y0, double const& x1, double const& y1, std::string const& options){
	s_ += "\\psline[" + options + "]{" + linetype + "}(" + my::tostring(my::chop(x0)) + "," + my::tostring(my::chop(y0)) + ")(" + my::tostring(my::chop(x1)) + "," + my::tostring(my::chop(y1)) + ")\n";
}

void PSTricks::linked_lines(std::string const& linetype, Matrix<double> const& xy, std::string const& options){
	for(unsigned int i(0);i+1<xy.row();i++){
		line(linetype,xy(i,0),xy(i,1),xy(i+1,0),xy(i+1,1),options);
	}
}

void PSTricks::polygon(Matrix<double> const& xy, std::string const& options){
	s_ += "\\pspolygon[" + options + "]";
	for(unsigned int i(0);i<xy.row();i++){
		s_ += "(" + my::tostring(my::chop(xy(i,0))) + "," + my::tostring(my::chop(xy(i,1))) + ")";
	}
	s_ += "\n";
}

void PSTricks::frame(double const& x0, double const& y0, double const& x1, double const& y1, std::string const& options){
	s_ += "\\psframe[" + options + "](" + my::tostring(my::chop(x0)) + "," + my::tostring(my::chop(y0)) + ")(" + my::tostring(my::chop(x1)) + "," + my::tostring(my::chop(y1)) + ")\n";
}

void PSTricks::put(double const& x, double const& y, std::string const& s, std::string const& options){
	s_ += "\\rput" + options + "("+my::tostring(my::chop(x))+","+my::tostring(my::chop(y))+"){"+s+"}\n";
}

void PSTricks::pie(double const& x, double const& y, Vector<double> const& p, double const& r, std::string const& options){
	s_ += "\\rput{ 0}("+my::tostring(my::chop(x))+","+my::tostring(my::chop(y))+"){";
	s_ += "\\psChart["+options+",linestyle=none]{";
	for(unsigned int i(0); i<p.size(); i++){
		s_ += my::tostring(my::chop(p(i)));
		if(i+1<p.size()){ s_ += ","; }
	}
	s_ += "}{}{"+my::tostring(my::chop(r))+"}}\n";
}

void PSTricks::circle(Vector<double> const& x, double const& r, std::string const& options){
	s_ += "\\pscircle["+options+"]("+my::tostring(my::chop(x(0)))+","+my::tostring(my::chop(x(1)))+"){"+my::tostring(my::chop(r))+"}\n";
}

void PSTricks::cross(Vector<double> const& x, double const& r, std::string const& options){
	double a(sqrt(2.0)*r/2.0);
	s_ += "\\psline[" + options + "]{-}(" + my::tostring(my::chop(x(0)-a)) + "," + my::tostring(my::chop(x(1)-a)) + ")(" + my::tostring(my::chop(x(0)+a)) + "," + my::tostring(my::chop(x(1)+a)) + ")\n";
	s_ += "\\psline[" + options + "]{-}(" + my::tostring(my::chop(x(0)-a)) + "," + my::tostring(my::chop(x(1)+a)) + ")(" + my::tostring(my::chop(x(0)+a)) + "," + my::tostring(my::chop(x(1)-a)) + ")\n";
}

void PSTricks::end(bool const& silent, bool const& png, bool const& crop){
	if(begin_end_){ s_ += "\\end{pspicture}\n"; }
	s_ += "\\end{document}\n";
	{/*to make sure that the file w is closed after the brackets*/
		IOFiles w(path_+filename_ + ".tex",true,false);
		w<<s_;
	}

	Linux command;
	command(Linux::latex(path_,filename_),silent);
	if(!command.status()){
		command(Linux::dvipdf(path_+filename_,path_+filename_),silent);
		command("rm " + path_+filename_+".dvi " + path_+filename_+".aux " + path_+filename_+".log",silent);
		if(crop){ command(Linux::pdfcrop(path_+filename_,path_+filename_),silent); }
		if(png){ command(Linux::pdf2png(path_+filename_,path_+filename_),silent); }
	} else { std::cerr<<__PRETTY_FUNCTION__<<" : Linux::latex(path_,filename_) returned an error ("<<command.status()<<")"<<std::endl; }
}
