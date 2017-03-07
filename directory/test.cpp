#include "Directory.hpp"

int main(){
	Directory d;
	d.search_files_ext(".cpp","./../",true,true);
	d.print(std::cout);
}
