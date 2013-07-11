/**
@file test.cpp 
*/

#include "Matrix.hpp"
#include "Lapack.hpp"
#include "Read.hpp"
#include "Write.hpp"

#include <complex>

int main(){
	/*operateurs*/
	///*{*/
	//unsigned int N1_row(3);
	//unsigned int N1_col(2);
	//unsigned int N2_row(2);
	//unsigned int N2_col(6);
	//Matrix<double> m1(N1_row,N1_col);
	//Matrix<double> m2(N2_row,N2_col,2);
	//
	//m1(0,0) = 1;
	//m1(0,1) = 2;
	//m1(1,0) = 3;
	//m1(1,1) = 4;
	//m1(2,0) = 5;
	//m1(2,1) = 6;
	//Matrix<double> m3(m1);
//
	//std::cout<<"m1"<<std::endl;
	//std::cout<<m1<<std::endl;
	//std::cout<<"m2"<<std::endl;
	//std::cout<<m2<<std::endl;
	//std::cout<<"m3"<<std::endl;
	//std::cout<<m3<<std::endl;
	//m1 = m1*m2;
	//std::cout<<"m1=m1*m2"<<std::endl;
	//std::cout<<m1<<std::endl;
	//m2=m2.transpose();
	//std::cout<<"m2=m2.tanspose"<<std::endl;
	//std::cout<<m2<<std::endl;
	//std::cout<<"m1*m2"<<std::endl;
	//std::cout<<m1*m2<<std::endl;
	//std::cout<<"m3-(m1-m2)"<<std::endl;
	//std::cout<<m3-(m1*m2)<<std::endl;
//
	//Matrix<double> m4(1,3,3.1);
	//Matrix<double> m5(3,1,11);
	//
	//std::cout<<"m4*m5"<<std::endl;
	//std::cout<<m5*m4<<std::endl;
	//m4 *= m1;
	//std::cout<<m4<<std::endl;
	//m1.print_mathematica();
	//std::cout<<m1<<std::endl;
	// /*}*/
	/*eigenvalue*/
	///*{*/
	//unsigned int N_site(6);
	//Matrix<double> H(N_site,N_site,0.0);
	//H(0,1)=-1.0;
	//H(0,N_site-1)=1.0;
	//for(unsigned int i(1); i< N_site-1; i++){
		//H(i,i-1) = -1.0;
		//H(i,i+1) = -1.0;
	//}
	//H(N_site-1,0)=1.0;
	//H(N_site-1,N_site-2)=-1.0;
	//std::cout<<"Original matrix"<<std::endl;
	//std::cout<<H<<std::endl;;
	//Matrix<double> T(H);
	//Matrix<double> EValOW(N_site,1);
	//Matrix<double> EValNOW(N_site,1);
//
	//Lapack<double> EVecNOW(&T,true,'S');
	//Lapack<double> EVecOW(&T,false,'S');
	//EVecNOW.eigensystem(EValNOW);
	//EVecOW.eigensystem(EValOW,true);
	//EValNOW.chop();
	//EValOW.chop();
//
	//Matrix<double> Tinv(T); // T is now the passage matrix
	//Lapack<double> Tinv_(&Tinv,false,'G'); Tinv_.inv();
	//Matrix<double> vp(Tinv*H*T);
//
	//std::cout<<"Eigenvalues without overwriting the original matrix"<<std::endl;
	//std::cout<<EValNOW<<std::endl;;
	//std::cout<<"Eigenvalues with overwriting the original matrix"<<std::endl;
	//std::cout<<EValOW<<std::endl;;
	//std::cout<<"Eigenvalues using the passage matrices"<<std::endl;
	//std::cout<<vp.diag()<<std::endl;
//
	//T.chop();
	//std::cout<<"Passage matrix : Tinv.H.T = eigenvalues"<<std::endl;
	//std::cout<<T<<std::endl;;
//
	//Matrix<std::complex<double> > M(3,3);
	//M(0,0) = std::complex<double> (1,0); 
	//M(0,1) = std::complex<double> (2,3); 
	//M(0,2) = std::complex<double> (6,4); 
	//M(1,0) = std::complex<double> (2,-3); 
	//M(1,1) = std::complex<double> (4,0); 
	//M(1,2) = std::complex<double> (-4,-6); 
	//M(2,0) = std::complex<double> (6,-4); 
	//M(2,1) = std::complex<double> (-4,6); 
	//M(2,2) = std::complex<double> (5,0); 
//
	//Lapack<std::complex<double> > M_(&M,false,'H');
	//Matrix<double> EVal2(3,1);
	//M_.eigensystem(EVal2,true);
	//std::cout<<EVal2<<"true eigenvalue " << -7.72113<<" "<<3.4124<<" "<<14.3087<<std::endl;
	//std::cout<<M<<std::endl;;
	//std::cout<<M.trans_conj()<<std::endl;;
	///*}*/
	/*lu et det*/
	///*{*/
	//Matrix<std::complex<double> > C(3,3);
	//C(0,0)=std::complex<double> (-1.0,3.2);
	//C(0,1)=std::complex<double> (-2.0,1.2);
	//C(0,2)=std::complex<double> (3.0,-7.3);
	//C(1,0)=std::complex<double> (-4.0,1.2);
	//C(1,1)=std::complex<double> (-5.0,1.3);
	//C(1,2)=std::complex<double> (6.0,-6.1);
	//C(2,0)=std::complex<double> (-7.0,7.1);
	//C(2,1)=std::complex<double> (-8.0,8.1);
	//C(2,2)=std::complex<double> (8.0,-1);
	//Lapack<std::complex<double> > C_(&C,false,'G');
	//
	//std::cout<<C<<std::endl;;
	//std::cout<<"det=(160.76,-117.943)="<<C_.det()<<std::endl;
//
	//Matrix<double> T(3,3);
	//T(0,0)=-1.0;
	//T(0,1)=2.5;
	//T(0,2)=5;
	//T(1,0)=0;
	//T(1,1)=-12;
	//T(1,2)=-145.42;
	//T(2,0)=	7;
	//T(2,1)=54;
	//T(2,2)=47;
	//Lapack<double> Tdet(&T,true,'G');
	//std::cout<<"det=-9413.53="<<Tdet.det()<<std::endl;
	//Lapack<double> Tlu(&T,true,'G');
	//Matrix<double> L(3,3,0.0),U(3,3,0.0);
	//Tlu.lu(L,U);
//
	//std::cout<<"T"<<std::endl;
	//std::cout<<T<<std::endl;;
	//std::cout<<"L"<<std::endl;
	//std::cout<<L<<std::endl;;
	//std::cout<<"U"<<std::endl;
	//std::cout<<U<<std::endl;;
	//std::cout<<"T=LU"<<std::endl;
	//std::cout<<L*U<<std::endl;;
	///*}*/
	//inverse
	/*{*/
	Matrix<double> T1(3,3);
	T1(0,0)=-1.0;
	T1(0,1)=2.5;
	T1(0,2)=5;
	T1(1,0)=0;
	T1(1,1)=-12;
	T1(1,2)=-145.42;
	T1(2,0)=	7;
	T1(2,1)=54;
	T1(2,2)=47;
	Matrix<double> T1inv(T1);

	std::cout<<"T1 : well defined"<<std::endl;
	std::cout<<T1<<std::endl;
	Lapack<double> Over(&T1inv,false,'G'); // Tinv va être écrasé
	
	Matrix<int> P1(Over.is_singular());
	Over.inv(P1);

	std::cout<<"Tinv"<<std::endl;
	std::cout<<T1inv<<std::endl;

	std::cout<<"I"<<std::endl;
	std::cout<<(T1*T1inv)<<std::endl;;
	std::cout<<"I"<<std::endl;
	std::cout<<(T1inv*T1)<<std::endl;;

	Matrix<double> T2(3,3);
	T2(0,0)=-1.0;
	T2(0,1)=2.5;
	T2(0,2)=2;
	T2(1,0)=0;
	T2(1,1)=-12;
	T2(1,2)=-145.42;
	T2(2,0)=0;
	T2(2,1)=0;
	T2(2,2)=1e-200;

	Lapack<double> Keep(&T2,true,'G'); // T va être conservé
	std::cout<<"T2 looks singular"<<std::endl;
	Matrix<int> P2(Keep.is_singular());
	Keep.inv(P2);
	Matrix<double> T2inv_lapack(Keep.get_mat());

	std::cout<<T2<<std::endl;
	std::cout<<"get Tinv from lapack"<<std::endl;
	std::cout<<T2inv_lapack<<std::endl;
	std::cout<<T2inv_lapack*T2<<std::endl;

	Lapack<double> Keep2(&T2,true,'G'); // T va être conservé
	Keep2.inv();
	std::cout<<"T2 looks singular but will be inverted anyway"<<std::endl;
	Matrix<double> T2inv_lapack2(Keep2.get_mat());

	std::cout<<"get Tinv2 from lapack"<<std::endl;
	std::cout<<T2inv_lapack2<<std::endl;
	std::cout<<T2inv_lapack2*T2<<std::endl;


	//std::cout<<"check that there is no memory leaks"<<std::endl;;
	//for(unsigned int i(0);i<10000000;i++){
		//Lapack<double> Over(&T,true,'G'); 
		//Over.inv();
		//Matrix<double> Tinv_new(Over.get_mat());
		////std::cout<<(Tinv_new*T)<<std::endl;;
	//}
	/*}*/
	//qr factorisation
	///*{*/
	//Matrix<double> T;
	//Read r("../../SUN/dev/src/sim/test_something.jdbin");
	//r>>T;
//
	////std::cout<<T<<std::endl;
	////std::cout<<T*P<<std::endl;
	////Matrix<double> A(T.row(),T.row());
	////Matrix<double> tmp(T*P);
	////for(unsigned int i(0);i<A.row();i++){
		////for(unsigned int j(0);j<A.row();j++){
			////A(i,j) = tmp(i,j);
		////}
	////}
//
	////Lapack<double> tmp_(&A,true,'G');
	////std::cout<<tmp_.det()<<std::endl;
//
//
	//T = T.transpose();
	//Matrix<double>R;
	//Matrix<double>Q;
	//Lapack<double>qr(&T, true, 'G');
	//qr.qr(Q,R,true);
	//Matrix<double>P(qr.get_mat());
	//T.chop();
	//Q.chop();
	//R.chop();
	//T.chop();
//
	////std::cout<<Q<<std::endl;
	////std::cout<<R<<std::endl;
	//std::cout<<P<<std::endl;
	//std::cout<<T<<std::endl;
	//std::cout<<Q*R*P<<std::endl;
	////Matrix<double> ouf(T.col(),T.col());
	////Matrix<double> QR(Q*R);
	////for(unsigned int i(0);i<ouf.row();i++){
		////for(unsigned int j(0);j<ouf.col();j++){
			////ouf(i,j) = QR(i,j);
		////}
	////}
	////Lapack<double> det_(&ouf,false,'G');
	////std::cout<<det_.det()<<std::endl;
//
	//Write wt("T.dat");
	//wt<<T;
	//Write wqr("QR.dat");
	//wqr<<Q*R*P;
	//Write wq("Q.dat");
	//wq<<Q;
	//Write wr("R.dat");
	//wr<<R;
	///*}*/
}
