#include "Time.hpp"
#include "Miscellaneous.hpp"

#include<iostream>
#include<unistd.h>

int main(){
	std::cout<<my::convert_seconds(100001)<<std::endl;
	std::cout<<my::convert_seconds(3702)<<std::endl;
	std::cout<<my::convert_seconds(520)<<std::endl;
	std::cout<<my::convert_seconds(60)<<std::endl;
	std::cout<<my::convert_seconds(27)<<std::endl;

	Time t;
	std::cout<<t.day()<<" "<<t.month()<<" "<<t.year()<<" "<<t.hour()<<" "<<t.min()<<" "<<t.sec()<<std::endl;
	std::string d;
	std::cout<<t.date("-")<<std::endl;
	std::cout<<t.day()<<std::endl;
	//unsigned int i(0);
	//do{
		//std::cout<<i<<std::endl;
		//sleep(3);
		//i++;
	//} while (!t.limit_reached(5));
	//std::cout<<t.elapsed()<<std::endl;

	std::cout<<time(0)<<std::endl;
	std::cout<<my::convert_seconds(time(0))<<std::endl;
}
