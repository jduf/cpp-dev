#include "Vector.hpp"
#include "Rand.hpp"
#include <ncurses.h>
#include <form.h>
#include <regex>

//int main(){
	//std::vector<std::string> list;
	//list.push_back("Jérôme Dufour");
	//list.push_back("Anika Clausen");
	//list.push_back("Camille Fuchs");
	//list.push_back("Guillaume Granges");
	//list.push_back("Daniel Dufour");
	//list.push_back("Florian Aubry");
	//list.push_back("Nathaniel Coupy");
	//list.push_back("Géraldine Ulrich");
	//list.push_back("Jacqueline Pirszel");
//
	//unsigned int choix(my::ncurses::select_from_list(list,"Choisir la personne :",false));
	//if(choix<list.size()){ std::cout<<"mon choix est : "<<list[choix]<<std::endl; }
//}

unsigned int nfw(4);

void set_window_frame(WINDOW* win, std::string const& msg, bool const& highlight){
	if(highlight){
		wattron(win,COLOR_PAIR(1));
		box(win,0,0);
		wattroff(win,COLOR_PAIR(1));
	} else {
		box(win,0,0);
	}
	mvwprintw(win,0,0,msg.c_str());
}

void display_help(WINDOW* win, unsigned int what){
	wclear(win);
	switch(what){
		case 0:
		case 1:
		case 2:
			mvwprintw(win, 0, 2, "Select : Type in what you want to selct");
			mvwprintw(win, 1, 2, "      or Use the arrow keys to select");
			mvwprintw(win, 2, 2, "Esc    : Exit");
			break;
		case 3:
			mvwprintw(win, 0, 2, "Enter the points");
	}
	wnoutrefresh(win);
}

bool select(std::vector<std::string> const& list, std::vector<unsigned int>& focus, std::vector<WINDOW*> winlist, std::vector<std::string> const& msglist, unsigned int& selected){
	unsigned int ih(focus.back()==2?0:1);
	unsigned int iw(focus.back()==2?10:0);

	WINDOW* win(winlist[focus.back()]);
	std::string msg(msglist[focus.back()]);

	unsigned int nsel(list.size());
	std::vector<unsigned int> sel(nsel);
	for(unsigned int i(0); i<nsel;i++) { sel[i] = i; }

	std::string cmp("");
	unsigned int highlight(0);
	unsigned int in(0);
	bool keepon(true);
	bool search(true);
	while(keepon){
		if(cmp == ""){ display_help(winlist[4],focus.back()); }
		wclear(win);
		set_window_frame(win,msg,true);
		for(unsigned int i(0); i<list.size();i++){
			if(i == sel[highlight]){
				wattron(win, A_REVERSE);
				mvwprintw(win, 2+i*ih, 2+i*iw, list[i].c_str());
				wattroff(win, A_REVERSE);
			} else {
				mvwprintw(win, 2+i*ih, 2+i*iw, list[i].c_str());
			}
		}
		wnoutrefresh(win);
		doupdate();

		search = false;
		in = wgetch(winlist[4]);
		switch(in){
			case 9: //TAB
				switch(focus.back()){
					case 1:
						if(focus[1] == 2){ focus.pop_back(); }
						else { focus.push_back(2); }
						keepon = false;
						break;
					case 2:
						if(focus[1] == 1){ focus.pop_back(); }
						else { focus.push_back(1); }
						keepon = false;
						break;
				}
				break;
			case 10: //ENTER
				switch(focus.back()){
					case 0:
						focus.push_back(1);
						break;
					case 1:
					case 2:
						focus.push_back(3);
						break;
				}
				keepon = false;
				break;
			case 27: //ESCAPE
				return false;
				break;
			case 258: //DOWN
			case 261: //RIGHT
				if(highlight+1 == nsel){ highlight = 0; }
				else { ++highlight; }
				break;
			case 260: //LEFT
			case 259: //UP
				if(highlight == 0) { highlight = nsel-1; }
				else { --highlight; }
				break;
			case 263: //BACKSPACE
				if(cmp.size()){
					cmp.pop_back();
					search = true;
				} else {
					if(focus.size()){ focus.pop_back(); }
					keepon = false;
				}
				break;
			case 330://DEL
				break;
			case 353://MAJTAB
				break;
			default:
				if(nsel != 0){
					cmp += in;
					search = true;
				}
				break;
		}

		if(search){
			sel.clear();
			std::regex r(cmp,std::regex_constants::icase);
			std::smatch sm;
			for(unsigned int i(0);i<list.size();i++){
				if(std::regex_search(list[i],sm,r)){ sel.push_back(i); }
			}
			nsel = sel.size();
			highlight = 0;
		}

		wclear(winlist[4]);
		mvwprintw(winlist[4], 0, 2, ( "Select : " + cmp ).c_str());
		for(unsigned int i(0); i<sel.size();i++){
			if(i==3){
				mvwprintw(winlist[4], i+1, 17, "... (other choices)");
				i=sel.size();
			} else {
				mvwprintw(winlist[4], i+1, 17, list[sel[i]].c_str());
			}
		}
		for(unsigned int i(0); i<focus.size();i++){
			mvwprintw(winlist[4],2,i,"%d",focus[i]);
		}
		wnoutrefresh(winlist[4]);
	}
	selected = sel[highlight];

	return true;
}

bool enter_points(unsigned int ss, Vector<double>& sp, std::vector<unsigned int>& focus, std::vector<WINDOW*> winlist, std::vector<std::string> const& msglist){
	unsigned int ih(0);
	unsigned int iw(0);
	unsigned int fw(5);
	unsigned int fx(0);
	unsigned int fy(0);

	display_help(winlist[4], focus.back()); 
	WINDOW* win(winlist[focus.back()]);
	std::string msg(msglist[focus.back()]);

	set_window_frame(win,msg,true);
	curs_set(1);

	WINDOW* win_form;
	if(focus.size()==3){
		iw = 2;
		fx = 1;
		fy = ss+2;
		win_form = derwin(win, 1, sp.size()*fw*2, fy, fx);
	} else {
		ih = 1;
		fx = 2*ss*fw+1;
		fy = 2;
		win_form = derwin(win, sp.size(), fw, fy, fx);
	}

	keypad(win_form,true);
	for(unsigned int i(0); i<focus.size();i++){
		mvwprintw(winlist[4],2,i,"%d",focus[i]);
	}
	mvwprintw(winlist[4],3,0,":%d-%d-%d-%d",ih,iw,sp.size(),ss);
	wnoutrefresh(winlist[4]);
	doupdate();

	std::vector<FIELD*> fields(sp.size()+1);
	for(unsigned int i(0);i<sp.size();i++){
		fields[i] = new_field(1, fw, i*ih, i*iw*fw, 0, 0);
		set_field_opts(fields[i], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE | O_BLANK | O_STATIC);
		set_field_type(fields[i], TYPE_NUMERIC, 2, 0.0, 99.99);
		set_field_back(fields[i], A_UNDERLINE);
		set_field_just(fields[i], JUSTIFY_RIGHT);
		set_field_buffer(fields[i], 0, my::tostring(sp(i)).c_str());
	}
	fields.back() = NULL;

	FORM* form = new_form(&fields[0]);
	set_form_win(form, win);
	set_form_sub(form, win_form);
	post_form(form);

	bool keepon(true);
	unsigned int in(0);
	while(keepon){
		in = wgetch(win);
		switch(in){
			case 9: //TAB
				form_driver(form, REQ_VALIDATION);
				form_driver(form, REQ_NEXT_FIELD);
				break;
				//case KEY_BTAB: //MAJTAB
			case 353: //MAJTAB
				form_driver(form, REQ_VALIDATION);
				form_driver(form, REQ_PREV_FIELD);
				break;
			case 260: //LEFT
			case 259: //UP
				//case KEY_UP: 
				//case KEY_LEFT:
				form_driver(form, REQ_VALIDATION);
				form_driver(form, REQ_PREV_CHAR);
				break;
				//case KEY_DOWN:
				//case KEY_RIGHT:
			case 261: //RIGHT
			case 258: //DOWN
				form_driver(form, REQ_NEXT_CHAR);
				break;
				//case KEY_BACKSPACE:
			case 263:
				form_driver(form, REQ_DEL_PREV);
				break;
				//case KEY_EXIT: //ESC
			case 10: //ENTER
				if(!form_driver(form, REQ_VALIDATION)){
					form_driver(form, REQ_NEXT_FIELD);
					form_driver(form, REQ_PREV_FIELD);
					for(unsigned int i(0);i<sp.size();i++){
						sp(i) = my::string2type<double>(field_buffer(fields[i],0));
					}
					keepon = false;
				}
				break;
			case 27: //ESC
				keepon = false;
				break;
				//case KEY_DC:
			case 330: //DEL
				form_driver(form, REQ_DEL_CHAR);
				break;
			default:
				form_driver(form, in);
		}
	}

	unpost_form(form);
	free_form(form);
	for(auto& f:fields){ free_field(f); }
	delwin(win_form);
	curs_set(0);

	for(unsigned int i(0);i<sp.size();i++){
		mvwprintw(win, fy+i*ih, fx+i*iw*fw,"%5.2f",sp(i));
	}

	focus.pop_back();
	return true;
}

int main(){
	std::vector<std::string> classes;
	classes.push_back("S1CSA");
	classes.push_back("1M4");
	classes.push_back("2M4");
	classes.push_back("2M6");

	std::vector<std::string> students;
	students.push_back("Jérôme Dufour");
	students.push_back("Anika Clausen");
	students.push_back("Camille Fuchs");
	students.push_back("Guillaume Granges");
	students.push_back("Daniel Dufour");
	students.push_back("Florian Aubry");
	students.push_back("Nathaniel Coupy");
	students.push_back("Géraldine Ulrich");
	students.push_back("Jacqueline Pirszel");

	std::vector<std::string> exa;
	exa.push_back("Exa 1");
	exa.push_back("Exa 2");
	exa.push_back("TP 1");

	Matrix<double> points(students.size(),3);
	Vector<double> sp(3);
	Rand<double> rnd(0,20);
	for(unsigned int i(0);i<points.size();i++){
		points.ptr()[i] = my::round_nearest(rnd(),100);
	}

	std::vector<std::string> msglist = {"Classes","Student list","Exa","Note"};

	initscr();
	noecho();
	curs_set(0);
	start_color();
	init_pair(1,COLOR_BLUE,COLOR_BLACK);
	unsigned int cols1(20);
	unsigned int cols2(60);
	unsigned int rows1(5);
	unsigned int rows2(30);

	std::vector<WINDOW*> winlist;
	winlist.push_back(newwin(rows1+rows2, cols1, 0,             0));
	winlist.push_back(newwin(rows2,       cols2, rows1,         cols1+1));
	winlist.push_back(newwin(rows1,       0,     0,             cols1+cols2+2));
	winlist.push_back(newwin(rows2,       0,     rows1,         cols1+cols2+2));
	winlist.push_back(newwin(rows1,       cols2, 0,             cols1+1));

	std::vector<unsigned int> focus(1,0);
	for(unsigned int i(0);i<nfw;i++){ keypad(winlist[i],true); }

	keypad(winlist[4],true);
	bool keepon(true);
	unsigned int selected(0);
	while(keepon){
		switch(focus.back()){
			case 0:
				for(unsigned int i(0);i<nfw;i++){
					wclear(winlist[i]);
					set_window_frame(winlist[i],msglist[i],i==focus.back());
					wnoutrefresh(winlist[i]);
				}
				keepon = select(classes,focus,winlist,msglist,selected);
				if(focus.back()){
					for(unsigned int i(0);i<points.row();i++){
						for(unsigned int j(0);j<points.col();j++){
							mvwprintw(winlist[3],i+2, 1+j*10,"%5.2f",points(i,j));
						}
					}
					wnoutrefresh(winlist[3]);
					set_window_frame(winlist[0],msglist[0],false);
					wnoutrefresh(winlist[0]);
				}
				break;
			case 1:
				for(unsigned int i(0); i<exa.size();i++){
					mvwprintw(winlist[2], 2, 2+i*10, exa[i].c_str());
				}
				wnoutrefresh(winlist[2]);

				keepon = select(students,focus,winlist,msglist,selected);

				set_window_frame(winlist[1],msglist[1],false);
				wnoutrefresh(winlist[1]);
				break;
			case 2:
				for(unsigned int i(0); i<students.size();i++){
					mvwprintw(winlist[1], 2+i, 2, students[i].c_str());
				}
				wnoutrefresh(winlist[1]);

				keepon = select(exa,focus,winlist,msglist,selected);

				set_window_frame(winlist[2],msglist[2],false);
				wnoutrefresh(winlist[2]);
				break;
			case 3:
				if(focus.size() == 3){
					sp.set(points.col());
					for(unsigned int i(0);i<sp.size();i++){ sp(i) = points(selected,i); }
					keepon = enter_points(selected,sp,focus,winlist,msglist);
					for(unsigned int i(0);i<sp.size();i++){ points(selected,i) = sp(i); }
				} else if(focus.size() == 4){
					sp.set(points.row());
					for(unsigned int i(0);i<sp.size();i++){ sp(i) = points(i,selected); }
					keepon = enter_points(selected,sp,focus,winlist,msglist);
					for(unsigned int i(0);i<sp.size();i++){ points(i,selected) = sp(i); }
				}
				set_window_frame(winlist[3],msglist[3],false);
				wnoutrefresh(winlist[3]);
				break;
		}
	}
	endwin();
}
