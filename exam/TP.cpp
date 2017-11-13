#include "TP.hpp"

TP::TP(Parseur& P):
	class_id_(P.get<std::string>("class")),
	title_(my::get_string("Titre")),
	average_(0),
	nfails_(0)
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
	points_.set(n,m,0.0);
	grades_.set(n,1.0);
}

TP::TP(IOFiles& ftp):
	class_id_(ftp.read<std::string>()),
	title_(ftp.read<std::string>()),
	class_list_(ftp.read<VectorOfStrings>()),
	points_(ftp.read<Matrix<double> >()),
	max_points_(ftp.read<Vector<double> >()),
	grades_(ftp.read<Vector<double> >()),
	average_(ftp.read<double>()),
	nfails_(ftp.read<unsigned int>()),
	criteria_(ftp.read<VectorOfStrings>()),
	comments_(ftp.read<ArrayOfStrings>())
{}

void TP::analyse(){
	nfails_ = 0;
	average_ = 0.0;
	unsigned int n_valid(0);
	unsigned int total_points(max_points_.sum());
	for(unsigned int i(0);i<grades_.size();i++){
		grades_(i) = 0;
		for(unsigned int j(0);j<points_.col();j++){ grades_(i) += points_(i,j); }
		grades_(i) = my::round_nearest(5.0*grades_(i)/total_points+1,2);

		if(!my::are_equal(grades_(i),1)){
			average_ += grades_(i);
			n_valid++;
			if(grades_(i)<4){ nfails_++; }
		}
	}
	average_ /= n_valid;
}

void TP::display(){
	for(unsigned int i(0);i<class_list_.size();i++){
		std::cout<<i<<". "<<class_list_(i);
		if(!my::are_equal(grades_(i),1.0)){ std::cout<<" -> "<<grades_(i); }
		std::cout<<std::endl;
	}
}

void TP::save(){
	IOFiles tmp("./tp/"+class_id_+"-TP-"+Time().date("-")+".jdbin",true,false);
	tmp.write("Classe",class_id_);
	tmp.write("Titre",title_);
	tmp<<class_list_<<points_<<max_points_<<grades_;
	tmp.write("Moyenne",average_);
	tmp.write("Échecs",nfails_);

	tmp<<criteria_<<comments_;
}

unsigned int TP::pick_student(){
	unsigned int s(my::get_number("Choisir l'étudiant :", (unsigned int)(0), class_list_.size()-1));
	std::cout<<"Etudiant sélectionné : "<<class_list_(s)<<std::endl;
	return s;
}

void TP::add(){
	display();
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
		display();
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

void TP::histogram(){
	Vector<double> bins(11,0);
	for(unsigned int i(0);i<grades_.size();i++){
		if(!my::are_equal(grades_(i),1)){ bins(grades_(i)*2-2)++; }
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

void TP::summary(){
	analyse();
	histogram();
	IOFiles latex(class_id_+"-summary-TP.tex",true,false);
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
	latex<<"\\begin{center}"<<IOFiles::endl;
	latex<<"\\begin{tabular}{l|";
	for(unsigned int i(0);i<points_.col();i++){ latex<<"|S[table-format=0.2]"; }
	latex<<"||S[table-format=1.3]||}"<<IOFiles::endl;;
	latex<<"Nom & \\multicolumn{"<<points_.col()<<"}{c||}{Points} & {Notes} \\\\\\hline\\hline"<<IOFiles::endl;;
	for(unsigned int i(0);i<class_list_.size();i++){
		if(i%2){ latex<<"\\rowcolor{gray!30}"<<IOFiles::endl; }
		latex<<class_list_(i)<<" &";
		if(!my::are_equal(grades_(i),1)){
			for(unsigned int j(0);j<points_.col();j++){ latex<<points_(i,j)<<" &"; }
			latex<<grades_(i);
		} else {
			for(unsigned int j(0);j<points_.col();j++){ latex<<" &"; }
		}
		latex<<"\\\\"<<IOFiles::endl;
	}
	latex<<"\\hline\\hline"<<IOFiles::endl;
	latex<<"\\multicolumn{"<<points_.col()+1<<"}{r||}{Moyennes}&"<<my::round_nearest(average_,1000)<<IOFiles::endl;

	latex<<"\\end{tabular}"<<IOFiles::endl;
	latex<<"\\end{center}"<<IOFiles::endl;
	latex<<"\\vfill"<<IOFiles::endl;
	latex<<"\\begin{center}"<<IOFiles::endl;
	latex<<"\\includegraphics{histogram-"+class_id_+"}"<<IOFiles::endl;
	latex<<"\\end{center}"<<IOFiles::endl;
	latex<<"\\end{document}"<<IOFiles::endl;

	Linux command;
	command(Linux::pdflatex("./",class_id_+"-summary-TP"),true);
}

void TP::feedback(){
	IOFiles latex(class_id_+"-feedback-TP.tex",true,false);
	latex<<"\\documentclass{article}"<<IOFiles::endl;
	latex<<"\\usepackage[a4paper,margin=1cm]{geometry}"<<IOFiles::endl;
	latex<<"\\usepackage[frenchb]{babel}"<<IOFiles::endl;
	latex<<"\\usepackage[T1]{fontenc}"<<IOFiles::endl;
	latex<<"\\usepackage[utf8]{inputenc}"<<IOFiles::endl;
	latex<<"\\usepackage{amsmath}"<<IOFiles::endl;
	latex<<"\\usepackage{siunitx}"<<IOFiles::endl;
	latex<<"\\pagenumbering{gobble}"<<IOFiles::endl;
	latex<<"\\begin{document}"<<IOFiles::endl;
	for(unsigned int i(0);i<class_list_.size();i++){
		if(!my::are_equal(grades_(i),1)){
			double total_points(0);
			for(unsigned int j(0);j<comments_.col();j++){ total_points += points_(i,j); }
			latex<<"\\section*{"<<class_list_(i)<<"}"<<IOFiles::endl;
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
	command(Linux::pdflatex("./",class_id_+"-feedback-TP"),true);
}

void TP::clean(){
	Linux command;
	command("rm histogram-" +class_id_ + "* "+class_id_+"-summary-TP.tex "+class_id_+"-feedback-TP.tex",false);
	command("rm *.aux *.log",true);
}
