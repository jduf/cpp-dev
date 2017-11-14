#include "Examen.hpp"

Examen::Examen(unsigned int const& nstudents):
	Note(nstudents),
	bonus_test_(0),
	bonus_coef_(0),
	points_bonus_(nstudents,0.0),
	grades_test_(nstudents,0.0),
	grades_with_bonus_(nstudents,0.0)
{
	Linux command;
	unsigned int m;
	do {
		std::vector<std::string> mp(my::string_split(my::get_string("Point par exercice"),','));
		m = mp.size();
		max_points_.set(m);
		for(unsigned int i(0);i<m;i++){ max_points_(i) = my::string2type<double>(mp[i]); }
		std::cout<<max_points_.sum()<<" points au total réparti sur "<<m<<" exercices"<<std::endl;
	} while ( !my::get_yn("Les points sont-ils corrects ?") );

	points_.set(nstudents,m,-1.0);
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
	nvalid_ = 0;
	unsigned int total_points(max_points_.sum());
	for(unsigned int i(0);i<grades_test_.size();i++){
		grades_test_(i) = 0;
		for(unsigned int j(0);j<points_.col();j++){ grades_test_(i) += points_(i,j); }
		if(grades_test_(i) >= 0){
			nvalid_++;
			grades_test_(i) = std::min(5*grades_test_(i)/(total_points-bonus_test_)+1.0,6.0);
			grades_with_bonus_(i) = std::min(grades_test_(i) + points_bonus_(i)*bonus_coef_,6.0);
			grades_(i) = my::round_nearest(grades_with_bonus_(i),2);
			if(grades_(i)<4){ nfails_++; }
			average_test_ += grades_test_(i);
			average_grades_with_bonus_ += grades_with_bonus_(i);
			average_ += grades_(i);
		} else {
			for(unsigned int j(0);j<points_.col();j++){ points_(i,j) = -1.0; }
			grades_with_bonus_(i) = 0;
			grades_(i) = 0;
		}
	}
	average_test_ /= nvalid_;
	average_grades_with_bonus_ /= nvalid_;
	average_ /= nvalid_;
}

void Examen::display(VectorOfStrings const& class_list){
	for(unsigned int i(0);i<class_list.size();i++){
		std::cout<<i<<". "<<class_list(i)<<" "<<points_bonus_(i)<<" ";
		for(unsigned int j(0);j<points_.col();j++){ std::cout<<points_(i,j)<<" "; }
		std::cout<<grades_test_(i)<<" "<<grades_with_bonus_(i)<<" "<<grades_(i)<<std::endl;
	}
	std::cout<<average_test_<<" "<<average_grades_with_bonus_<<" "<<average_<<std::endl;
}

void Examen::edit(unsigned int const& student){
	if(student>=grades_.size()){
		std::string tmp;

		tmp = my::get_string("titre ("+title_+")");
		if(tmp != ""){
			title_ = tmp;
			tmp = "";
		}
		tmp = my::get_string("bonus coefficient ("+my::tostring(bonus_coef_)+")");
		if(tmp != ""){
			bonus_coef_ = my::string2type<double>(tmp); 
			tmp = "";
		}
		tmp = my::get_string("bonus point ("+my::tostring(bonus_test_)+")");
		if(tmp != ""){
			bonus_test_ = my::string2type<double>(tmp); 
			tmp = "";
		}
		tmp = "max points (";
		for(unsigned int i(0);i<max_points_.size()-1;i++){
			tmp += my::tostring(max_points_(i))+",";
		}
		tmp += my::tostring(max_points_.back())+")";
		tmp = my::get_string(tmp);
		if(tmp != ""){
			std::vector<std::string> q(my::string_split(tmp,' '));
			max_points_.set(q.size());
			for(unsigned int i(0);i<q.size();i++){
				max_points_(i) = my::string2type<unsigned int>(q[i]);
			}
			tmp = "";
		}
	} else { 
		do {
		double tmp(my::get_number("point de bonus ("+my::tostring(points_bonus_(student))+")",0.0,1.0,false));
		if( tmp >= 0.0 ){ points_bonus_(student) = tmp; }

		for(unsigned int i(0);i<max_points_.size();i++){
			tmp = my::get_number("Exo"+my::tostring(i+1)+" ("+my::tostring(points_(student,i))+")",0.0,max_points_(i),false);
			if( tmp >= 0.0 ){ points_(student,i) = tmp; }
		}

		std::cout<<std::endl;
		std::cout<<"point de bonus : "<<points_bonus_(student) <<std::endl;
		std::cout<<"points au test : ";
		for(unsigned int i(0);i<max_points_.size();i++){
			std::cout<<points_(student,i)<<" ";
		}
		} while (!my::get_yn("Est-ce que les points sont correctement entrés"));
		std::cout<<std::endl;
	}
}

void Examen::save(IOFiles& w){
	Note::save(w);
	w.write("Bonus points",bonus_test_);
	w.write("Bonus coefficient",bonus_coef_);
	w.write("Average (test)",average_test_);
	w.write("Average (with bonus)",average_grades_with_bonus_);
	w<<points_bonus_<<grades_test_<<grades_with_bonus_;
}

void Examen::summary(Latex& latex, std::string const& class_id, VectorOfStrings const& class_list){
	analyse();

	std::string tmp("{l||S[table-format=-1.1]|");
	for(unsigned int i(0);i<points_.col();i++){ tmp += "|S[table-format=-1.1]"; }
	tmp += "||S[table-format=-1.3]|S[table-format=-1.3]|S[table-format=-1.3]||}";

	latex.command("subsection*{"+class_id+": "+title_+"}");
	//latex.begin("itemize");
	//latex.item("Échecs: "+my::tostring(nfails_));
	//latex.end("itemize");
	//latex.command("vfill");
	latex.begin("center");
	latex.begin("tabular",tmp);
	latex+="Nom & {Bonus} & \\multicolumn{" + my::tostring(points_.col()) + "}{c||}{Points} & \\multicolumn{3}{c||}{Notes}\\\\\\hline";
	tmp = "& " + my::tostring(bonus_coef_) + " & ";
	for(unsigned int i(0);i<max_points_.size();i++){ tmp += my::tostring(max_points_(i)) + " &"; }
	latex+= tmp + "\\multicolumn{3}{c|}{$\\dfrac{5}{"+my::tostring(max_points_.sum())+"-"+my::tostring(bonus_test_)+"}(\\text{points})+"+my::tostring(bonus_coef_)+"(\\text{bonus})+1$} \\\\\\hline\\hline";
	for(unsigned int i(0);i<class_list.size();i++){
		if(i%2){ latex+="\\rowcolor{gray!30}"; }
		tmp = class_list(i) + " &" + my::tostring(points_bonus_(i)) + " &";
		if(grades_(i)>0){
			for(unsigned int j(0);j<points_.col();j++){ tmp += my::tostring(points_(i,j)) + " &"; }
			tmp += my::tostring(my::round_nearest(grades_test_(i),1000)) + " &";
			tmp += my::tostring(my::round_nearest(grades_with_bonus_(i),1000)) + " &";
			tmp += my::tostring(grades_(i));
		} else {
			for(unsigned int j(0);j<points_.col()+2;j++){ tmp += " &"; }
		}
		latex+=tmp+"\\\\";
	}
	latex+="\\hline\\hline";
	latex+="\\multicolumn{" + my::tostring(points_.col()+2) + "}{r||}{Moyennes}&" + my::tostring(my::round_nearest(average_test_,1000)) + " &" + my::tostring(my::round_nearest(average_grades_with_bonus_,1000)) + " &" + my::tostring(my::round_nearest(average_,1000));
	latex.end("tabular");
	latex.command("includegraphics[scale=0.7]{"+histogram(grades_,1,6,0.5,class_id,"Test")+"}\\\\");
	for(unsigned int j(0);j<max_points_.size();j++){
		Vector<double> tmp(grades_.size());
		for(unsigned int i(0);i<tmp.size();i++){ tmp(i) = points_(i,j); }
		latex.command("includegraphics[scale=0.5]{"+histogram(tmp,0,max_points_(j),1,class_id,"Exo"+my::tostring(j+1))+"}");
		if(!((j+1)%2)){ latex += "\\\\"; }
	}
	latex.end("center");
}
