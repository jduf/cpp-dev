#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

inline std::vector<std::string> &string_split(std::string const& s, char const& delim, std::vector<std::string>& elems){
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) { elems.push_back(item); }
	return elems;
}

inline std::vector<std::string> string_split(std::string const& s, char const& delim){
	std::vector<std::string> elems;
	string_split(s, delim, elems);
	if(elems.back()==""){ elems.pop_back(); }
	return elems;
}

template<typename Type>
bool string2type(std::string const& s, Type& out){
	std::stringstream ss(s);
	if( ss>>out ){ return true; }
	else {
		std::cerr<<__PRETTY_FUNCTION__<<" incoherent types"<<std::endl;
		return false;
	}
}

template<typename Type>
Type string2type(std::string const& s){
	Type t;
	string2type(s,t);
	return t;
}

void stemX0Y2(char* argv[]){
	std::ifstream in(argv[2]);
	unsigned int c1(string2type<unsigned int>(argv[3]));
	unsigned int c2(string2type<unsigned int>(argv[4]));
	std::vector<std::string> d;
	std::string line;

	while(std::getline(in,line)){
		d = string_split(line,' ');
		std::cout<<d[c1-1]<<" "<<0<<std::endl<<d[c1-1]<<" "<<d[c2-1]<<std::endl<<std::endl;
	}
}

void stemXY1Y2(char* argv[]){
	std::ifstream in(argv[2]);
	unsigned int c(string2type<unsigned int>(argv[3]));
	double y1(string2type<double>(argv[4]));
	double y2(string2type<double>(argv[5]));
	std::vector<std::string> d;
	std::string line;

	while(std::getline(in,line)){
		d = string_split(line,' ');
		std::cout<<d[c-1]<<" "<<y1<<std::endl<<d[c-1]<<" "<<y2<<std::endl<<std::endl;
	}
}

int main(int argc, char* argv[]){
	if(argc>1){
		switch(string2type<unsigned int>(argv[1])){
			case 0:{ stemX0Y2(argv); }break;// stems are (X,0)-(X,Y2) where X,Y are columns in file
			case 1:{ stemXY1Y2(argv); }break; // stems are (X,Y1)-(X,Y2) where X is a clumn in file and Y1,Y2 are constants
			default:{ std::cerr<<"Unknown request"<<std::endl; }break;
		}
	}
}
