#include "Combination.hpp"

void test1(){
	unsigned int n(3);
	unsigned int k(2);
	Combination generate(n,k);

	Vector<int> v(n);
	for(unsigned int i(0);i<n;i++){ v(i) = i; }

	unsigned int i(0);
	do{
		std::cout<<v.order(generate.get_combination())<<std::endl;
		i++;
	} while(generate.next());
}

void test2(){
	Vector<int> v(3);
	v(0) = 1;
	v(1) = 2;
	v(2) = 3;

	unsigned int n(6);
	unsigned int k(3);
	Vector<int> b(my::comb(n,k,v));
	std::cout<<b<<std::endl;
}

int main(){
	std::cout<<"---- test1 ----"<<std::endl;
	test1();
	std::cout<<"---- test2 ----"<<std::endl;
	test2();
}
