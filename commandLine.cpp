/*
 * readComLine.cpp
 *
 *  Created on: Jun 8, 2015
 *      Author: sonbol
 */

#include "commandLine.h"
using namespace std;

//Definition of static variables
/*
 * numVariate= number of variate. In case of multivariate time series, it is greater than 1
 * numOutput= number of output. It is usually same as numVariate but we consider a separate variable in case of MISO systems
 * numOfMF= number of membership functions
 * numEpoch= number of epoch
 * threshold= the threshold considered for error to stop the experiment
 * numRule= number of rules
 * numCore= number of Computer cores we want to use
 * lambdaRLS= lambda used in RLS algorithm
 * initError= a large error to initiate the error vectors
 * TrainFile1= name of training set
 * CheckFile1=name of checking set
 * TrainSize= number of data points in the training set
 * CheckSize= number of data points in the checking set
 * per10= the size of the first window we consider and here it is 10% of the whole data in the training set
 * corrlength= it is the maximum delay that we consider for mutual information for finding delay in Delay class
 * dmax= it gives maximum delay considered in Cao's approach to find dimension
 * numSetDim= the number of set of dimensions we need to do experiment on for Cao's approach
 * forget= forgetting factor in RLS algorithm
 * LengthSurrodata= the size of the second window we use for Fourier transform. We determine in MFParam class
 *
 * ****We determine these variable in DelayVector class after knowing appropriate delay and dimension*****
 * numWeight= number of weights that we have in RANCFIS
 * LengthDV= number of delay vectors we have in our delay vectors
 * inputVL= length of one input vector without considering its predictions. It is sume of dimension of each variate
 * lengthofVar= it is vector of the dimension of each variate
 * */

int commandLine::numVariate;
int commandLine::numOutput;
vector<int> commandLine::numOfMF;
int commandLine::numEpoch;
double commandLine::threshold;
int commandLine::numRule;
int commandLine::numCore;
double commandLine::lambdaRLS;
double commandLine::initError;
const char * commandLine::TrainFile1;
const char * commandLine::CheckFile1;
int commandLine::TrainSize;
int commandLine::CheckSize;
int commandLine::per10;
long commandLine::corrlength;
int commandLine::dmax;
int commandLine::numSetDim;
double commandLine::forget;
double commandLine::per10Coeff;
//int commandLine::numWeight;
//int commandLine::LengthSurrodata;
//int commandLine::LengthDVSet;
//int commandLine::inputVL;
//vector<int> commandLine::lengthofVar;


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
				 -ep= number of epochs
				 -th= threshold of error for average error of outputs
				 -Nout= number of outputs
				 -Ncore=number of core to be used
				 -lambdaRLS= lambda used in RLS algorithm
				 -trFN=Name of training file
				 -chFN-Name of checking file
				 -Trsize= number of training data
				 -Chsize= number of checking data
				 -dl= it is the maximum delay that we consider for mutual information for finding delay in Delay class
				 -dmax= it gives maximum delay considered in Cao's approach to find dimension
				 -fr= Forgetting coefficient in RLS
				 -perCf= it is the percentage, we consider for the first window of the train set

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

		if(strcmp(argV[i],"-ep")==0){
			numEpoch=atoi(argV[i+1]);
			i++;
		}
		if(strcmp(argV[i],"-th")==0){
			threshold=atof(argV[i+1]);
			i++;
		}
		if(strcmp(argV[i],"-Nout")==0){
			numOutput=atoi(argV[i+1]);
			i++;
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
		if(strcmp(argV[i],"-fr")==0){
			forget=atof(argV[i+1]);
			i++;
		}
		if(strcmp(argV[i],"-perCf")==0){
			per10Coeff=atof(argV[i+1]);
			i++;
		}
	}

	//calculate the other variables
	calculateOthers();

	saveParameters();
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

	int b=1;
	for_each(numOfMF.begin(),numOfMF.end(),[&](int n){b*=n;});
	numRule=b;
	initError=1000;
	per10=TrainSize*per10Coeff;
	//dmax=per10/2;	//cannot be arbitrary. It depends on the size of the per10
	//TODO:change dmax: Here I com considering as 15 because the greater dimension that I saw in batch learning was 12
	//dmax=TrainSize*0.1/2;
	dmax=15;
	numSetDim=pow(dmax,numVariate);
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
	myfile<<"numberofEpoch:\t\t\t"<<numEpoch<<endl;
	myfile<<"threshold:\t\t\t"<< threshold<<endl;
	myfile<<"numofRule:\t\t\t"<<numRule<<endl;

	myfile<<endl<<"number of core used:\t\t"<<numCore<<endl;
	myfile<<"lambda for RLs:\t\t\t"<<lambdaRLS<<endl;
	myfile<<"forget coefficient:\t\t\t"<<forget<<endl;
	myfile<<"correlation length:\t\t\t"<<corrlength<<endl;
	myfile<<"TrainSize:\t\t\t"<<TrainSize<<endl;
	myfile<<"CheckSize:\t\t\t"<<CheckSize<<endl;
	myfile<<"dmax:\t\t\t"<<dmax<<endl;
	myfile<<"numSetDim:\t\t\t"<<numSetDim<<endl;
	myfile<<"per10Coeff:\t\t\t"<<per10Coeff<<endl;

	myfile.close();
}
/******************************************************************/
/*******************************************************************

 *******************************************************************/
unique_ptr<vector<vector<int>>> commandLine::ruleStruct(){
	//	cout<<"hi8"<<endl;
	//	cout<<"numRul "<<numRule<<"  numVar"<<numVariate<<endl;
	unique_ptr<vector<vector<int>>> prule;
	prule.reset(new vector<vector<int>> (numRule, vector<int> (numVariate)));

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
	//cout<<"hi9"<<endl;
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

	cout <<"nameofTrain= "<<Cl.TrainFile1<<endl;
	cout<<"nameofCheck= "<<Cl.CheckFile1<<endl;
	cout<<"numberofEpoch= "<<Cl.numEpoch<<endl;
	cout<<"threshold= "<< Cl.threshold<<endl;
	cout<<"numofRule= "<<Cl.numRule<<endl;
	cout<<"numofOutput="<<Cl.numOutput<<endl;
	cout<<"number of core used= "<<Cl.numCore<<endl;
	cout<<"lambda for RLs= "<<Cl.lambdaRLS<<endl;
	cout<<"forget coefficient= "<<Cl.forget<<endl;
	cout<<"correlation length= "<<Cl.corrlength<<endl;
	cout<<"TrainSize= "<<Cl.TrainSize<<endl;
	cout<<"CheckSize= "<<Cl.CheckSize<<endl;
	cout<<"dmax= "<<Cl.dmax<<endl;
	cout<<"numSetDim= "<<Cl.numSetDim<<endl;
	cout<<"per10Coeff= "<<Cl.per10Coeff<<endl;

	/*shared_ptr<vector<vector<int>>> prule=Cl.ruleStruct();
	cout<<"this is rule"<<endl;

	for (auto i:(*(prule))){
		for(auto j:i){
			cout<<" "<<j;
		}
		cout<<endl;
	}
	 */

}
