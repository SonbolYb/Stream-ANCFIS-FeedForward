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
AncfisChecking::AncfisChecking():commandLine(),checkMatrix(NULL),rpH(pH),checkError(1),
		aveError(1),weightTrn(NULL),checkingDV(NULL),dimension(NULL){
//	cout<<"AncfisChecking1"<<endl;

}
AncfisChecking::~AncfisChecking(){
//	cout<<"AncfisChecking2"<<endl;
}
/******************************************************************/
/*******************************************************************
output()

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

void AncfisChecking::output(vector<vector<double>> *weight,vector<int>* delay, vector<int>* dim,vector<vector<vector<double>>>* mfParam,int lengthsurrodata){
	lengthSurdata=lengthsurrodata;
dimension=dim;
	int sumDim=0;
	for(auto i:*dimension){
		sumDim+=i;
	}
numWeight=numRule*sumDim+numRule;
	weightTrn=weight;
	//1.
	checkMatrix=checking.getCheckingN();
//	checkMatrix=checking.getChecking();
	checkingDV=DV.getDV(checkMatrix,delay,dimension);
	double aveFinal=0;
	double aveFinalalter=0;
	numOfInVecChecking=checkingDV->size();
	aveError.resize(numOfInVecChecking);
	checkError.resize(numOfInVecChecking);
	int j=0;
	for(auto i:checkError){
		checkError[j].resize(numOutput);
		j++;
	}
	//2.
	for( int indexInV=0; indexInV < numOfInVecChecking; indexInV++){
	//2.1.
		vector<double> & inputVec=(*checkingDV)[indexInV];
		buildNet(inputVec,indexInV,mfParam);

		double ave=0;
	//2.2

		for (int nV=0; nV<numOutput; nV++){

			vector<double> &rw=(*weight)[nV];

			double result=0;

				for(int i=0;i< numWeight;i++){
					result+=rw[i]*(*rpH)[i];
				}

			checkError[indexInV][nV]=(*target)[nV]-result;
		//	cout<<result<<" ";
			ave+=pow(checkError[indexInV][nV],2);
			//cout <<(*target)[nV]<< "  "<<inVW.Cal_VVS(rw,rpH)<<endl;
		}
		aveError[indexInV]=sqrt(ave/numOutput);
		aveFinal+=ave;
		aveFinalalter+=aveError[indexInV];
	}
	//3.
	finalChErrorMain=sqrt(aveFinal/(numOfInVecChecking*numOutput));
	finalChErroralter=aveFinalalter/numOfInVecChecking;
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
void AncfisChecking::buildNet(vector<double> & inputVec, int iter,vector<vector<vector<double>>> * mfParam){
	inputV.readData(inputVec,dimension);
	std::unique_ptr<std::vector<double>> input=move(inputV.readInput());
	target.reset();
	target=move(inputV.readTarget());
	firingStrength fS;
	pH=fS.cal_firingStrenght(move(input),iter,mfParam,dimension,lengthSurdata);
	/*for(auto i:*pH){
			cout<<i<<" ";
		}
		cout<<endl;*/

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
		myfile<<endl<<"FinalError_Alter_Chk:\t\t"<<finalChErroralter<<endl;
	myfile<<"FinalError_Chk:\t\t\t"<<finalChErrorMain<<endl;
	
	
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

	myfile3<<endl<<"FinalError_Chk:\t\t"<<finalChErrorMain<<endl;

	myfile3.close();
	
	fstream myfile4;
	try{
		myfile4.open("ChkError.txt",ios::app|ios::out);
	}
	catch(fstream::failure &e){
		cerr << "Exception opening/reading/closing file\n";
	}
	myfile4<<endl<<"FinalError_Chk:\t\t"<<finalChErrorMain<<endl;
	for (int nV=0; nV <numOutput; nV++ ){
		
		myfile4<< "output: "<<nV<<endl;
		
		for (int indexInV=0; indexInV<numOfInVecChecking; indexInV++){
			myfile4 << checkError[indexInV][nV] <<" ";
		}
		myfile4 << endl <<endl;
	}
	myfile4 <<"******************************************************"<<endl;
	myfile4.close();
	cout<<"this is final error chk=  "<<finalChErrorMain<<endl;
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
	cout<<"this is final alter eror chk="<<AnChk.finalChErroralter<<endl;
	cout<<"this is final error chk= "<<AnChk.finalChErrorMain<<endl;


}
