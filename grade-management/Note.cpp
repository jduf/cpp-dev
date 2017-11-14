#include "Note.hpp"

Note::Note(unsigned int const& nstudents):
	title_(my::get_string("Titre")),
	grades_(nstudents,0.0),
	average_(0),
	nfails_(0)
{}

Note::Note(IOFiles& ftp):
	title_(ftp.read<std::string>()),
	points_(ftp.read<Matrix<double> >()),
	max_points_(ftp.read<Vector<double> >()),
	grades_(ftp.read<Vector<double> >()),
	average_(ftp.read<double>()),
	nfails_(ftp.read<unsigned int>())
{}

void Note::save(IOFiles& w){
	w.write("Titre",title_);
	w<<points_<<max_points_<<grades_;
	w.write("Moyenne",average_);
	w.write("Échecs",nfails_);
}

std::string Note::histogram(Vector<double> const& data, double const& min, double const& max, double const& bin_width, std::string const& class_id, std::string const& title){
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
	data_average /= nvalid_;

	std::string fname("histogram-"+class_id+"-"+title);
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
