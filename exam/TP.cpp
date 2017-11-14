#include "TP.hpp"

TP::TP(Parseur& P):
	Note(P)
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
		{
			command("vim " + class_id_ +"-data.txt",false);
			std::fstream file(class_id_+"-data.txt",std::ios::in);
			while(std::getline(file,tmps)){ tmp.push_back(tmps); }
		}
		m = tmp.size();
		max_points_.set(m);
		criteria_.set(m);
		std::vector<std::string> point_criterium;
		for(unsigned int i(0);i<m;i++){
			point_criterium = my::string_split(tmp[i],'=');
			max_points_(i) = my::string2type<double>(point_criterium[0]);
			criteria_(i) = point_criterium[1];
		}

		for(unsigned int i(0);i<criteria_.size();i++){
			std::cout<<max_points_(i)<<" : "<<criteria_(i)<<std::endl;
		}
		std::cout<<max_points_.sum()<<" points au total"<<std::endl;
	} while ( !my::get_yn("Les critères sont-ils corrects ?") );

	comments_.set(n,m,"");
	points_.set(n,m,-1.0);
	grades_.set(n,1.0);
}

TP::TP(IOFiles& ftp):
	Note(ftp),
	criteria_(ftp.read<VectorOfStrings>()),
	comments_(ftp.read<ArrayOfStrings>())
{}

void TP::analyse(){
	nfails_ = 0;
	average_ = 0.0;
	nvalid_ = 0;
	unsigned int total_points(max_points_.sum());
	for(unsigned int i(0);i<grades_.size();i++){
		grades_(i) = 0;
		for(unsigned int j(0);j<points_.col();j++){ grades_(i) += points_(i,j); }
		if(grades_(i) >= 0){
			nvalid_++;
			grades_(i) = my::round_nearest(5.0*grades_(i)/total_points+1,2);
			if(grades_(i)<4){ nfails_++; }
			average_ += grades_(i);
		} else {
			for(unsigned int j(0);j<points_.col();j++){ points_(i,j) = -1.0; }
			grades_(i) = 0;
		}
	}
	average_ /= nvalid_;
}

void TP::display(VectorOfStrings const& class_list){
	for(unsigned int i(0);i<class_list.size();i++){
		std::cout<<i<<". "<<class_list(i);
		if(grades_(i)>0.0){ std::cout<<" -> "<<grades_(i); }
		std::cout<<std::endl;
	}
}

void TP::save(IOFiles& w){
	Note::save(w);
	w<<criteria_<<comments_;
}

void TP::add(){
	//display();
	unsigned int student(pick_student());

	for(unsigned int i(0);i<comments_.col();i++){
		std::cout<<criteria_(i)<<" : ";
		std::getline(std::cin,comments_(student,i));
		points_(student,i) = my::round_nearest(my::get_number("% réussite (max "+my::tostring(max_points_(i))+")",0.0,1.0)*max_points_(i),20);
	}
	edit(student);
}

void TP::edit(unsigned int student){
	if(student > class_list_.size()){
		//display();
		student = pick_student();
	}

	std::string ncriteria("criteria.txt");
	std::string npoints("points.txt");
	std::string ncomments("comments.txt");
	{
		IOFiles fcriteria(ncriteria,true,false);
		IOFiles fpoints(npoints,true,false);
		IOFiles fcomments(ncomments,true,false);
		for(unsigned int i(0);i<comments_.col();i++){
			fcriteria<<criteria_(i)<<IOFiles::endl;
			fpoints<<points_(student,i)<<IOFiles::endl;
			fcomments<<comments_(student,i)<<IOFiles::endl;
		}
	}
	Linux command;
	command("vim -O "+ ncriteria+"  "+npoints+" "+ncomments,false);

	{
		IOFiles fpoints(npoints,false,false);
		IOFiles fcomments(ncomments,false,false);
		for(unsigned int i(0);i<comments_.col();i++){
			fpoints>>points_(student,i);
			if(points_(student,i) > max_points_(i)){
				std::cerr<<__PRETTY_FUNCTION__<<" : can't give more point than the maximum"<<std::endl;
				points_(student,i) = max_points_(i);
			}
			comments_(student,i) = fcomments.getline();
		}

	}
	command("rm " + ncriteria+" "+npoints+" "+ncomments,false);
}

void TP::summary(Latex& latex, std::string const& class_id, VectorOfStrings const& class_list){
	analyse();

	std::string tmp("{l||S[table-format=-1.1]|");
	for(unsigned int i(0);i<points_.col();i++){ tmp += "|S[table-format=0.2]"; }
	tmp +="||S[table-format=1.3]||}";;

	latex.command("subsection*{" + class_id + ": " + title_ + "}");
	latex.begin("center");
	latex.begin("tabular",tmp);
	latex+="Nom & \\multicolumn{" + my::tostring(points_.col()) + "}{c||}{Points} & {Notes} \\\\\\hline\\hline";
	for(unsigned int i(0);i<class_list.size();i++){
		tmp = "";
		if(i%2){ latex += "\\rowcolor{gray!30}"; }
		latex += my::tostring(class_list(i)) + " &";
		if(grades_(i)>0){
			for(unsigned int j(0);j<points_.col();j++){ tmp += my::tostring(points_(i,j)) + " &"; }
			tmp += my::tostring(grades_(i));
		} else {
			for(unsigned int j(0);j<points_.col();j++){ tmp += " &"; }
		}
		latex+=tmp+"\\\\";
	}
	latex+="\\hline\\hline";
	latex+="\\multicolumn{" + my::tostring(points_.col()+1) + "}{r||}{Moyennes}&" + my::tostring(my::round_nearest(average_,1000));
	latex.end("tabular");
	latex.end("center");
	latex+="\\vfill";
	latex.begin("center");
	latex+="\\includegraphics{"+ histogram(grades_,1,6,0.5,"TP") +"}";
	latex.end("center");
}

void TP::feedback(std::string const& class_id, VectorOfStrings const& class_list){
	IOFiles latex(class_id+"-feedback-TP.tex",true,false);
	latex<<"\\documentclass{article}"<<IOFiles::endl;
	latex<<"\\usepackage[a4paper,margin=1cm]{geometry}"<<IOFiles::endl;
	latex<<"\\usepackage[frenchb]{babel}"<<IOFiles::endl;
	latex<<"\\usepackage[T1]{fontenc}"<<IOFiles::endl;
	latex<<"\\usepackage[utf8]{inputenc}"<<IOFiles::endl;
	latex<<"\\usepackage{amsmath}"<<IOFiles::endl;
	latex<<"\\usepackage{siunitx}"<<IOFiles::endl;
	latex<<"\\pagenumbering{gobble}"<<IOFiles::endl;
	latex<<"\\begin{document}"<<IOFiles::endl;
	for(unsigned int i(0);i<class_list.size();i++){
		if(!my::are_equal(grades_(i),1)){
			double total_points(0);
			for(unsigned int j(0);j<comments_.col();j++){ total_points += points_(i,j); }
			latex<<"\\section*{"<<class_list(i)<<"}"<<IOFiles::endl;
			latex<<"\\subsection*{"<<title_<<" $\\longrightarrow$ Note : "<<grades_(i)<<" ("<<total_points<<"/5)}"<<IOFiles::endl;
			latex<<"\\begin{center}"<<IOFiles::endl;
			latex<<"\\begin{tabular}{l|l|m{9cm}||}";
			latex<<"Critères & Points & Commentaires \\\\\\hline\\hline"<<IOFiles::endl;
			for(unsigned int j(0);j<comments_.col();j++){
				latex<<criteria_(j)<<" &"<<points_(i,j)<<"/"<<max_points_(j)<<" &"<<comments_(i,j)<<"\\\\\\hline"<<IOFiles::endl;
			}
			latex<<"\\end{tabular}"<<IOFiles::endl;
			latex<<"\\end{center}"<<IOFiles::endl;
		}
	}
	latex<<"\\end{document}"<<IOFiles::endl;

	Linux command;
	command(Linux::pdflatex("./",class_id+"-feedback-TP"),true);
}
