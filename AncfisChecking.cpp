/*
 * AncfisChecking.cpp
 *
 *  Created on: Jul 9, 2015
 *      Author: sonbol
 */

#include "AncfisChecking.h"
using namespace std;

/*******************************************************************
AncfisChecking()

checkMatrix:				pointer to 2D vector having the checking set
input:						A pointer to inputs of an input vector
target:						A pointer to targets of an input vector
checkError:					A 2D vector saving error of each input vector for each variable
aveError:					A vector saving average error over all the variables in each input vector
 *******************************************************************/
AncfisChecking::AncfisChecking():commandLine(),checkMatrix(NULL), input(NULL),target(NULL),rpH(pH),checkError(numOfInVecChecking,vector<double>(numOutput)),
		aveError(numOfInVecChecking,0),finalChError(0),weightTrn(NULL){

}
/******************************************************************/
/*******************************************************************
outputRMSE()

Use:				Gets the final weights from the AncfisNet class and apply them to checking data.
Out:				Final error of checking data
Status:				public and called from the main function
PreConditions:
Postconditions:
Invariant:

Note:
	1. Getting the checking data set
	2. For each input vector in the checking set,
		2.1. build a network.
		2.2. For each variable in the input vector, get the error by using weight coming from AncfisNet.
	3. Getting the final error by RMSE
 *******************************************************************/

void AncfisChecking::outputRMSE(vector<vector<double>> *weight){

	weightTrn=weight;
	//1.
	checkMatrix=checking.getChecking();

	double aveFinal=0;

	//2.
	for(int indexInV=0; indexInV <numOfInVecChecking; indexInV++){
	//2.1.
		vector<double> & inputVec=(*checkMatrix)[indexInV];
		buildNet(inputVec);

		double ave=0;
	//2.2
		for (int nV=0; nV<numOutput; nV++){

			vector<double> &rw=(*weight)[nV];
			checkError[indexInV][nV]=(*target)[nV]-inVW.Cal_VVS(rw,rpH);
			ave+=pow(checkError[indexInV][nV],2);
		}
		aveError[indexInV]=sqrt(ave/numOutput);
		aveFinal+=ave;
	}
	//3.
	finalChError=sqrt(aveFinal/(numOfInVecChecking*numOutput));

	saveParams();
}
/******************************************************************/
/*******************************************************************
outputMAE()

Use:				Gets the final weights from the AncfisNet class and apply them to checking data.
Out:				Final error of checking data
Status:				public and called from the main function
PreConditions:
Postconditions:
Invariant:

Note:
	1. Getting the checking data set
	2. For each input vector in the checking set,
		2.1. build a network.
		2.2. For each variable in the input vector, get the error by using weight coming from AncfisNet.
	3. Getting the final error by RMSE
 *******************************************************************/

void AncfisChecking::outputMAE(vector<vector<double>> *weight){

	weightTrn=weight;
	//1.
	checkMatrix=checking.getChecking();

	double aveFinal=0;

	//2.
	for(int indexInV=0; indexInV <numOfInVecChecking; indexInV++){
	//2.1.
		vector<double> & inputVec=(*checkMatrix)[indexInV];
		buildNet(inputVec);

		double ave=0;
	//2.2
		for (int nV=0; nV<numOutput; nV++){

			vector<double> &rw=(*weight)[nV];
			checkError[indexInV][nV]=(*target)[nV]-inVW.Cal_VVS(rw,rpH);
			ave+=abs(checkError[indexInV][nV]);
		}
		aveError[indexInV]=(ave/numOutput);
		aveFinal+=ave;
	}
	//3.
	finalChError=(aveFinal/(numOfInVecChecking*numOutput));

	saveParams();
}
/******************************************************************/
/*******************************************************************
buildNet()

Use:				Get firing strength of each rule for the given input vector by using instance of inputVector and firingStrenght class
Out:				Obtain firing strength of each rule
Status:				Private and called from output()
PreConditions:		inputVec is valid
Postconditions:		input, target and pH and rpH (reference to pH) are filled
Invariant:
 *******************************************************************/
void AncfisChecking::buildNet(vector<double> & inputVec){
	inputV.readData(inputVec);
	input=inputV.readInput();
	target=inputV.readTarget();
	firingStrength fS;
	pH=fS.cal_firingStrenght(input);
}
/******************************************************************/
/*******************************************************************
osaveParams()

Use:				Save final checking error in FinalParamsUsedForChecking.txt
Out:
Status:
PreConditions:
Postconditions:
Invariant:
 *******************************************************************/
void AncfisChecking::saveParams(){
	fstream myfile;

			//throw exception if the file cannot be opened
			myfile.exceptions(ifstream::failbit|ifstream::badbit);
			try{
				myfile.open("FinalParams.txt",ios::app|ios::out);
			}
			catch(fstream::failure &e){
				cerr << "Exception opening/reading/closing file\n";
			}
		myfile<<endl<<"FinalError_Chk:\t\t\t"<<finalChError<<endl;


		/*myfile<<"this is mf used"<<endl;
			for(auto &i :pSinMfParam_allVar){
				for(auto j:*i){
					for(auto k:j){
						myfile<<k<<" ";
					}
					myfile<<endl;
				}
				myfile<<endl;
			}*/
		//myfile<<"******************************************************"<<endl;
		myfile.close();

		fstream myfile3;
		myfile3.exceptions(ifstream::failbit|ifstream::badbit);
		try{
			myfile3.open("FinalError.txt",ios::app|ios::out);
		}
		catch(fstream::failure &e){
			cerr << "Exception opening/reading/closing file\n";
		}
		myfile3<<endl<<"FinalError_Chk:\t\t"<<finalChError<<endl;
		myfile3.close();

		fstream myfile4;
		try{
			myfile4.open("ChkError.txt",ios::app|ios::out);
		}
		catch(fstream::failure &e){
			cerr << "Exception opening/reading/closing file\n";
		}
		myfile4<<endl<<"FinalError_Chk:\t\t"<<finalChError<<endl;
		for (int nV=0; nV <numOutput; nV++ ){

			myfile4<< "output: "<<nV<<endl;

			for (int indexInV=0; indexInV<numOfInVecChecking; indexInV++){
				myfile4 << checkError[indexInV][nV] <<" ";
			}
			myfile4 << endl <<endl;
		}
		myfile4 <<"******************************************************"<<endl;
		myfile4.close();
	}

/******************************************************************/
void output_test(AncfisChecking & AnChk){

	/*cout<<"W coming to checking"<<endl;
	for(auto i: *(AnChk.weightTrn)){
		for(auto j:i){
			cout<< j<<" ";
		}
		cout<<endl;
	}
	cout<<"This is average error of all input vectors"<<endl;
	for(auto i:AnChk.aveError){
		cout<<i<<" ";
	}*/
	cout<<"this is final error= "<<AnChk.finalChError<<endl;

}
