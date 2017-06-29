#include "Directory.hpp"

int main(){
	Directory d;
	d.search_files_ext(".cpp","../",true,true,false);
	d.sort();
	d.print(std::cout);
}
