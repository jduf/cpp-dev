#include "Examen.hpp"

Examen::Examen(Parseur& P):
	Note(P),
	bonus_test_(0),
	bonus_coef_(0)
{
	Linux command;
	std::string tmps;
	std::vector<std::string> tmp;
	unsigned int n;
	unsigned int m;

	do {
		{
			command("vim " + class_id_ +"-list.txt",false);
			std::fstream file(class_id_+"-list.txt",std::ios::in);
			while(std::getline(file,tmps)){ tmp.push_back(tmps); }
		}
		n = tmp.size();
		class_list_.set(n);
		for(unsigned int i(0);i<n;i++){ class_list_(i) = tmp[i]; }
		tmp.clear();
		std::cout<<class_list_<<std::endl;
	} while ( !my::get_yn(my::tostring(class_list_.size())+" élèves enregistrés, est-ce correct?") );

	do {
		std::vector<std::string> mp(my::string_split(my::get_string("Point par exercice"),','));
		m = mp.size();
		max_points_.set(m);
		for(unsigned int i(0);i<m;i++){ max_points_(i) = my::string2type<double>(mp[i]); }
		std::cout<<max_points_.sum()<<" points au total réparti sur "<<m<<" exercices"<<std::endl;
	} while ( !my::get_yn("Les points sont-ils corrects ?") );

	points_.set(n,m,0.0);
	grades_.set(n,0.0);
	grades_test_.set(n,0.0);
	grades_with_bonus_.set(n,0.0);
}

Examen::Examen(IOFiles& fexa):
	Note(fexa),
	bonus_test_(fexa.read<double>()),
	bonus_coef_(fexa.read<double>()),
	average_test_(fexa.read<double>()),
	average_grades_with_bonus_(fexa.read<double>()),
	points_bonus_(fexa.read<Vector<double> >()),
	grades_test_(fexa.read<Vector<double> >()),
	grades_with_bonus_(fexa.read<Vector<double> >())
{}

void Examen::analyse(){
	nfails_ = 0;
	average_test_ = 0.0;
	average_grades_with_bonus_ = 0.0;
	average_ = 0.0;
	unsigned int n_valid(0);
	unsigned int total_points(max_points_.sum());
	for(unsigned int i(0);i<grades_test_.size();i++){
		grades_test_(i) = 0;
		grades_with_bonus_(i) = 0;
		grades_(i) = 0;
		for(unsigned int j(0);j<points_.col();j++){ grades_test_(i) += points_(i,j); }
		if(grades_test_(i) != 0){
			n_valid++;
			grades_test_(i) = std::min(5*grades_test_(i)/(total_points-bonus_test_)+1.0,6.0);
			grades_with_bonus_(i) = std::min(grades_test_(i) + points_bonus_(i)*bonus_coef_,6.0);
			grades_(i) = my::round_nearest(grades_with_bonus_(i),2);
			if(grades_(i)<4){ nfails_++; }

			average_test_ += grades_test_(i);
			average_grades_with_bonus_ += grades_with_bonus_(i);
			average_ += grades_(i);
		}
	}
	average_test_ /= n_valid;
	average_grades_with_bonus_ /= n_valid;
	average_ /= n_valid;
}

void Examen::display(){
	for(unsigned int i(0);i<class_list_.size();i++){
		std::cout<<class_list_(i)<<" "<<points_bonus_(i)<<" ";
		for(unsigned int j(0);j<points_.col();j++){ std::cout<<points_(i,j)<<" "; }
		std::cout<<grades_test_(i)<<" "<<grades_with_bonus_(i)<<" "<<grades_(i)<<std::endl;
	}
	std::cout<<average_test_<<" "<<average_grades_with_bonus_<<" "<<average_<<std::endl;
}

void Examen::edit(){
	std::string nclass(class_id_+".txt");
	std::string npoints("points-"+class_id_+".txt");
	std::string nbonus("bonus-"+class_id_+".txt");
	std::string ndata("data-"+class_id_+".txt");
	Linux command;
	{
		IOFiles fclass(nclass,true,false);
		fclass<<class_list_;

		IOFiles fpoints(npoints,true,false);
		fpoints<<points_;

		IOFiles fbonus(nbonus,true,false);
		for(unsigned int i(0);i<points_bonus_.size();i++){ fbonus<<points_bonus_(i)<<IOFiles::endl; }

		IOFiles fdata(ndata,true,false);
		fdata<<"titre="<<title_<<IOFiles::endl;
		fdata<<"max points="<<max_points_<<IOFiles::endl;
		fdata<<"bonus coefficient="<<bonus_coef_<<IOFiles::endl;
		fdata<<"bonus point="<<bonus_test_<<IOFiles::endl;

	}
	command("vim "+ ndata+" -c 'sp "+npoints+" |vsp "+nbonus+" |vsp "+nclass+"'",false);

	{
		IOFiles fdata(ndata,false,false);
		std::string txt;
		fdata>>txt;
		std::vector<std::string> options(my::string_split(txt,'\n'));
		for(auto& o:options){
			std::vector<std::string> p(my::string_split(o,'='));
			if(p.size()==2){
				if(p[0] == "titre"){ title_ = p[1]; }
				else if(p[0] == "bonus coefficient"){ bonus_coef_ = my::string2type<double>(p[1]); }
				else if(p[0] == "bonus point"){ bonus_test_ = my::string2type<double>(p[1]); }
				else if(p[0] == "max points"){
					std::vector<std::string> q(my::string_split(p[1],' '));
					max_points_.set(q.size());
					for(unsigned int i(0);i<q.size();i++){
						max_points_(i) = my::string2type<unsigned int>(q[i]);
					}
				} else { std::cerr<<__PRETTY_FUNCTION__<<" : option not found"<<std::endl; }
			} else {
				std::cerr<<__PRETTY_FUNCTION__<<" : the file is not correctly parsed"<<std::endl;
			}
		}

		IOFiles fpoints(npoints,false,false);
		points_.set(class_list_.size(),max_points_.size(),0);
		fpoints>>points_;

		IOFiles fbonus(nbonus,false,false);
		points_bonus_.set(class_list_.size(),0);
		fbonus>>points_bonus_;
	}
	command("rm " + ndata+" "+npoints+" "+nbonus+" "+nclass,false);
}

void Examen::save(){
	IOFiles w("./exa/"+Time().date("-")+"-"+class_id_+".jdbin",true,false);
	save(w);
}

void Examen::save(IOFiles& w){
	Note::save(w);
	w.write("Bonus points",bonus_test_);
	w.write("Bonus coefficient",bonus_coef_);
	w.write("Average (test)",average_test_);
	w.write("Average (with bonus)",average_grades_with_bonus_);
	w<<points_bonus_<<grades_test_<<grades_with_bonus_;
}

void Examen::summary(){
	analyse();

	IOFiles latex(class_id_+"-summary.tex",true,false);
	latex<<"\\documentclass{article}"<<IOFiles::endl;
	latex<<"\\usepackage[a4paper,margin=1cm]{geometry}"<<IOFiles::endl;
	latex<<"\\usepackage[frenchb]{babel}"<<IOFiles::endl;
	latex<<"\\usepackage[T1]{fontenc}"<<IOFiles::endl;
	latex<<"\\usepackage[utf8]{inputenc}"<<IOFiles::endl;
	latex<<"\\usepackage{amsmath}"<<IOFiles::endl;
	latex<<"\\usepackage{siunitx}"<<IOFiles::endl;
	latex<<"\\usepackage{graphicx}"<<IOFiles::endl;
	latex<<"\\usepackage[table]{xcolor}"<<IOFiles::endl;
	latex<<"\\pagenumbering{gobble}"<<IOFiles::endl;
	latex<<"\\begin{document}"<<IOFiles::endl;
	latex<<"\\section*{"<<class_id_<<": "<<title_<<"}"<<IOFiles::endl;
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
		latex<<class_list_(i)<<" &" << points_bonus_(i)<<" &";
		if(grades_test_(i)>0){
			for(unsigned int j(0);j<points_.col();j++){ latex<<points_(i,j)<<" &"; }
			latex<<my::round_nearest(grades_test_(i),1000)<<" &";
			latex<<my::round_nearest(grades_with_bonus_(i),1000)<<" &";
			latex<<grades_(i);
		} else {
			for(unsigned int j(0);j<points_.col()+2;j++){ latex<<" &"; }
		}
		latex<<"\\\\"<<IOFiles::endl;
	}
	latex<<"\\hline\\hline"<<IOFiles::endl;
	latex<<"\\multicolumn{"<<points_.col()+2<<"}{r||}{Moyennes}&"<<my::round_nearest(average_test_,1000)<<" &"<<my::round_nearest(average_grades_with_bonus_,1000)<<" &"<<my::round_nearest(average_,1000)<<IOFiles::endl;
	latex<<"\\end{tabular}"<<IOFiles::endl;
	latex<<"\\end{center}"<<IOFiles::endl;
	latex<<"\\vfill"<<IOFiles::endl;
	latex<<"\\begin{center}"<<IOFiles::endl;
	latex<<"\\includegraphics{"+histogram(grades_,1,6,0.5,"Notes")+"}"<<IOFiles::endl;
	latex<<"\\end{center}"<<IOFiles::endl;
	for(unsigned int j(0);j<max_points_.size();j++){
		Vector<double> tmp(grades_.size());
		for(unsigned int i(0);i<tmp.size();i++){ tmp(i) = points_(i,j); }
		latex<<"\\begin{center}"<<IOFiles::endl;
		latex<<"\\includegraphics{"+histogram(tmp,0,max_points_(j),1,"Exo"+my::tostring(j+1))+"}"<<IOFiles::endl; 
		latex<<"\\end{center}"<<IOFiles::endl;
	}
	latex<<"\\end{document}"<<IOFiles::endl;

	Linux command;
	command(Linux::pdflatex("./",class_id_+"-summary"),true);
}
