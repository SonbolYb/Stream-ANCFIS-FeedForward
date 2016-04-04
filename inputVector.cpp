/*
 * inputVector.cpp
 *
 *  Created on: Jun 10, 2015
 *      Author: sonbol
 */

#include "inputVector.h"
using namespace std;

/*******************************************************************
inputVector ()

input:		A vector to input data in an input vector
target:		A vector to target data in an input vector
 *******************************************************************/
inputVector::inputVector(): commandLine(){

//cout<<"inputVector1"<<endl;

}

inputVector::~inputVector(){
//	cout<<"inputVector2"<<endl;
}
/******************************************************************/
/*******************************************************************
readData()

Use:				Reads an input vector and and separate input and target
Out:				input and target vectors are filled
Status:				Public and called from
					calculateWeight() in inputVecWeight class
					output() in AncfisChecking
PreConditions:
Postconditions:
Invariant:
 *******************************************************************/
void inputVector::readData(const std::vector<double>& datapoints, vector<int>* dim){
	/*cout<<"inputVec from readData"<<endl;
	for(auto i:datapoints){
		cout<<i<<" ";
	}*/
	int inputV=0;
		for(auto i:*dim){
			inputV+=i;
		}
	inputVL=inputV;
	input.reset(new vector<double> (inputVL));
	target.reset(new vector<double> (numVariate));

	int j=0;
	int k=0;
	int lengthCounter_target=(*dim)[0];

	for (int i=0 ; i < inputVL+numVariate;i++){
	//	cout<<"datapoint"<<datapoints[i];
		if(i==(lengthCounter_target)){
			(*target)[j]=datapoints[i];
			j++;
			if(j<numVariate){
			lengthCounter_target=lengthCounter_target+(*dim)[j]+1;
			}


		}
		else{
			(*input)[k]=datapoints[i];
			k++;
		}
	}

}
/******************************************************************/
/*******************************************************************
readInput()

Use:				Returns a pointer to input in the input vector
Out:
Status:				Public and called from
					calculateWeight() in inputVecWeight class
					output() in AncfisChecking
PreConditions:		input is not empty
Postconditions:
Invariant:
 *******************************************************************/
unique_ptr<std::vector<double>>&  inputVector::readInput() {

	return((input));

	//To check if all the element of the input is not zero
	//vector<double> copyVector(input.size(),0);

	//vector<double>* inp= & input;
	/*if (!input.empty()){
		return(inp);
	}*/
	/*else if (input==copyVector){
		cout << "Warning:"<<endl<<"All the element of the input is zero"<<endl;
		return(inp);
	}*/
	/*else{
		cout << "You probably have not saved the input vector. Save it first by instantiating time series class and then continue"<<endl;
		exit(1);
	}*/
}
/******************************************************************/
/*******************************************************************
readTarget()

Use:				Returns a pointer to target in the input vector
Out:
Status:				Public and called from
					calculateWeight() in inputVecWeight class
					output() in AncfisChecking
PreConditions:		target is not empty
Postconditions:
Invariant:
 *******************************************************************/
unique_ptr<std::vector<double>> & inputVector::readTarget(){
	return((target));

	//TO check if all the element of the target is zero
	/*vector<double> copyVector(target.size(),0);
	vector<double>* tar=&target;
	if (! target.empty()){
		return(tar);
	}
	else if(target==copyVector){
		cout << "Warning:"<<endl<<"All the element of the target is zero"<<endl;
		return(tar);
	}
	else{
		cout << "You probably have not saved the input vector. Save it first by instantiating time series class and then continue"<<endl;
		exit(1);
	}*/
}
/******************************************************************/

/*void inputVector_test(vector<double>  tt){


	inputVector t1;
	vector<double> & ptt=tt;
	//TODO: make it work
	//t1.readData(ptt);

		cout <<endl;
		cout <<"input"<<endl;
		for (auto i:t1.input){
			cout << i<<" ";
		}
		cout<<endl;
		cout <<"target"<<endl;
		for(auto i:t1.target){
			cout<<i<<" ";
		}
		//Check read input and target function
		cout <<endl<<endl<<endl;
		cout <<"Input of function"<<endl;
		 for(auto i: *(t1.readInput())){
			 cout<<" "<<i;
		 }
		for(auto i =((t1.readInput())->begin(); i!=(t1.readInput())->end(),i++){
			cout<< *i;
		}


		 cout << endl;
}*****************************************************************/
/*inputVector::inputVector(double * data): commandLine(),datapoints(data),input(inputlength),target(targetlength){

	readData();
}*/

/*inputVector::inputVector(vector<double> *data): commandLine(),datapoints(data),input(inputlength),target(targetlength){

	readData();
}*/
