/*
 * mfGrade.cpp
 *
 *  Created on: Jun 19, 2015
 *      Author: sonbol
 */

#include "mfGrade.h"


using namespace std;



/*******************************************************************
mfGrade()

pConvolution:		It is a vector of unique pointers pointed to mf of each variable. For each variable, there is an a pointer pointed to the vector having
					mf values of the variable.
pNormalize:			It is a vector of unique pointers pointed to the normalize values of mfs. So it has normalized values of what pConvolution is pointing to.
					So it has same size as pConvolution.

Note:

The reason that we have a vector of unique_ptr is that, we know about the number of variables in the input vector (numVariate) but the number of mf for each of the
variable is different and we save it in a vector numOfMF. So, what we can do here is that e.g. for convolution, define a vector to the # of variable where each item
in the vector is a unique_ptr to the mfs of one variable. So, we do not worry about that # of mf is different
 *******************************************************************/
mfGrade::mfGrade():commandLine(),pConvolution(numVariate),pNormalize(numVariate),lengthofVar(numVariate){
//cout<<"mfGrad1"<<endl;
}

mfGrade::~mfGrade(){
//	cout<<"mfGrad2"<<endl;
}
/*******************************************************************
MemFungrade()

Use:				Calculates the membership function grade of each variate for each of their membership functions by using two methods
						convolutionFn() ===> It calculates convolution to get the membership functions
						elliotFn()		===> It normalizes the membership grades.
Out:				Normalized membership grades
Status:				public and called from firingStrength class.
PreConditions:		inputVector and pNormalize are valid
Postconditions:		pNormalize is moved to firingStrength class
Invariant:
 *******************************************************************/
vector<unique_ptr<vector<complex<double> >>> mfGrade:: MemFungrade(unique_ptr<std::vector<double>> const & inputVector, int iter, vector<vector<vector<double>>> *MFparam,vector<int>* dim,int lengthSurod){
	iteration=iter;
	lengthSurodata=lengthSurod;
	/*cout<<"mf params in mfGrade"<<endl;
	for(auto i:*MFparam){
		for (auto j:i){
			for (auto k:j){
				cout << k<< " ";
			}
			cout<<endl;
		}
	}*/
	int j=0;
	for(auto i:*dim){
		lengthofVar[j]=i;
		j++;
	}
	convolutionFn(inputVector,MFparam);
	/*cout<<endl<<"convolution"<<endl;
	for(auto &i:pConvolution){
		for(auto j:*i){
			cout<< j<< " ";
		}
	}
	cout<<endl;*/
	elliotFn();
	/*cout<<endl<<"normalize from mfGrade"<<endl;
		for(auto &i:pNormalize){
			for(auto j:*i){
				cout<< j<< " ";
			}
		}
		cout<<endl;*/
	return (move(pNormalize));
}

/***********************************************************************************************/
/*******************************************************************
convolutionFn()

Convolution of an input vector:

Use:		 	 This function gets an input vector (a pointer to the input vector) and obtain mf grade of each variable.
 	 	 	 	 For each variable, a unique_ptr is defined pointing to a vector having mf grades of the variable. The pointer
 	 	 	 	 is passed to a vector (member variable: pConvolution). So, at the end, there is a vector of pointers where
 	 	 	 	 each pointer points to mf vector of one variable.

Out:			it updates a vector of pointers where each pointer points to a vector having mf values of each variable.

Status:			It is a private member called in MemFungrade method

PreConditions:	pmFEachVar: For each variable, it is pointing to a vector whose size is equal number of mf considered for the variable.
				pmfParam_foreach_Variate: it is a unique pointer defined in commandLine class with random mf params {a,b,c,d} for each membership function of each variate.
				We have one set of parameters for all the input vectors and epochs which is defined once in commandLine class

Postconditions: Calculate convolution of the given input vector.

Invariant:	numVariate,
			numOfMF[i]:
				The number of mf does not need to be same for each variate. e.g. in bivariate time series., we can have
					v1 has 2 mf
					v2 has 3 mf


The function works as:
	There is a main loop over all the variate
		1.Find begin and end of the variate in the input vector
		2. Define a vector (by unique_ptr) to save mf of each variable
		3.For each variable, calculate convolution for each membership function considered for the variable.
			3.1.Calculate convolution based on Eq 13-20 in ANCFIS paper
				0.data=f(j), out=g(k+1-j)
				0.The convolution is calculated as:
					 convSum1=f(1)g(1)+f(1)g(2)+...+f(1).g(n)
					 convSum2=f(2)g(1)+f(2)g(2)+...+f(2).g(n)
					 ...
					 convSumn=f(n)g(1)+f(n)g(2)+...+f(n).g(n)
				0.convSum=convsum1+convSum2+...+convSumn
 *******************************************************************/
void  mfGrade::convolutionFn(unique_ptr<std::vector<double>> const &inputVector,std::vector<std::vector<std::vector<double>>>* MFparam){

	auto beginV=inputVector->begin();
	auto endV=inputVector->begin();

	//The main loop. It is for each variate
	for(int i=0 ; i<numVariate; i++){

		//1.
		if (i==0){
			endV=beginV+lengthofVar[i];
		}
		else{
			beginV=endV;
			endV=beginV+lengthofVar[i];
		}
	//	cout<<endl<<"begin="<<*(beginV)<<endl<<"endl="<<*(endV)<<endl<<"lV"<<lengthofVar[i]<<endl;;

		//2.
		unique_ptr<vector<complex<double>>> pmFEachVar(new vector<complex<double>>(numOfMF[i]));

		//3.
		for(int j=0; j < numOfMF[i]; j++){

			int indexFrq=(*MFparam)[i][j][0];
			double f_real=(*MFparam)[i][j][1];
			double f_imag=(*MFparam)[i][j][2];
			//cout<<endl<<"index= "<<indexFrq<<" freal= "<<f_real<<" f_image= "<<f_imag<<endl;
			//double mf=0;
			/*
			 * int indexFrq=MFparam[i][j][0];
			 * double f_real=MFparam[i][j][1];
			 * double f_imag=MFparam[i][j][2];
			 * MF=f_real*cos(2*pi*indexFrq*k/LengthSurrodata))-f_image*sin(2*pi*indexFrq*k/LengthSurrodata)
			 * */
			double t=0;
			double teta;
			double r=0;
			/*double d=(*pSinMfParam_allVar[i])[j][3];
			double a=(*pSinMfParam_allVar[i])[j][0];
			double b=(*pSinMfParam_allVar[i])[j][1];
			double c=(*pSinMfParam_allVar[i])[j][2];*/
			complex <double> convSum (0,0);
			//	t=2*PI/lengthOfVariate[i];
			//	t=2*M_PI/lengthOfVariate[i];
			t=2*M_PI*indexFrq/lengthSurodata;
		//	cout<<" t= "<<t<<endl;
			//3.1.
			for_each(beginV,endV,[&](double data){						//Eq 19

				for (int k=0;k<lengthofVar[i];k++){

					//TODO: not sure if we need iteration here
					teta=t*(k+iteration);

					r=f_real*cos(teta)-f_imag*sin(teta); //We are getting the value of inverse of FOurier transform in the position of the point that we have
					//TODO: it is been changes						//Eq 14
					//r=d*sin(a*teta+b)+c;								//Eq 13
					//r=(*pSinMfParam_allVar[i])[j][3]*sin((*pSinMfParam_allVar[i])[j][0]*teta+(*pSinMfParam_allVar[i])[j][1])+(*pSinMfParam_allVar[i])[j][2];
					//r=d*sin(a*teta+b)+c;
					//TODO: not sure if we need to get polar in this way. Not sure if teta is working here

					complex<double> out (r*cos(teta), r*sin(teta));		//Eq 15-16
					convSum=out*data+convSum;
					/*cout<<"var= "<<i<<endl;
					cout << "data= "<<data<<endl;
					cout<<"k= "<<k<<endl;
					cout <<"teta= "<<teta<<endl;
					cout << "r= "<<r<<endl;
					cout << "out= "<<out<<endl;
					cout<<"conSum= "<<convSum<<endl;
*/
				}
			});

			//mf of each variate is saved in the vector
			(*pmFEachVar)[j]=convSum;

		}
		pConvolution[i].reset();
		pConvolution[i]=(move(pmFEachVar));

	}

}
/*******************************************************************/

/*******************************************************************
elliotFn()

Use:				It normalizes the mf grade by using elliot function in equation 21. z/(|z|+1)
Out:				Normalized mf grade are generated to be used in firingStrength class
Status:				Private and called from MemFungrade method
PreConditions:		pConvolution is valid
Postconditions:		PNormalize is filled
Invariant:
 *******************************************************************/
void mfGrade::elliotFn(){

	for (int i=0; i<numVariate;i++){

		unique_ptr<vector<complex<double>>> pnorm (new vector<complex<double>>(numOfMF[i]));

		for(int j=0; j < numOfMF[i]; j++){
			if(pConvolution[i]){
				(*pnorm)[j]=(*pConvolution[i])[j]/(1+abs((*pConvolution[i])[j]));
			}
			else{
				cout<<endl<<"Error in pointer handling 6"<<endl;
				exit(1);
			}
		}
		pNormalize[i].reset();
		pNormalize[i]=move(pnorm);
	}
}
/*******************************************************************/


/*******************************************************************
Unit test of concolution function
 *******************************************************************/
//void mfGrade_test(const vector<double> & input_test){
//void mfGrade_test(vector<double> * input_test){
void mfGrade_test(mfGrade & mf1){
	//	const vector<double> & input_test={1,3,4,6,7};
	//mfGrade mf1;
	//mf1.convolutionFn(input_test);



	cout <<endl<<"my container"<<endl;


	cout<<endl<<"this con1"<<endl;
	for (auto i=move((mf1.pConvolution).begin()); i!=move((mf1.pConvolution).end());i++){
		//for (auto j=(i.begin());j!=(i.end());j++)
		auto j=i->get();

		for(auto k:(*j)){
			cout<<k;
		}



	}



	//cout<<"size "<< mf1.pConvolution1.size();

	//	mf1.elliotFn();

	cout<<endl<<"norm2"<<endl;
	for (auto i=move((mf1.pNormalize).begin()); i!=move((mf1.pNormalize).end());i++){
		//for (auto j=(i.begin());j!=(i.end());j++)
		auto j=i->get();

		for(auto k:(*j)){
			cout<<k;
		}



	}

}
/*******************************************************************/
//auto maxMF=max_element(numOfMF.begin(),numOfMF.end());
//We could have different mfs for each variables and this command does not let the vector grows.
//vector<vector<complex <double>>> conv (numVariate, vector<complex<double>> (*maxMF));
/*convolution.reserve(numVariate);
 for (int i=0; i<numVariate;i++){
	 convolution[i].reserve(numOfMF[i]);
 }
 normalizeG.reserve(numVariate);
 	for(int i=0; i <numVariate;i++){
 		normalizeG[i].reserve(numOfMF[i]);
 	}*/
//vector<complex<double>> &conn= convolution[0];
//conn.reserve(*maxMF);
// , vector<complex<double>> (*maxMF));
/////////////////////////////////////////////////
/*for (int i=0; i < numVariate; i++){
		vector<complex<double>> row(numOfMF[i]);
		convolution.push_back(row);
	}*/
/*for (int i=0; i<numVariate;i++){
		convolution[i].reserve(numOfMF[i]);
	}

	for (int i=0; i<numVariate;i++){
		 normalizeG[i].reserve(numOfMF[i]);
			 }*/
////////////////////////////////////////
/*normalizeG.reserve(numVariate);
		 	for(int i=0; i <numVariate;i++){
		 		normalizeG[i].reserve(numOfMF[i]);
		 	}*/

//vector <complex<double>> & vv=normalizeG[0];
//vector<vector<complex<double>>>::iterator row;
//vector<complex<double>>::iterator col;
//vector <vector<complex<double>>>  nor (numVariate,vector<complex<double>> (1));
//int i=0;
//int j=0;
/*for (auto row=convolution.begin(); row <= convolution.end();++row){
		for(auto col=row->begin(); col<= row->end(); ++col){

			nor[i][j]=*col/(1+abs(*col));
			//normalizeG[i].at(j)=(*col/(1+abs(*col)));
			//normalizeG[i][j]=convolution[i][j];
		//	vv.push_back(*col/(1+abs(*col)));
			j++;

		}
		i++;

	}*/
//complex<double> one(1,1);
//	complex<double> &one1=one;
////////////////////////////
/*for_each(convolution.begin(), convolution.end(),[&](vector <complex <double>> & row) {row.reserve(numOfMF[i]);
		});*/
//////////////////
//	conv[i][j]=convSum;
//convolution[i][j]=convSum;
//convolution[i].push_back(convSum);

//unique_ptr<vector<complex<double>>> pmFEachVar(new vector<complex<double>>(numOfMF[i]));
//shared_ptr<complex<double> []> pmFEachVar(new complex<double>[numOfMF[i]]);
//pmFEachVar=make_shared <complex<double> []>(convSum);
//pmFEachVar[j]->push_back(convSum);
//myContainer.push_back(pmFEachVar);
//	myContainer.push_back(move(pmFEachVar));
//nor[i][j]=convolution[i][j]/(1+abs(convolution[i][j]));
//normalizeG[ii][jj]=(convolution[ii][jj]/(1+abs(convolution[ii][jj])));

//normalizeG[ii][jj]={real(c_divide(convolution[ii][jj],c)),imag(c_divide(convolution[ii][jj],c))};
//normalizeG[i][j]=one;
//	myContainerNor[ii][jj]=myContainer[ii][jj]/(1+abs(myContainer[ii][jj]));
//myContainerNor.push_back(move(pnorm));
//normalizeG=nor;

/**/
