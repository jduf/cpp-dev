#ifndef DEF_MISCELLANEOUS
#define DEF_MISCELLANEOUS

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <complex>
#include <cmath>
#include <vector>
#include <algorithm>

namespace my{
	/*bool are_equal(T,T)*/
	/*{*/
	inline bool are_equal(double x, double y, double abs_tol=1e-14, double rel_tol=1e-14){
		double diff(std::abs(x-y));
		x = std::abs(x);
		y = std::abs(y);
		x = (x>y)?x:y;
		return (diff<rel_tol*x) || (diff<abs_tol);
	}

	inline bool are_equal(std::complex<double> const& x, std::complex<double> const& y, double abs_tol=1e-14, double rel_tol=1e-14){
		if(!are_equal(std::abs(x),std::abs(y),abs_tol,rel_tol)){ return false; }
		if(!are_equal(x.real(),y.real(),abs_tol,rel_tol)){ return false; }
		if(!are_equal(x.imag(),y.imag(),abs_tol,rel_tol)){ return false; }
		return true;
	}
	/*}*/

	/*round*/
	/*{*/
	inline double round_nearest(double const& d, unsigned int const decimal_place){
		return roundf(d*decimal_place)/decimal_place;
	}

	inline double round_down(double const& d, unsigned int const decimal_place){
		return floorf(d*decimal_place)/decimal_place;
	}

	inline double round_up(double const& d, unsigned int const decimal_place){
		return ceil(d*decimal_place)/decimal_place;
	}
	/*}*/

	/*tostring*/
	/*{*/
	template<typename Type>
		std::string tostring(Type const& t){
			std::ostringstream s;
			s<<t;
			return s.str();
		}

	template<typename Type>
		std::string tostring(Type const& t, unsigned int const& precision){
			std::ostringstream s;
			s<<std::fixed<<std::setprecision(precision)<<t;
			return s.str();
		}
	/*}*/

	/*string2type*/
	/*{*/
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
			my::string2type(s,t);
			return t;
		}
	/*}*/

	inline double get_double(std::string const& msg){
		std::string token;
		double in;
		do{
			std::cout<<msg<<" [double] ";
			std::getline(std::cin,token);
		} while (!(token.find(' ') == std::string::npos && my::string2type<double>(token,in)));
		return in;
	}

	inline std::string get_string(std::string const& msg){
		std::string token;
		std::cout<<msg<<" [std::string] ";
		std::getline(std::cin,token);
		return token;
	}

	inline bool get_yn(std::string const& msg, std::string const& y="y", std::string const& n="n"){
		std::string token;
		do{
			std::cout<<msg<<" ["<<y<<"/"<<n<<"]";
			std::getline(std::cin,token);
			if(token == y){ return true; }
			if(token == n){ return false; }
		} while(true);
	}

	template<typename Type>
		inline Type get_number(std::string const& msg, Type const& nmin, Type const& nmax, bool const& keepon=true){
			std::string token;
			Type ntoken;
			do{
				std::cout<<msg<<" ["<<nmin<<"-"<<nmax<<"] ";
				std::getline(std::cin,token);
				if(token != ""){
					ntoken = my::string2type<Type>(token);
					if( ntoken >= nmin && ntoken <= nmax){ return ntoken; }
				}
			} while(keepon);
			return nmin-1.0;
		}

	inline std::vector<std::string> &string_split(std::string const& s, char const& delim, std::vector<std::string>& elems){
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) { elems.push_back(item); }
		return elems;
	}

	inline std::vector<std::string> string_split(std::string const& s, char const& delim){
		std::vector<std::string> elems;
		if(s != ""){
			string_split(s, delim, elems);
			if(elems.back()==""){ elems.pop_back(); }
		}
		return elems;
	}

	inline unsigned int count_substring(std::string const& s, std::string const& ss){
		unsigned int n(0);
		unsigned int l(ss.size());
		unsigned int p(0);
		while( (p=s.find(ss,p)+l) != l-1){ n++; }
		return n;
	}

	inline std::string ensure_trailing_slash(std::string const& s){
		if(s.back()=='/'){ return s; }
		else { return s+'/'; }
	}

	/*int to alphabet*/
	inline char int_to_alphabet(unsigned int const& i, bool const& upper_case){
		if(i<26){
			if(upper_case){ return "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[i]; }
			else { return "abcdefghijklmnopqrstuvwxyz"[i]; }
		} else { return '?'; }
	}

	/*sign*/
	/*{*/
	template<typename Type> inline constexpr
		int sign_unsigned(Type x){ return Type(0)<x; }

	template<typename Type> inline constexpr
		int sign_signed(Type x){ return (Type(0)<x)-(x<Type(0)); }

	template<typename Type> inline constexpr
		int sign(Type x) { return std::is_signed<Type>()?sign_signed(x):sign_unsigned(x); }
	/*}*/

	/*double real(T)*/
	/*{*/
	inline double real(double const& x){ return x; }

	inline double real(std::complex<double> const& x){ return std::real(x); }
	/*}*/

	/*double imag(T)*/
	/*{*/
	inline double imag(double const& x){ (void)(x); return 0; }

	inline double imag(std::complex<double> const& x){ return std::imag(x); }
	/*}*/

	/*double square(T)*/
	/*{*/
	template<typename Type>
		inline Type square(Type const& x){ return x*x; }
	/*}*/

	/*double norm_squared(T)*/
	/*{*/
	inline double norm_squared(double const& x){ return x*x; }

	inline double norm_squared(std::complex<double> const& x){ return std::norm(x); }
	/*}*/

	/*double chop(T)*/
	/*{*/
	inline double chop(double const& x, double precision = 1e-10){ return (std::abs(x)<precision?0.0:x); }

	inline std::complex<double> chop(std::complex<double> x, double precision = 1e-10){
		if(std::abs(x.imag()) < precision ){ x.imag(0.0); }
		if(std::abs(x.real()) < precision ){ x.real(0.0); }
		return x;
	}
	/*}*/

	inline unsigned long long gcd(unsigned long long x, unsigned long long y){
		unsigned long long t;
		while(y != 0){
			t = x % y;
			x = y;
			y = t;
		}
		return x;
	}

	inline unsigned long long nCk(unsigned long long n, unsigned long long k){
		if(k > n){ std::cerr<<__PRETTY_FUNCTION__<<" : k > n"<<std::endl; }
		unsigned long long r(1);
		unsigned long long g;
		unsigned long long t;
		for (unsigned long long d(1); d<=k;++d,--n){
			g = gcd(r,d);
			r /= g;
			t = n / (d / g);
			if(r > std::numeric_limits<unsigned long long>::max() / t){
				std::cerr<<__PRETTY_FUNCTION__<<" : other bug"<<std::endl;
			}
			r *= t;
		}
		return r;
	}

	inline void display_progress(double const& step, double const& total, std::string const& msg = " "){
		std::cout<<msg<<100.*step/total<<"%       \r"<<std::flush;
	}

	/*!checks wether a point in inside a polygon (see https://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html) */
	inline bool in_polygon(unsigned int const& nvert, double const* const vertx, double const* const verty, double const& testx, double const& testy){
		unsigned int i(0);
		unsigned int j(0);
		bool c(false);
		for (i = 0, j = nvert-1; i < nvert; j = i++){
			if ( ((verty[i]>testy) != (verty[j]>testy)) && (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) ){ c = !c; }
		}
		return c;
	}

	/*determines if two segments intersect (see from http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect)*/
	/*{*/
	/*!given three colinear points p, q, r, checks if point q lies between 'pr'*/
	inline bool on_segment(double const* p, double const* q, double const* r){
		return (q[0] <= std::max(p[0], r[0]) && q[0] >= std::min(p[0], r[0]) && q[1] <= std::max(p[1], r[1]) && q[1] >= std::min(p[1], r[1]));
	}

	/*{*//*!Find orientation of ordered triplet (p, q, r).
		   The function returns following values
		   0 --> p, q and r are colinear
		   1 --> Clockwise
		   2 --> Counterclockwise
		   See http://www.geeksforgeeks.org/orientation-3-ordered-points/ // for
		   details of below formula.  *//*}*/
	inline int orientation(double const* p, double const* q, double const* r){
		double val((q[1]-p[1])*(r[0]-q[0]) - (q[0]-p[0])*(r[1]-q[1]));
		if(my::are_equal(val,0)){ return 0; }
		return(val>0)?1:2;
	}

	/*!returns true if line segment 'p1q1' and 'p2q2' intersect*/
	inline bool intersect(double const* p1, double const* q1, double const* p2, double const* q2){
		int o1(my::orientation(p1,q1,p2));
		int o2(my::orientation(p1,q1,q2));
		int o3(my::orientation(p2,q2,p1));
		int o4(my::orientation(p2,q2,q1));
		//General case
		if(o1 != o2 && o3 != o4){ return true; }
		//p1, q1 and p2 are colinear and p2 lies on segment p1q1
		if(o1 == 0 && my::on_segment(p1,p2,q1)){ return true; }
		if(o2 == 0 && my::on_segment(p1,q2,q1)){ return true; }
		//p2, q2 and p1 are colinear and p1 lies on segment p2q2
		if(o3 == 0 && my::on_segment(p2,p1,q2)){ return true; }
		if(o4 == 0 && my::on_segment(p2,q1,q2)){ return true; }
		return false;
	}
	/*}*/

	/*!transforms a double into a fration*/
	inline unsigned int to_fraction(double const& x0, unsigned long long& num, unsigned long long& den, double& sign, double const& err = 1e-15){
		sign = my::sign(x0);
		double g(std::abs(x0));
		unsigned long long a(0);
		unsigned long long b(1);
		unsigned long long c(1);
		unsigned long long d(0);
		unsigned long long s;
		unsigned int iter(0);
		do {
			s = std::floor(g);
			num = a + s*c;
			den = b + s*d;
			a = c;
			b = d;
			c = num;
			d = den;
			g = 1.0/(g-s);
			if(err>std::abs(sign*num/den-x0)){ return iter; }
		} while(iter++<1e6);
		std::cerr<<__PRETTY_FUNCTION__<<" : failed to find a fraction for "<<x0<<std::endl;
		return 0;
	}

	/*!seconds to hours, minutes, seconds*/
	inline std::string convert_seconds(unsigned int const& s0){
		std::string elapsed("");
		unsigned int s(s0);
		if(s0>=86400){
			elapsed += my::tostring(s/86400)+"d";
			s = s%86400;
		}
		if(s0>=3600){
			elapsed += my::tostring(s/3600)+"h";
			s = s%3600;
		}
		if(s0>=60){
			elapsed+= my::tostring(s/60)+"m";
			s = s%60;
		}
		elapsed+= my::tostring(s)+"s";
		return elapsed;
	}

	/*!returns string of length str.size() with spaces*/
	inline std::string whitespace(std::string const& s){
		return std::string(s.size(),' ');
	}

	/*!remove all given character from string*/
	inline std::string tr(std::string s, char const& c){
		s.erase(std::remove(s.begin(), s.end(),c), s.end());
		return s;
	}

	/*!replace all given characher from string by another*/
	inline std::string replace_in_string(std::string s, char const& c0, char const& c1){
		std::replace(s.begin(),s.end(),c0,c1);
		return s;
	}

	/*!trim leading and/or trailing space*/
	inline std::string trim(std::string const& s, std::string const& whitespace = " \t"){
		auto const s_begin(s.find_first_not_of(whitespace));
		if (s_begin == std::string::npos){ return ""; }
		return s.substr(s_begin, s.find_last_not_of(whitespace) - s_begin + 1);
	}

	/*!repeat input string n times*/
	inline std::string repeat_string(std::size_t n, std::string const& str){
		if(n==0){ return {}; }
		if(n==1||str.empty()){ return str; }
		if(str.size() == 1){ return std::string(n,str[0]); }

		std::string result(str);
		result.reserve(str.size()*n);
		std::size_t m(2);
		for(; m <= n; m *= 2){ result += result; }
		n -= m/2;

		return result.append(result.c_str(),n*str.size());
	}

	/*!wrap strings*/
	inline std::string wrap(std::string in, unsigned int const& line_width){
		size_t size(in.size());
		size_t start(0);
		size_t found(0);
		while(start+line_width<size){
			found = in.substr(start,line_width).find_last_of(' ');
			if(found != std::string::npos){
				in[start+found] = '\n';
			} else {
				found = in.substr(start).find(' ');
				if(found != std::string::npos){ in[start+found] = '\n'; }
				else { return in; }
			}
			start += found;
		}	
		return in;
	}

	/*!compare string with locale settings (handles é,à,ü...*/
	inline int compare_string_locale(std::string const& s1, std::string const& s2, std::locale const& l=std::locale("fr_CH.UTF-8")){
		return std::use_facet<std::collate<char> >(l).compare(&s1[0], &s1[0] + s1.size(), &s2[0], &s2[0] + s2.size());
	}
}
#endif
