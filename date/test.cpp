#include "Date.hpp"
#include "Rand.hpp"
#include <algorithm>

bool sort(Date const& da, Date const& db){ return (da<db); }

int main(){
	//Date d;
	//std::cout<<d.day()<<" "<<d.month()<<" "<<d.year()<<std::endl;
	//std::cout<<d<<std::endl;
	//d.set(29,2,2019);
	//std::cout<<d<<std::endl;
	//d.set(29,2,2020);
	//std::cout<<d<<std::endl;
	//{
		//IOFiles w("test.jdbin",true,false);
		//w<<d;
	//}
	//{
		//IOFiles r("test.jdbin",false,false);
		//Date d2;
		//r>>d2;
		//std::cout<<d2.ddmmyyyy('/')<<std::endl;
	//}
//
	std::vector<Date> list(20);
	Rand<unsigned int> rnd_day(1,25);
	Rand<unsigned int> rnd_month(1,12);
	Rand<unsigned int> rnd_year(2000,2005);
	for(auto& l:list){
		l.set(rnd_day(),rnd_month(),rnd_year());
	}
	std::sort(list.begin(),list.end(),sort);
	for(auto& l:list){ std::cout<<l<<std::endl; }
}
