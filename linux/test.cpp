#include "Linux.hpp" 
#include <iostream>

int main(){
	Linux command;
	std::string dir(command.pwd());
	std::cout<<dir<<std::endl;
	command("ls");
}