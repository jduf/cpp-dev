#include "Vector.hpp"
#include <ncurses.h>
#include <form.h>
#include <regex>

#include "Rand.hpp"

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

struct Tracking{
	std::vector<unsigned int> action;
	std::vector<unsigned int> focus;
	std::vector<unsigned int> select;
};

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
	wnoutrefresh(win);
}

void display_help(WINDOW* win, unsigned int what){
	wclear(win);
	switch(what){
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
			mvwprintw(win, 1, 2, "Select : Type in what you want to selct");
			mvwprintw(win, 2, 2, "      or Use the arrow keys to select");
			mvwprintw(win, 3, 2, "Esc    : Exit");
			break;
		case 5:
		case 6:
			mvwprintw(win, 1, 2, "Enter the points");
			mvwprintw(win, 2, 2, "Use tabulations to move");
			mvwprintw(win, 3, 2, "Enter : exit and save changes");
			mvwprintw(win, 4, 2, "Esc   : exit and discard changes");
	}
	wnoutrefresh(win);
}

void mvwprintwlist(WINDOW* win, unsigned int const& y, unsigned int const& x, unsigned int const& ih, unsigned int const& iw, std::vector<std::string> const& list){
	for(unsigned int i(0); i<list.size();i++){ mvwprintw(win, y+i*ih, x+i*iw, list[i].c_str()); }
	wnoutrefresh(win);
}

void mvwprintwlist(WINDOW* win, unsigned int const& y, unsigned int const& x, unsigned int const& ih, unsigned int const& iw, std::vector<unsigned int> const& list){
	for(unsigned int i(0); i<list.size();i++){ mvwprintw(win, y+i*ih, x+i*iw, "%d",list[i]); }
	wnoutrefresh(win);
}

bool select(std::vector<std::string> const& list, Tracking& track, std::vector<WINDOW*> winlist, std::vector<std::string> const& msglist, unsigned int const geometry[]){
	unsigned int ih(geometry[0]);
	unsigned int iw(geometry[1]);
	//unsigned int wh(geometry[2]);
	//unsigned int ww(geometry[3]);
	unsigned int fy(geometry[2]);
	unsigned int fx(geometry[3]);
	//unsigned int fw(5);

	WINDOW* win(winlist[track.focus.back()]);
	std::string msg(msglist[track.focus.back()]);

	unsigned int nsel(list.size());
	std::vector<unsigned int> sel(nsel);
	for(unsigned int i(0); i<nsel;i++) { sel[i] = i; }

	std::string cmp("");
	unsigned int highlight(track.select.back());
	unsigned int in(0);
	bool keepon(true);
	bool search(true);
	while(keepon){
		if(cmp == ""){ display_help(winlist[4],track.action.back()); }
		wclear(win);
		set_window_frame(win,msg,true);
		for(unsigned int i(0); i<list.size();i++){
			if(i == sel[highlight]){
				wattron(win, A_BOLD);
				mvwprintw(win, fy+i*ih, fx+i*iw, list[i].c_str());
				wattroff(win, A_BOLD);
			} else {
				mvwprintw(win, fy+i*ih, fx+i*iw, list[i].c_str());
			}
		}
		wnoutrefresh(win);
		doupdate();

		search = false;
		in = wgetch(winlist[4]);
		switch(in){
			case 9: //TAB
				if(track.action.back()<5){
					set_window_frame(winlist[track.focus.back()],msglist[track.focus.back()],false);
					switch(track.action.back()){
						case 1:
							track.action.push_back(2); 
							track.focus.push_back(2);
							keepon = false;
							break;
						case 3:
							track.action.push_back(4); 
							track.focus.push_back(1);
							keepon = false;
							break;
						case 4:
						case 2:
							track.action.pop_back();
							track.focus.pop_back();
							keepon = false;
							break;
					}
					track.select.back() = sel[highlight];
					if(track.select.size()>=track.action.size()){ std::swap(track.select.back(),track.select.end()[-2]); } 
					else { track.select.push_back(0); }
				}
				break;
			case 10: //ENTER
				set_window_frame(win,msg,false);
				switch(track.action.back()){
					case 0:
						track.action.push_back(1);
						track.focus.push_back(1);
						break;
					case 1:
						track.action.push_back(3);
						track.focus.push_back(2);
						break;
					case 2:
						track.action.push_back(7);
						track.focus.push_back(2);
						break;
					case 3:
						track.action.push_back(6);
						track.focus.push_back(3);
						break;
					case 4:
						track.action.push_back(5);
						track.focus.push_back(3);
						break;
				}
				if(track.select.size()>track.action.size()){
					track.select.end()[-2] = track.select.back();
					track.select.back() = sel[highlight];
				} else {
					track.select.back() = sel[highlight];
					track.select.push_back(0);
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
					if(track.action.size()>1){
						set_window_frame(winlist[track.focus.back()],msglist[track.focus.back()],false);
						track.action.pop_back(); 
						track.focus.pop_back();
						track.select.pop_back();
					}
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
		if(sel.size()){
			mvwprintw(winlist[4],1,2,("Select : " + cmp ).c_str());
			for(unsigned int i(0); i<sel.size();i++){
				if(i==3){
					mvwprintw(winlist[4], i+2, 17, "... (other choices)");
					i=sel.size();
				} else {
					mvwprintw(winlist[4], i+2, 17, list[sel[i]].c_str());
				}
			}
		} else { mvwprintw(winlist[4],1,2, ("Select : "+cmp+" (No match found)").c_str() ); }
		wnoutrefresh(winlist[4]);
	}

	return true;
}

bool fill_form(Vector<double>& sp, Tracking& track, std::vector<WINDOW*> winlist, std::vector<std::string> const& msglist, unsigned int const geometry[]){
	display_help(winlist[4], track.action.back()); 

	unsigned int ih(geometry[0]);
	unsigned int iw(geometry[1]);
	unsigned int wh(geometry[2]);
	unsigned int ww(geometry[3]);
	unsigned int fy(geometry[4]);
	unsigned int fx(geometry[5]);
	unsigned int fw(5);

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

	std::string msg(msglist[track.focus.back()]);
	WINDOW* win(winlist[track.focus.back()]);
	set_window_frame(win,msg,true);
	curs_set(1);

	WINDOW* win_form(derwin(win,wh,ww,fy,fx));
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

	set_window_frame(win,msg,false);
	track.action.pop_back();
	track.focus.pop_back();
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
	students.push_back("El Haidaoui-Brunet Jamila");
	students.push_back("Vandermensbrugghe Julie");

	std::vector<std::string> exa;
	exa.push_back("Test 23.11.2017");
	exa.push_back("Test 1.1.2018");
	exa.push_back("TP-01");
	exa.push_back("TP-03");

	Rand<double> rndg(1,6);
	Matrix<double> grades(students.size(),exa.size());
	for(unsigned int i(0);i<grades.size();i++){
		grades.ptr()[i] = my::round_nearest(rndg(),2);
	}

	std::vector<std::string> exo;
	exo.push_back("Exo 1");
	exo.push_back("Exo 2");
	exo.push_back("Exo 3");
	Rand<double> rndp(0,20);
	Matrix<double> points(students.size(),exo.size());
	for(unsigned int i(0);i<points.size();i++){
		points.ptr()[i] = my::round_nearest(rndp(),100);
	}

	std::vector<std::string> msglist = {"Classes","Évaluations","Élèves","Note","Points"};

	setlocale(LC_ALL,"");
	initscr();
	noecho();
	curs_set(0);
	start_color();
	init_pair(1,COLOR_BLUE,COLOR_BLACK);
	unsigned int cols1(15);
	unsigned int cols2(45);
	unsigned int rows1(6);
	unsigned int rows2(30);

	std::vector<WINDOW*> winlist;
	winlist.push_back(newwin(rows1+rows2, cols1, 0,     0));//classes
	winlist.push_back(newwin(rows1,       0,     0,     cols1+cols2+2));//exa/exo
	winlist.push_back(newwin(rows2,       cols2, rows1, cols1+1));//students
	winlist.push_back(newwin(rows2,       0,     rows1, cols1+cols2+2));//grades/points
	winlist.push_back(newwin(rows1,       cols2, 0,     cols1+1));//entry
	winlist.push_back(newwin(7,0, rows1+rows2,0));//debug

	Tracking track;
	track.action.push_back(0);
	track.focus.push_back(0);
	track.select.push_back(0);
	for(unsigned int i(0);i<winlist.size();i++){ keypad(winlist[i],true); }

	Vector<double> sp;
	bool keepon(true);
	unsigned int fw(5);
	unsigned int iw(4);
	unsigned int cs(iw*fw);
	unsigned int const select_vertical_display[] = {1,0,2,2};
	unsigned int const select_horizontal_display[] = {0,cs,2,2};
	unsigned int fill_lign_display[] = {0,iw,1,0,0,1};
	unsigned int fill_column_display[] = {1,0,0,fw,2,0};

	while(keepon){
		if(winlist.size()==6){
			set_window_frame(winlist[5],"Debug",false);
			mvwprintw(winlist[5],2,3,"Action:");
			mvwprintw(winlist[5],3,3,"Focus:");
			mvwprintw(winlist[5],4,3,"Select:");

			mvwprintwlist(winlist[5],2,12,0,1,track.action);
			mvwprintwlist(winlist[5],3,12,0,1,track.focus);
			mvwprintwlist(winlist[5],4,12,0,1,track.select);
			wnoutrefresh(winlist[5]);
			wclear(winlist[5]);
		}
		switch(track.action.back()){
			case 0://select class (->1)
				for(unsigned int i(0);i<nfw;i++){
					wclear(winlist[i]);
					set_window_frame(winlist[i],msglist[i],i==track.action.back());
				}
				keepon = select(classes,track,winlist,msglist,select_vertical_display);
				break;
			case 1://select exa (->3) or switch to student (->2)
				if(track.action.end()[-2]!=2){
					for(unsigned int i(0);i<grades.row();i++){
						for(unsigned int j(0);j<grades.col();j++){
							mvwprintw(winlist[3],i+2,1+j*cs,"%5.2f",grades(i,j));
						}
					}
					wnoutrefresh(winlist[3]);
				}
				mvwprintwlist(winlist[2],2,2,1,0,students);
				keepon = select(exa,track,winlist,msglist,select_horizontal_display);
				if(track.action.back()){ msglist[1] = exa[track.select.end()[-2]]; } 
				else { msglist[1] = "Évaluation"; }
				break;
			case 2://select student (->7) or switch to exa (->1)
				mvwprintwlist(winlist[1],2,2,0,cs,exa);
				keepon = select(students,track,winlist,msglist,select_vertical_display);
				break;
			case 3://select student (->6) or switch to exo (->4)
				wclear(winlist[1]);
				wclear(winlist[3]);
				set_window_frame(winlist[1],msglist[1],false);
				set_window_frame(winlist[3],msglist[3],false);
				mvwprintwlist(winlist[1],2,2,0,cs,exo);
				for(unsigned int i(0);i<points.row();i++){
					for(unsigned int j(0);j<points.col();j++){
						mvwprintw(winlist[3],i+2,1+j*cs,"%5.2f",points(i,j));
					}
				}
				wnoutrefresh(winlist[3]);
				wclear(winlist[1]);
				keepon = select(students,track,winlist,msglist,select_vertical_display);
				break;
			case 4://select exo (->5) or switch to student (->3)
				mvwprintwlist(winlist[2],2,2,1,0,students);
				keepon = select(exo,track,winlist,msglist,select_horizontal_display);
				break;
			case 5:// enter points for exo then -> 4
				track.select.pop_back();
				sp.set(points.row());
				for(unsigned int i(0);i<sp.size();i++){ sp(i) = points(i,track.select.back()); }
				fill_column_display[2] = sp.size();
				fill_column_display[5] = cs*track.select.back()+1;
				keepon = fill_form(sp,track,winlist,msglist,fill_column_display);
				for(unsigned int i(0);i<sp.size();i++){ points(i,track.select.back()) = sp(i); }
				break;
			case 6://enter points for student then -> 3
				track.select.pop_back();
				sp.set(points.col());
				for(unsigned int i(0);i<sp.size();i++){ sp(i) = points(track.select.back(),i); }
				fill_lign_display[3] = cs*sp.size();
				fill_lign_display[4] = track.select.back()+2;
				keepon = fill_form(sp,track,winlist,msglist,fill_lign_display);
				for(unsigned int i(0);i<sp.size();i++){ points(track.select.back(),i) = sp(i); }
				break;
			case 7:// print summary
				mvwprintw(winlist[5],3,0,("should print "+students[track.select.back()]+" summary").c_str());
				track.action.pop_back();
				track.focus.pop_back();
				track.select.pop_back();
				keepon = true;
				break;
		}
	}
	endwin();
}
