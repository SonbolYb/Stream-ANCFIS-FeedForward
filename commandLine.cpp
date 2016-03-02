/*
 * readComLine.cpp
 *
 *  Created on: Jun 8, 2015
 *      Author: sonbol
 */

#include "commandLine.h"
using namespace std;

//Definition of static variables

int commandLine::numVariate;
int commandLine::numOutput;
vector<int> commandLine::numOfMF;
vector<int> commandLine::lengthOfVariate;
int commandLine::inputVectorLength;
int commandLine::inputlength;
int commandLine::targetlength;
int commandLine::ChunkSize;
int commandLine::numOfInVecTraining;
int commandLine::numOfInVecChecking;
const char * commandLine::TrainFile;
const char * commandLine::CheckFile;
int commandLine::numEpoch;
double commandLine::threshold;
int commandLine::numRule;
int commandLine::numParamMF;
int commandLine::numCore;
double commandLine::lambdaRLS;
double commandLine::initError;
vector<double> commandLine::aRange;
vector<double> commandLine::bRange;
std::vector<std::unique_ptr<std::vector<std::vector<double>>>> commandLine::pSinMfParam_allVar(1);
double commandLine::aS;
double commandLine::bS;
double commandLine::cS;
double commandLine::dS;

const char * commandLine::TrainFile1;
const char * commandLine::CheckFile1;
int commandLine::TrainSize;
int commandLine::CheckSize;
int commandLine::per10;
long commandLine::corrlength;
int commandLine::dmax;
int commandLine::numSetDim;


/*******************************************************************
saveComLine

Use:	This function reads command line arguments and save them directly in static variables. It also calls
 	 	 calculateOthers method to save the other input variables which can  be calculated from command line
 	 	 and save them in the other static member variables.


Out:	Assign values to static variables of commandLine class. The class is used as base class
		for other classes.
Status:	Public method member and it is callled in the main

PreConditions:
				 argC(=TODO?) and argV are read from main and save in static member variables.
				 -Nv = number of variates >0 	(if numVariate=1 ==> univariate timeseries)
				 -Nmf= number of membership functions for each variate 	(length of numOfMF=numVariate)
				 -Lv = Length of each variate	(length of lengthOfVariate=numVariate)
				 -NTr= number of training input vector >0
				 -NCh= number of checking input vector >0
				 -trFN= name of training time series (a valid string)
				 -chFN= name of checking time series (a valid sttring)
				 -ep= number of epochs
				 -th= threshold of error for average error of outputs
				 -Nout= number of outputs
				 -aR= range of a in mf params
				 -bR= range of b in mf params
				 -dl= it is the maximum delay that we consider for mutual information for finding delay in Delay class
				 -dmax= it gives maximum delay considered in Cao's approach to find dimension

Postconditions:
				All the static member variables are filled

Invariant:
 *******************************************************************/

void commandLine::saveComLine(int argC,char * argV[]){

	//Read directly from command line

	for (int i=0; i < argC; i++){
		if (strcmp(argV[i],"-Nv")==0){
			numVariate=(atoi(argV[i+1]));
			i++;
		}
		if (strcmp(argV[i],"-Nmf")==0){
			for (int j=0; j<numVariate; j++){
				numOfMF.push_back(atoi(argV[i+1]));
				i++;
			}
		}
		if (strcmp(argV[i],"-Lv")==0){
			for (int j=0; j<numVariate; j++){
				lengthOfVariate.push_back(atoi(argV[i+1]));
				i++;
			}
		}
		if(strcmp(argV[i],"-NTr")==0){
			numOfInVecTraining=(atoi(argV[i+1]));
			i++;
		}
		if(strcmp(argV[i],"-NCh")==0){
			numOfInVecChecking=(atoi(argV[i+1]));
			i++;
		}
		if(strcmp(argV[i],"-trFN1")==0){
			TrainFile=argV[i+1];
			i++;
		}
		if(strcmp(argV[i],"-chFN1")==0){
			CheckFile=argV[i+1];
			i++;
		}
		if(strcmp(argV[i],"-ep")==0){
			numEpoch=atoi(argV[i+1]);
			i++;
		}
		if(strcmp(argV[i],"-th")==0){
			threshold=atof(argV[i+1]);
			i++;
		}
		if(strcmp(argV[i],"-Nout")==0){
			numOutput=atof(argV[i+1]);
			i++;
		}
		if(strcmp(argV[i],"-aR")==0){
			for(int j=0;j<2;j++){
				aRange.push_back(atof(argV[i+1]));
				i++;
			}
		}

		if(strcmp(argV[i],"-bR")==0){
			for(int j=0;j<2;j++){
				bRange.push_back(atof(argV[i+1]));
				i++;
			}
		}
		if(strcmp(argV[i],"-Ncore")==0){
			numCore=atoi(argV[i+1]);
			i++;
		}
		if(strcmp(argV[i],"-lam")==0){
			lambdaRLS=atof(argV[i+1]);
			i++;
		}


		if(strcmp(argV[i],"-trFN")==0){
			TrainFile1=argV[i+1];
			i++;
		}
		if(strcmp(argV[i],"-chFN")==0){
			CheckFile1=argV[i+1];
			i++;

		}
		if(strcmp(argV[i],"-Trsize")==0){
						TrainSize=atoi(argV[i+1]);
						i++;
					}
		if(strcmp(argV[i],"-Chsize")==0){
						CheckSize=atoi(argV[i+1]);
						i++;
					}
		if(strcmp(argV[i],"-dl")==0){
								corrlength=atoi(argV[i+1]);
								i++;
							}
		/*if(strcmp(argV[i],"-dmax")==0){
										dmax=atoi(argV[i+1]);
										i++;
									}*/
	}

	//calculate the other variables
	calculateOthers();
//	initialMf();
//	saveParameters();
}
/******************************************************************/
/*******************************************************************
calculateOthers

Use:	This function calculate the other static member variables from the static member variables read directly
		from command line (those are declare in saveComLine).

Out:	Assign value to the rest of static member variables have not been initialized in saveComLine.

Status:	Private member method of the class of commandLine and called from saveComLine

PreConditions:
				inputVectorLength > 0
				inputlength > 0
				targetlength > 0

Postconditions:
				static member variables are assigned.
				inputVectorLength= sum over length of each variate (sum over lengthOfVariate vector)+ targets (numVariate)
				inputlength in each input vector = sum over length of each variate (sum over lengthOfVariate vector)=[inputVectorLength-target]
				targetlength in each input vector = numVariate
				numRule
				pSinMfParam_allVar= size of it changes to numVariate because it has mf params of all variate
Invariant:
 *******************************************************************/
void commandLine::calculateOthers(){

	for_each(lengthOfVariate.begin(),lengthOfVariate.end(),[this](int n){inputVectorLength+=n;});
	inputVectorLength+=numVariate; //This is to consider outputs in the input vectors
	inputlength=inputVectorLength-numVariate;
	targetlength=numVariate;
	int b=1;
	for_each(numOfMF.begin(),numOfMF.end(),[&](int n){b*=n;});
	numRule=b;
	numParamMF=4;
	//numCore=1;
	//lambdaRLS=0.1;
	initError=1000;
	ChunkSize=1;
	pSinMfParam_allVar.resize(numVariate);
	per10=TrainSize*0.1;
	dmax=per10/2;	//cannot be arbitrary. It depends on the size of the per10
	numSetDim=pow(dmax,numVariate);

//	numSetDim=dmax*numVariate;
}
/******************************************************************/
/*******************************************************************
saveParameter()
Use:
Out:
Status:
PreConditions:
Postconditions:
Invariant:
 *******************************************************************/
void commandLine::saveParameters(){
	fstream myfile;

	//throw exception if the file cannot be opened
	myfile.exceptions(ifstream::failbit|ifstream::badbit);
	try{
		myfile.open("FinalParams.txt",ios::app|ios::out);
	}
	catch(fstream::failure &e){
		cerr << "Exception opening/reading/closing file\n";
	}
	myfile<<"******************************************************"<<endl;
	myfile <<"numVaraite:\t\t\t"<<numVariate<<endl;

	myfile <<"numofMf:\t\t\t";
	for(auto i:numOfMF){
		myfile <<i<<" ";
	}
	myfile<<endl;
	myfile<<"lenghtofVariates:\t\t";
	for(auto i:lengthOfVariate){
		myfile<<i<<" ";
	}
	myfile<<endl;
	myfile<<"inputVectorLength:\t\t"<<inputVectorLength<<endl;
	myfile<<"numofInVecTr:\t\t\t"<<numOfInVecTraining<<endl;
	myfile<<"numofInVecCh:\t\t\t"<< numOfInVecChecking<<endl;
	myfile<<"numberofEpoch:\t\t\t"<<numEpoch<<endl;
	myfile<<"threshold:\t\t\t"<< threshold<<endl;
	myfile<<"numofRule:\t\t\t"<<numRule<<endl;
	myfile<<"Rang of a:\t\t\t";
	for(auto i:aRange){
		myfile<<i<<" ";
	}
	myfile<<endl<<"Range of b:\t\t\t";
	for(auto i:bRange){
		myfile<<i<<" ";
	}
	myfile<<endl<<"numver of core used:\t\t"<<numCore<<endl;
	myfile<<"lambda for RLs:\t\t\t"<<lambdaRLS<<endl;
	myfile<<"This is mf params:"<<endl;
	for(auto &i:pSinMfParam_allVar){
		for(auto j:*i){
			for(auto k:j){
				myfile<<k<<" ";
			}
			myfile<<endl;
		}
		myfile<<endl;
	}
	myfile<<endl;
	myfile.close();
}
/******************************************************************/

/*******************************************************************
initialMf()
Use:				It assign random values to mf params of each variate. Each mf has 4 parametes to be assigned {a,b,c,d}.
					It fills a vector of unique pointers where each pointer pointing to mfs of one variate.
Out:				fill pSinMfParam_allVar
Status:				Private and called from saveComLine
PreConditions:		pSinMfParam_allVar is a valid vector and the size is equal to numVariate.
Postconditions:		the unique_ptrs in pSinMfParam_allVar are valid
Invariant:
 *******************************************************************/
void commandLine::initialMf(){

	for (int i=0; i<numVariate;i++){
		unique_ptr<vector <vector <double> > > pmfParam_foreach_Variate(new vector< vector <double> > (numOfMF[i], vector<double>(numParamMF)));
		for (int j=0; j< numOfMF[i];j++){
			initiateA();
			initiateB();
			initiateCD();
			(*pmfParam_foreach_Variate)[j]={aS,bS,cS,dS};

		}
		pSinMfParam_allVar[i]=move(pmfParam_foreach_Variate);
	}
}
/******************************************************************/
/******************************************************************
initiateA()

Use:				Initialize a in (mf) d*sin(a*teta+b)+c by using a random number generator mt19937 and over uniform distribution
Out:				Random value for a between ...
Status:				Private and called from initialMf()
PreConditions:
Postconditions:
Invariant:
 ******************************************************************/
void commandLine::initiateA(){



	random_device rd;			//seed
	mt19937_64 mt(rd());			//using mt19937 random generator
	uniform_real_distribution<double> dist(aRange[0],aRange[1]);		//a between 0-10 by uniform distribution
	aS=dist(mt);
	//	a=5;
}
/*****************************************************************/
/******************************************************************
initiateB()

Use:				Initialize b in (mf) d*sin(a*teta+b)+c by using a random number generator mt19937 and over uniform distribution
Out:				A random value for b between ...
Status:				Private and called from initialMf()
PreConditions:
Postconditions:
Invariant:
 ******************************************************************/
void commandLine::initiateB(){

	random_device rd;			//seed
	mt19937_64 mt(rd());			//using mt19937 random generator
	uniform_real_distribution<double> dist(bRange[0],bRange[1]);		//b between 0-100 by uniform distribution
	bS=dist(mt);
	//b=90;
}
/*****************************************************************/
/******************************************************************
initiateCD()

Use:				Initialize c and d in (mf) d*sin(a*teta+b)+c by using a random number generator mt19937 and over uniform distribution
Out:				Random values between 0-1 (inclusive)
Status:				Private and called from initialMf()
PreConditions:
Postconditions:		0 =< d+c =< 1
					1 >= c >= d> >=0
Invariant:
 ******************************************************************/
void commandLine::initiateCD(){

	random_device rd;			//seed
	mt19937_64 mt(rd());			//using mt19937 random generator
	uniform_real_distribution<double> dist(0,nextafter (1,DBL_MAX));

	/* Initialize c and d to satisfy one of the condition and go to the while loop
	 * because we have multiple calls to initiateCD*/
	cS=0;
	dS=0.1;
	while ((0 > cS+dS  || cS+dS > 1) || (cS < dS)){
		dS=dist(mt);
		cS=dist(mt);

	}


}
/*****************************************************************/
/*******************************************************************

 *******************************************************************/
shared_ptr<vector<vector<int>>> commandLine::ruleStruct()const{
	shared_ptr<vector<vector<int>>> prule (new vector<vector<int>> (numRule, vector<int> (numVariate)));

	int N=numRule;
	int r=0, c=0;

	for (int i=0; i< numVariate;i++){
		N=N/numOfMF[i];
		while(r<numRule){
			for (int j=0; j<numOfMF[i];j++){
				for (int k=0;k<N;k++){
					(*prule)[r][c]=j;
					r++;
				}
			}
		}
		c++;
		r=0;
	}
	return(prule);
}
/******************************************************************/
void commandLine_test()
{
	commandLine Cl;
	cout <<"numVaraite= "<<Cl.numVariate<<endl;

	cout <<"numofMf= "<<endl;
	for(auto i:Cl.numOfMF){
		cout <<i<<" "<<endl;
	}
	cout<<endl;
	cout<<"lenghtofVariates= "<<endl;
	for(auto i:Cl.lengthOfVariate){
		cout<<i<<" "<<endl;
	}
	cout<<endl;
	cout<<"inputVectorLength= "<<Cl.inputVectorLength<<endl;
	cout<<"inputLength= "<<Cl.inputlength<<endl;
	cout<<"targetLenth= "<<Cl.targetlength<<endl;
	cout<<"numofInVecTr= "<<Cl.numOfInVecTraining<<endl;
	cout <<"numofInVecCh= "<< Cl.numOfInVecChecking<<endl;
	cout <<"nameofTrain= "<<Cl.TrainFile<<endl;
	cout<<"nameofCheck= "<<Cl.CheckFile<<endl;
	cout<<"numberofEpoch= "<<Cl.numEpoch<<endl;
	cout<<"threshold= "<< Cl.threshold<<endl;
	cout<<"numofRule= "<<Cl.numRule<<endl;
	cout<<"numofOutput="<<Cl.numOutput<<endl;
	cout<<"Rang of a:"<<endl;
	for(auto i:Cl.aRange){
		cout<<i<<" ";
	}
	cout<<endl<<"Range of b:"<<endl;
	for(auto i:Cl.bRange){
		cout<<i<<" ";
	}
	cout<<endl<<"numver of core used= "<<Cl.numCore<<endl;
	cout<<"lambda for RLs= "<<Cl.lambdaRLS;
	cout<<endl<<"This is mf params:"<<endl;
	for(auto &i:Cl.pSinMfParam_allVar){
		for(auto j:*i){
			for(auto k:j){
				cout<<k<<" ";
			}
			cout<<endl;
		}
		//cout<<endl;
	}

	shared_ptr<vector<vector<int>>> prule=Cl.ruleStruct();
	cout<<"this is rule"<<endl;

	for (auto i:(*(prule))){
		for(auto j:i){
			cout<<" "<<j;
		}
		cout<<endl;
	}


}
