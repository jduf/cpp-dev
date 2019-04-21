#include "Date.hpp"

Date::Date(IOFiles& r):
	valid_(r.read<bool>()),
	day_(r.read<unsigned int>()),
	month_(r.read<unsigned int>()),
	year_(r.read<unsigned int>())
{}

/*i/o methods*/
/*{*/
std::ostream& operator<<(std::ostream& flux, Date const& date){
	flux<<date.ddmmyyyy('.');
	return flux;
}

std::istream& operator>>(std::istream& flux, Date& date){
	std::string input;
	std::getline(flux,input);
	std::vector<std::string> tmp(my::string_split(input,'.'));
	if(tmp.size()==3 && tmp[2].size() == 4){
		date.set(my::string2type<unsigned int>(tmp[0]),my::string2type<unsigned int>(tmp[1]),my::string2type<unsigned int>(tmp[2]));
	} else { date.set(0,0,0,true); }
	flux.clear();
	return flux;
}

IOFiles& operator<<(IOFiles& w, Date const& date){
	if(w.is_binary()){
		w<<date.valid()<<date.day()<<date.month()<<date.year();
	} else { w.stream()<<date; }
	return w;
}

IOFiles& operator>>(IOFiles& r, Date& date){
	if(r.is_binary()){ date = std::move(Date(r)); }
	else { r.stream()>>date; }
	return r;
}
/*}*/

void Date::set(unsigned int const& d, unsigned int const& m, unsigned int const& y, bool const& silent){
	valid_ = false;
	switch(m){
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			month_ = m;
			if(d>0 && d<32){
				day_ = d;
				year_ = y;
				valid_ = true;
			}
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			month_ = m;
			if(d>0 && d<32){
				day_ = d;
				year_ = y;
				valid_ = true;
			}
			break;
		case 2:
			month_ = m;
			unsigned int dm;
			if(y%4){ dm = 28; }
			else if (y%100) { dm = 29; }
			else if (y%400) { dm = 28; }
			else {  dm = 29;  }
			if(d>0 && d<=dm){
				day_ = d;
				year_ = y;
				valid_ = true;
			}
			break;
	}
	if(!valid_ && !silent){ std::cerr<<__PRETTY_FUNCTION__<<" : "<<d<<"."<<m<<"."<<y<<" is not a valid Date"<<std::endl; }
}

std::string Date::ddmmyyyy(char split) const {
	return valid_?
		(day_<10?"0":"") + my::tostring(day_) + split +
		(month_<10?"0":"") + my::tostring(month_) + split +
		my::tostring(year_):
		"Invalid date";
}

std::string Date::yyyymmdd(char split) const {
	return valid_?
		my::tostring(year_) + split +
		(month_<10?"0":"") + my::tostring(month_) + split +
		(day_<10?"0":"") + my::tostring(day_):
		"Invalid date";
}

bool Date::operator<(Date const& d) const {
	if(year_ > d.year_){ return false; }
	if(year_ != d.year_){ return true; }
	if(month_ > d.month_){ return false; }
	if(month_ != d.month_){ return true; }
	return day_ < d.day_;
}

bool Date::operator>(Date const& d) const {
	if(year_ < d.year_){ return false; }
	if(year_ != d.year_){ return true; }
	if(month_ < d.month_){ return false; }
	if(month_ != d.month_){ return true; }
	return day_ > d.day_;
}

bool Date::operator!=(Date const& d) const {
	if(year_ == d.year_){ return false; }
	if(month_ == d.month_){ return false; }
	return day_ != d.day_;
}

bool Date::operator<=(Date const& d) const { return !((*this) > d); }
bool Date::operator>=(Date const& d) const { return !((*this) < d); }
bool Date::operator==(Date const& d) const { return !((*this)!= d); }

