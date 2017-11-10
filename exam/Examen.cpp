#include "Examen.hpp"

Examen::Examen(Parseur& P):
	class_id_(P.get<std::string>("class")),
	max_points_(P.get<std::vector<double> >("maxpoints"))
{
	{
		Linux command;
		command("vim "+ class_id_+".txt",false);
	}

	std::fstream file(class_id_+".txt",std::ios::in);
	std::string tmps;
	while(getline(file,tmps)){ class_list_.push_back(tmps); }
}

Examen::Examen(IOFiles& fexa):
	class_id_(fexa.read<std::string>()),
	date_(fexa.read<std::string>()),
	bonus_test_(fexa.read<double>()),
	bonus_coef_(fexa.read<double>()),
	average_(fexa.read<double>()),
	average_grades_with_bonus_(fexa.read<double>()),
	average_final_(fexa.read<double>()),
	nfails_(fexa.read<unsigned int>()),
	points_(fexa.read<Matrix<double> >()),
	max_points_(fexa.read<Vector<double> >()),
	points_bonus_(fexa.read<Vector<double> >()),
	grades_(fexa.read<Vector<double> >()),
	grades_with_bonus_(fexa.read<Vector<double> >()),
	grades_final_(fexa.read<Vector<double> >())
{
	class_list_.resize(fexa.read<unsigned int>());
	for(auto& name:class_list_){ fexa>>name; }
}

void Examen::analyse(){
	nfails_ = 0;
	average_ = 0.0;
	average_grades_with_bonus_ = 0.0;
	average_final_ = 0.0;
	unsigned int n_valid(0);
	unsigned int total_points(max_points_.sum());
	for(unsigned int i(0);i<grades_.size();i++){
		grades_(i) = 0;
		for(unsigned int j(0);j<points_.col();j++){
			grades_(i) += points_(i,j);
		}
		grades_(i) = std::min(5*grades_(i)/(total_points-bonus_test_)+1.0,6.0);
		grades_with_bonus_(i) = std::min(grades_(i) + points_bonus_(i)*bonus_coef_,6.0);
		grades_final_(i) = my::round_nearest(grades_with_bonus_(i),2);

		if(!my::are_equal(grades_(i),1)){ 
			average_ += grades_(i);
			average_grades_with_bonus_ += grades_with_bonus_(i);
			average_final_ += grades_final_(i);
			n_valid++;
			if(grades_final_(i)<4){ nfails_++; }
		}
	}
	average_ /= n_valid;
	average_grades_with_bonus_ /= n_valid;
	average_final_ /= n_valid;
}

void Examen::display(){
	for(unsigned int i(0);i<class_list_.size();i++){
		std::cout<<class_list_[i]<<" "<<points_bonus_(i)<<" ";
		for(unsigned int j(0);j<points_.col();j++){
			std::cout<<points_(i,j)<<" ";
		}
		std::cout<<grades_(i)<<" "<<grades_with_bonus_(i)<<" "<<grades_final_(i)<<std::endl;
	}
	std::cout<<average_<<" "<<average_grades_with_bonus_<<" "<<average_final_<<std::endl;
}

void Examen::edit(){
	std::string nclass(class_id_+".txt");
	std::string npoints("points-"+class_id_+".txt");
	std::string nbonus("bonus-"+class_id_+".txt");
	std::string ndata("data-"+class_id_+".txt");
	Linux command;
	{
		IOFiles fclass(nclass,true,false);
		for(auto& name:class_list_){ fclass<<name<<IOFiles::endl; }

		IOFiles fpoints(npoints,true,false);
		fpoints<<points_;

		IOFiles fbonus(nbonus,true,false);
		for(unsigned int i(0);i<points_bonus_.size();i++){ fbonus<<points_bonus_(i)<<IOFiles::endl; }

		IOFiles fdata(ndata,true,false);
		fdata<<"date="<<date_<<IOFiles::endl;
		fdata<<"max points="<<max_points_<<IOFiles::endl;
		fdata<<"bonus coefficient="<<bonus_coef_<<IOFiles::endl;
		fdata<<"bonus point="<<bonus_test_<<IOFiles::endl;

		command("vim "+ ndata+" -c 'sp "+npoints+" |vsp "+nbonus+" |vsp "+nclass+"'",false);
	}

	{
		IOFiles fpoints(npoints,false,false);
		fpoints>>points_;

		IOFiles fbonus(nbonus,false,false);
		fbonus>>points_bonus_;

		IOFiles fdata(ndata,false,false);
		std::string txt;
		fdata>>txt;
		std::vector<std::string> options(my::string_split(txt,'\n'));
		for(auto& o:options){
			std::vector<std::string> p(my::string_split(o,'='));
			if(p.size()==2){
				if(p[0] == "date"){ date_ = p[1]; }
				else if(p[0] == "bonus coefficient"){ bonus_coef_ = my::string2type<double>(p[1]); }
				else if(p[0] == "bonus point"){ bonus_test_ = my::string2type<double>(p[1]); }
				else if(p[0] == "max points"){ 
					std::vector<std::string> q(my::string_split(p[1],' '));
					max_points_.set(q.size());
					for(unsigned int i(0);i<q.size();i++){
						max_points_(i) = my::string2type<unsigned int>(q[i]);
					}
				}
				else { std::cerr<<__PRETTY_FUNCTION__<<" : option not found"<<std::endl; }
			} else {
				std::cerr<<__PRETTY_FUNCTION__<<" : the file is not correctly parsed"<<std::endl;
			}
		}
	}
	command("rm " + ndata+" "+npoints+" "+nbonus+" "+nclass,false);
}

void Examen::save(){
	IOFiles tmp("./exa/"+Time().date("-")+"-"+class_id_+".jdbin",true,false);
	tmp.write("Classe",class_id_);
	tmp.write("Date",date_);
	tmp.write("Bonus points",bonus_test_);
	tmp.write("Bonus coefficient",bonus_coef_);
	tmp.write("Average",average_);
	tmp.write("Average (bonus)",average_grades_with_bonus_);
	tmp.write("Average (final)",average_final_);
	tmp.write("Échecs",nfails_);
	tmp<<points_<<max_points_<<points_bonus_<<grades_<<grades_with_bonus_<<grades_final_;

	tmp<<(unsigned int)(class_list_.size());
	for(auto& name:class_list_){ tmp<<name; }
}

void Examen::histogram(){
	Vector<double> bins(11,0);
	for(unsigned int i(0);i<grades_final_.size();i++){
		if(!my::are_equal(grades_(i),1)){ bins(grades_final_(i)*2-2)++; }
	}

	IOFiles ffrequencies("histogram-"+class_id_+".dat",true,false);
	for(unsigned int i(0);i<bins.size();i++){
		ffrequencies<<1.0+0.5*i<<" "<<bins(i)<<IOFiles::endl;
	}
	Gnuplot histogram("./","histogram-"+class_id_);
	histogram += "set style data histogram";
	histogram.tics("y",1);
	histogram += "plot 'histogram-"+class_id_+".dat' using 2:xtic(1) t 'Notes'";
	histogram.save_file();
	histogram.create_image(true);
}

void Examen::summary(){
	IOFiles latex(class_id_+"-summary.tex",true,false);
	latex<<"\\documentclass{article}"<<IOFiles::endl;
	//latex<<"\\usepackage[a4paper,landscape,twocolumn,margin=1cm]{geometry}"<<IOFiles::endl;
	latex<<"\\usepackage[a4paper,margin=1cm]{geometry}"<<IOFiles::endl;
	latex<<"\\usepackage[frenchb]{babel}"<<IOFiles::endl;
	latex<<"\\usepackage[T1]{fontenc}"<<IOFiles::endl;
	latex<<"\\usepackage[utf8]{inputenc}"<<IOFiles::endl;
	latex<<"\\usepackage{amsmath}"<<IOFiles::endl;
	latex<<"\\usepackage{siunitx}"<<IOFiles::endl;
	latex<<"\\usepackage{graphics}"<<IOFiles::endl;
	latex<<"\\usepackage[table]{xcolor}"<<IOFiles::endl;
	latex<<"\\pagenumbering{gobble}"<<IOFiles::endl;
	latex<<"\\begin{document}"<<IOFiles::endl;
	latex<<"\\section*{"<<class_id_<<": Examen du "+date_+"}"<<IOFiles::endl;
	latex<<"\\begin{itemize}"<<IOFiles::endl;
	latex<<"\\item Nombre total de points: "<<max_points_.sum()<<IOFiles::endl;
	latex<<"\\item Points de bonus: "<<bonus_test_<<IOFiles::endl;
	latex<<"\\item Coefficient bonus: "<<bonus_coef_<<IOFiles::endl;
	latex<<"\\item Échecs: "<<nfails_<<IOFiles::endl;
	latex<<"\\item Note non arrondie: $\\dfrac{5}{"<<max_points_.sum()<<"-"<<bonus_test_<<"}(\\text{points})+"<<bonus_coef_<<"(\\text{bonus})+1$"<<IOFiles::endl;
	latex<<"\\end{itemize}"<<IOFiles::endl;
	latex<<"\\vfill"<<IOFiles::endl;
	latex<<"\\begin{center}"<<IOFiles::endl;
	latex<<"\\begin{tabular}{l||S[table-format=-1.1]|";
	for(unsigned int i(0);i<points_.col();i++){ latex<<"|S[table-format=-1.1]"; }
	latex<<"||S[table-format=-1.3]|S[table-format=-1.3]|S[table-format=-1.3]||}"<<IOFiles::endl;;
	latex<<"Nom & {Bonus} & \\multicolumn{"<<points_.col()<<"}{c||}{Points} & \\multicolumn{3}{c||}{Notes} \\\\\\hline\\hline"<<IOFiles::endl;;
	for(unsigned int i(0);i<class_list_.size();i++){
		if(i%2){ latex<<"\\rowcolor{gray!30}"<<IOFiles::endl; }
		latex<<class_list_[i]<<" &" << points_bonus_(i)<<" &";
		if(!my::are_equal(grades_(i),1)){ 
			for(unsigned int j(0);j<points_.col();j++){ latex<<points_(i,j)<<" &"; }
			latex<<my::round_nearest(grades_(i),1000)<<" &";
			latex<<my::round_nearest(grades_with_bonus_(i),1000)<<" &";
			latex<<grades_final_(i);
		} else {
			for(unsigned int j(0);j<points_.col()+2;j++){ latex<<" &"; }
		}
		latex<<"\\\\"<<IOFiles::endl; 
	}
	latex<<"\\hline\\hline"<<IOFiles::endl;
	latex<<"\\multicolumn{"<<points_.col()+2<<"}{r||}{Moyennes}&"<<my::round_nearest(average_,1000)<<" &"<<my::round_nearest(average_grades_with_bonus_,1000)<<" &"<<my::round_nearest(average_final_,1000)<<IOFiles::endl;
	latex<<"\\end{tabular}"<<IOFiles::endl;
	latex<<"\\end{center}"<<IOFiles::endl;
	latex<<"\\vfill"<<IOFiles::endl;
	latex<<"\\begin{center}"<<IOFiles::endl;
	latex<<"\\includegraphics{histogram-"+class_id_+"}"<<IOFiles::endl;
	latex<<"\\end{center}"<<IOFiles::endl;
	latex<<"\\end{document}"<<IOFiles::endl;

	Linux command;
	command(Linux::pdflatex("./",class_id_+"-summary"),true);
}

void Examen::clean(){
	Linux command;
	command("rm histogram-" +class_id_ + "* "+class_id_+"-summary.tex",false);
	command("rm *.aux *.log",true);
}
