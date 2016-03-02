/*
 * firingStrength.cpp
 *
 *  Created on: Jun 19, 2015
 *      Author: sonbol
 */

#include "firingStrength.h"

using namespace std;

/*******************************************************************
firingStrength()

pFS:		A unique_ptr to the final firing strength
mfG:		A vector of shared pointers pointing to the membership functions of each variate.
fsRaw:		Vector having raw firing strength
fsNorm:		Vector having normalized firing strength
SumNorm:	Save sum of denum of equation 24.
sumDP:		save the sum in euqation 25.
 *******************************************************************/
firingStrength::firingStrength():commandLine(),pFS(new vector<double> (numRule)),mfG(numVariate),fsRaw(numRule),fsNorm(numRule),SumNorm(0),SumDP(0,0) {
	// TODO Auto-generated constructor stub
}

/*******************************************************************
cal_firingStrenghT ()

Use:				Calculates the firing strength of the rules for a given input.
					It called two pointers from mf and commandline class(rule) and use them
					to find firing strength of each rule.
Out:				returns a unique pointer to the final firing strength.
Status:				Public and called in inputVecWeight class
PreConditions:		input vector and mfG and rule are valid
Postconditions:		pointer to the firing strength (pFS) is valid.
Invariant:
 *******************************************************************/
unique_ptr<vector<double>> firingStrength::cal_firingStrenght( vector<double> * const input){

	mfG=mf.MemFungrade(input);
	//mfGrade_test(mf);
	rule=ruleStruct();
	getFinalFs();
	return(move(pFS));


}
/*******************************************************************/
/*******************************************************************
getFinalFs()
Use:				Calculates the final firing strength of rules by using 3 private methods
							getRawFS ==> to get the raw fs
							normalize ==> to normalize the raw fs;
							dotP==> to convert complex fs to real fs
Out:
Status:				Private and called from cal_firingStrength
PreConditions:
Postconditions:
Invariant:
 *******************************************************************/
void firingStrength::getFinalFs(){
	getRawFS();
	normalize();
	dotP();
	/*fsNorm=normalize(fsRaw);
		fS=dotProd(fsNorm);*/
}
/*******************************************************************/
/*******************************************************************
getRawFS()

Use:				Calculates Firing strength before going to normalization and dot product.based on the rule structure matrix,
 	 	 	 	 	 it finds the nodes should be connected to give a rule. For each rule:
							it goes to the corresponding row in the rule matrix and then read the row
							which has indexes of which mf of each variate should be connected now.
							Then go to the mfG matrix and based on the index for each variate reads
							the corresponding value and use conjunction operator to find firing strength of the rule.
Out:				fill fsRaw.
Status:				Private and called from getFinalFs()
PreConditions:		rule and mfG are valid
Postconditions:		fill fsRaw.
Invariant:
 *******************************************************************/
void firingStrength::getRawFS(){

	int index=0;

	for (int i=0;i<numRule;i++){

		complex<double> mf_Final (1.0,0.0);

		for(int j=0; j<numVariate;j++){
			if(rule && mfG[j]){
				index=(*rule)[i][j];
				mf_Final=conjunctionOp(mf_Final,(*(mfG[j]))[index]);	//eq. 23 in ANCFIS paper
			}
			else{
				cout<<endl<<"Error in pointer handling 5"<<endl;
				exit(1);
			}

		}

		fsRaw[i]=mf_Final;
	}
}
/*******************************************************************/
/*******************************************************************
conjunctionOp():

Use:				conjunction operator used in ANCFIS (now it is multiplication)
Out:
Status:				private and called from getRawFS()
PreConditions:
Postconditions:
Invariant:
 *******************************************************************/
complex<double> firingStrength::conjunctionOp(complex<double> a,complex<double>b){

	complex<double> c(0,0);
	c=a*b;
	return(c);
}
/*******************************************************************/
/*******************************************************************
normalize:


Use:				Normalization of raw firing strength based on Equation 24 in ANCFIS paper
Out:				fsNorm is filled
Status:				Private and called from getFinalFs()
PreConditions:
Postconditions:		fsNorm is valid and it does not have Nan as value.
Invariant:
 *******************************************************************/
void firingStrength::normalize(){	//eq.24 in ANCFIS paper


	int j=0;
	SumNorm=0;
	for(auto i:fsRaw){
		SumNorm+=abs(i);
	}
	//It is for Nan
	if(SumNorm==0){
		for(int j=0; j<numRule;j++){
			fsNorm[j]=0;
			j++;
		}
	}
	else{
		for(auto i:fsRaw){
			fsNorm[j]=i/SumNorm;
			j++;
		}
	}



}
/*******************************************************************/
/*******************************************************************
dotP

Use:				Dot product applied on the normalized firing strength. It is Equation 25 in ANCFIS paper
Out:				Final firing strength that has real valued fs.
Status:				Private and called from getFinalFs()
PreConditions:		fsNorm is valid
Postconditions:		pFS is filled
Invariant:
 *******************************************************************/
void firingStrength::dotP(){	//eq.25
	int j=0;
	SumDP=0;
	for(auto i:fsNorm){
		SumDP+=i;
	}
	for(auto i:fsNorm){
		(*pFS)[j]=dotOper(i,SumDP);
		j++;
	}
}
/*******************************************************************/
/*******************************************************************
dotOper

Use:				the operation used for converting complex to real number.(currently, we have dot product)
Out:
Status:				private and called from dotP.
PreConditions:
Postconditions:
Invariant:
 *******************************************************************/
double firingStrength::dotOper(complex<double> a,complex<double> b){
	double c=0;
	c=real(a)*real(b)+imag(a)*imag(b);
	return(c);
}

/*******************************************************************/
void FS_test(firingStrength &fs){

	cout<<endl<<"this is mfs"<<endl;
	for(int i=0;i<fs.numVariate;i++){
		for (auto j: *(fs.mfG[i]))
			cout<<j<<" ";
	}
		/*for(auto &i:(fs.mfG)){
			for(auto j:*i){
				cout <<" "<<j;
			}

			cout<<endl;
		}*/

		cout<<"this is rule"<<endl;
		for(auto i:(*(fs.rule))){
			for(auto j:i){

				cout<<" "<<j;


			}
			cout<<endl;
		}
		cout<<endl;

		cout<<"This is FSraw"<<endl;
		for (auto i:fs.fsRaw){
			cout<<" "<<i;
		}
		//cout<<endl<<"magSUm="<<fs.magSum<<endl;
		cout<<endl<<"this is fsNorm"<<endl;
		for(auto i:fs.fsNorm){
			cout<<" "<<i;
		}
		cout<<endl<<"weightSum "<< fs.SumDP;
		cout<< endl<<"this is final= "<<endl;

}
/*void FS_test(std::vector<double> * input){
	//const vector<double> & input={0,2,3,4,5};
	vector<double> input={0,2,3,4,5};
	vector<double> * pinput=&input;
	firingStrength fs;
	shared_ptr<vector<double>> fss;
	fss=fs.cal_firingStrenght(pinput);


	cout<<"this is mfs"<<endl;
	for(auto &i:(fs.mfG)){
		for(auto j:*i){
			cout <<" "<<j;
		}

		cout<<endl;
	}

	cout<<"this is rule"<<endl;
	for(auto i:(*(fs.rule))){
		for(auto j:i){

			cout<<" "<<j;


		}
		cout<<endl;
	}
	cout<<endl;

	cout<<"This is FSraw"<<endl;
	for (auto i:fs.fsRaw){
		cout<<" "<<i;
	}
	//cout<<endl<<"magSUm="<<fs.magSum<<endl;
	cout<<endl<<"this is fsNorm"<<endl;
	for(auto i:fs.fsNorm){
		cout<<" "<<i;
	}
	cout<<endl<<"weightSum "<< fs.SumDP;
	cout<< endl<<"this is final= "<<endl;
	for(auto i:(*fss)){

		cout<<" "<<i;

	}*/

	/*cout<<endl<<"this is temp"<<endl;
vector<double> t={1,-2,3,5};
vector<double> t2;
t2=normalize(t);
for(auto i:t2){
	cout<<" "<<i;
}

vector<complex<double>> t1={{1,0},{-2,0},{3,0},{5,0}};
vector<complex<double>> t3;
t3=normalize(t1);
for(auto i:t3){
	cout<<" "<<i;
}*/

//}
