#include "Gnuplot.hpp"

Gnuplot::Gnuplot(std::string const& path, std::string const& filename):
	path_(my::ensure_trailing_slash(path)),
	filename_(filename),
	plot_(""),
	multiplot_(false)
{}

void Gnuplot::multiplot(){
	plot_ += "set multiplot\n";
	multiplot_ = true;
}

void Gnuplot::title(std::string const& title){ plot_+="set title '"+title+"'\n"; }

void Gnuplot::range(std::string const& axis, std::string const& a, std::string const& b){
	plot_+="set "+axis+"range ["+a+":"+b+"]\n";
}
void Gnuplot::range(std::string const& axis, double const& a, double const& b){
	range(axis,my::tostring(a),my::tostring(b));
}
void Gnuplot::range(std::string const& axis, double const& a, std::string const& b){
	range(axis,my::tostring(a),b);
}
void Gnuplot::range(std::string const& axis, std::string const& a, double const& b){
	range(axis,a,my::tostring(b));
}
void Gnuplot::range(std::string const& axis, std::string const& s){
	plot_ += "set "+axis+"range "+s+"\n";
}
void Gnuplot::range(std::string const& axis){
	plot_ += "unset "+axis+"range\n";
}

void Gnuplot::tics(std::string const& axis, std::string const& t){
	plot_+="set "+axis+"tics "+t+"\n";
}
void Gnuplot::tics(std::string const& axis, double const& t){
	tics(axis,my::tostring(t));
}
void Gnuplot::tics(std::string const& axis){
	plot_ += "unset "+axis+"tics\n"; }

void Gnuplot::margin(std::string const& l, std::string const& r, std::string const& t, std::string const& b){
	plot_ += "set lmargin at screen "+l+"\n";
	plot_ += "set rmargin at screen "+r+"\n";
	plot_ += "set tmargin at screen "+t+"\n";
	plot_ += "set bmargin at screen "+b+"\n";
}

void Gnuplot::margin(double const& l, double const& r, double const& t, double const& b){
	margin(my::tostring(l),my::tostring(r),my::tostring(t),my::tostring(b));
}

void Gnuplot::label(std::string const& axis, std::string const& l, std::string const& options){ plot_ += "set "+axis+"label '"+l+"' "+options+"\n"; }
void Gnuplot::label(std::string const& axis){ plot_ += "unset "+axis+"label\n"; }

void Gnuplot::key(std::string const& option){ plot_ += "set key "+option+"\n"; }

void Gnuplot::operator=(std::string const& s){ plot_ = s + "\n"; }
void Gnuplot::operator+=(std::string const& s){ plot_ += s + "\n"; }

void Gnuplot::save_file(){
	IOFiles w_gp(path_ + filename_+".gp",true,false);
	if(multiplot_){ plot_ += "unset multiplot\n"; }
	w_gp<<plot_<<IOFiles::endl;
}

void Gnuplot::create_image(bool const& silent, std::string const& format){
	std::string texfile(filename_);
	size_t pos(texfile.find("."));
	while(pos != std::string::npos){
		texfile.replace(pos,1,"");
		pos = texfile.find(".");
	}

	Linux command;
	command(Linux::gp2latex(path_,filename_,"/tmp/",texfile),silent);
	if(!command.status()){
		command(Linux::pdflatex("/tmp/",texfile),silent);
		if(!command.status()){
			command("mv /tmp/" + texfile + ".pdf " + path_ + filename_ + ".pdf",silent);
			command("rm /tmp/" + texfile + "*",silent);
			if(format == "png"){ command(Linux::pdf2png(path_ + filename_, path_ + filename_),silent); }
			if(format == "jpg"){ command(Linux::pdf2jpg(path_ + filename_, path_ + filename_),silent); }
		} else { std::cerr<<__PRETTY_FUNCTION__<<" : Linux::pdflatex(\"/tmp/\",texfile) returned an error ("<<command.status()<<")"<<std::endl; }
	} else { std::cerr<<__PRETTY_FUNCTION__<<" : Linux::gp2latex(\"/tmp/\"+texfile,filename_,texfile) returned an error ("<<command.status()<<")"<<std::endl; }
}

std::string Gnuplot::histogram(Vector<double> const& data, double const& min, double const& max, double const& bin_width, std::string const& id, std::string const& title){
	unsigned int nbins(ceil((max-min)/bin_width+1));
	Vector<double> xbin(nbins);
	Vector<unsigned int> ybin(nbins,0);
	for(unsigned int i(0);i<nbins;i++){ xbin(i) = min+i*bin_width; }

	double data_average(0);
	for(unsigned int i(0);i<data.size();i++){
		for(unsigned int j(0);j<nbins;j++){
			if(std::abs(data(i)-xbin(j))<=bin_width/2.0){
				ybin(j)++;
				data_average += data(i);
				j=nbins;
			}
		}
	}
	data_average /= ybin.sum();

	std::string fname("histogram-"+id+"-"+my::tr(my::tr(title,' '),'.'));
	IOFiles fbins(fname+".dat",true,false);
	for(unsigned int i(0);i<nbins;i++){
		fbins<<xbin(i)<<" "<<ybin(i)<<IOFiles::endl;
	}
	Gnuplot histogram("./",fname);
	histogram += "set boxwidth 0.5 relative";
	histogram += "set style fill transparent solid 0.5 noborder";
	histogram.key("left");
	histogram.tics("x",bin_width);
	histogram.range("x",min-bin_width/2.0,max+bin_width/2.0);
	histogram.tics("y",1);
	histogram.range("y",0,ybin.max()+1);
	histogram += "plot '"+fname+".dat' using 1:2 with boxes t '"+title+" : Moyenne "+my::tostring(data_average)+"'";
	histogram.save_file();
	histogram.create_image(true);

	return fname;
}
