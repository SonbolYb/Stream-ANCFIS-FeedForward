/*
 * timeSeries.cpp
 *
 *  Created on: Jun 10, 2015
 *      Author: sonbol
 */

#include "timeSeries.h"

using namespace std;
/*******************************************************************
timeSeries()

seed: It used in shuffleTraining() method

Note:
	We call two methods in the constructor because after instantiate the time series,
	we want to fill automatically readTraining and readChecking
 *******************************************************************/
timeSeries::timeSeries():commandLine(),trainingSet(numOfInVecTraining,vector<double>(inputVectorLength)),checkingSet(numOfInVecChecking,vector<double>(inputVectorLength)){
	readTraining();
	readChecking();
	seed = std::chrono::system_clock::now().time_since_epoch().count();
}

/*******************************************************************
readTraining()

Use:				It gets name of the file has training set from commandline and save the file in trainingSet (2D vector)
Out:				A 2D vector of training set
Status:				private and called from constructor
PreConditions:		Training file is valid.
					TrainFile is a valid name for the file
Postconditions:		trainingSet is filled
Invariant:

 *******************************************************************/

void timeSeries::readTraining(){

	//Open file
	fstream myfile;

	//throw exception if the file cannot be opened
	myfile.exceptions(ifstream::failbit|ifstream::badbit);

	try{
		myfile.open(TrainFile,ios::in);

	}
	catch(ifstream::failure &e){
		cerr << "Exception opening/reading/closing file\n";
	}

	//save the file
	for (int i=0; i <numOfInVecTraining ;i++){
		for (int j=0; j< inputVectorLength;j++){
			myfile >>trainingSet[i][j];
		}
	}
	//Closing file
	myfile.close();
}
/******************************************************************/
/*******************************************************************
readChecking()

Use:				It gets name of the file has checking set from commandline and save the file in checkingSet (2D vector)
Out:				A 2D vector of checking set
Status:				private and called from constructor
PreConditions:		Checking file is valid.
					CheckFile is a valid name for the file
Postconditions:		checkingSet is filled
Invariant:
 *******************************************************************/
void timeSeries::readChecking(){

	fstream myfile;

	//throw exception if the file cannot be opened
	myfile.exceptions(ifstream::failbit|ifstream::badbit);

	try{
		myfile.open(CheckFile,ios::in);
	}
	catch(ifstream::failure &e){
		cerr << "Exception opening/reading/closing file\n";
	}

	for (int i=0; i <numOfInVecChecking ;i++){
		for (int j=0; j< inputVectorLength;j++){
			myfile >>checkingSet[i][j];
		}
	}
	//Closing file
	myfile.close();
}
/******************************************************************/
/*******************************************************************
getTraining()

Use:				Returns a pointer to the training set
Out:
Status:				Public and called from Cal_outputWeight() in AncfisNet class.
PreConditions:
Postconditions:		trSet pointer is valid
Invariant:
 *******************************************************************/
vector<vector<double>>*  const timeSeries::getTraining(){
	vector<vector<double>>* trSet=&trainingSet;
	return(trSet);
}
/******************************************************************/
/*******************************************************************
getChecking()

Use:				Returns a pointer to the checking set
Out:
Status:				Public and called from output() in AncfisChecking class
PreConditions:
Postconditions:		cheSet pointer is valid
Invariant:

 *******************************************************************/
vector<vector<double>>*  const timeSeries::getChecking(){
	vector<vector<double>>* chSet=&checkingSet;
	return(chSet);
}
/******************************************************************/
/*******************************************************************
shuffleTraining()

Use:				Shuffles training set by using shuffle method in std
Out:
Status:				Public and called from  Cal_outputWeight() in AncfisNet class.
PreConditions:
Postconditions:
Invariant:
 *******************************************************************/

vector<vector<double>>* const timeSeries::shuffleTraining(){

	shuffle(trainingSet.begin(),trainingSet.end(), default_random_engine(seed));

	//TODO: fill here
	return (getTraining());
}

/******************************************************************/

void timeSeries_test(){
	timeSeries t1;
	vector<vector<double>>* ttrain;
	//ttrain=t1.readTimeSeries("datatrn.txt");
	//ttrain=t1.trainingSet;
	cout<<"training data"<<endl;
	ttrain=t1.getTraining();
	for(int i=0; i<8;i++){
		cout <<(*ttrain)[0][i];
	}

}

/*timeSeries::timeSeries():commandLine(),dataMatrix(numOfInVecTraining,vector<double> (inputVectorLength)) {
	// TODO Auto-generated constructor stub
	readTraining();
	trainingSet=NULL;
	checkingSet=NULL;
	//readChecking(); //TODO: copy training file in the directory
	 seed = std::chrono::system_clock::now().time_since_epoch().count();
	 ,
		dataMatrix(numOfInVecTraining,vector<double> (inputVectorLength))
}*/
/*******************************************************************
readTimeSeries

Use:	This method reads training (or checking) file and save it in a matrix.
		It asks for the name of file. We pass the name by using members of commandLine class
		as it has name of training (checking) file as one of its member variables.

Out:	Save the file in a matrix.

Status:	Public member class of timeSeries class and it is called in the main function

PreConditions:
				Filename is a valid file.
				numofInputVectors > 0
				inputVectorLength > 0


Postconditions:
				dataMatrix is generated whose dimension is (numofInputVector,inputVectorLength)
				numofInputVector = numOfInVecTraining 	if reading training file
				numofInputVector = numOfInVecChecing 	if reading checking file

Invariant:		numofInvecTraining, numofInVecChecking,inputVectorLength

 *******************************************************************/
/*vector<vector<double>>* timeSeries::readTimeSeries(const char *Filename){

	int numofInputVectors=0;

	//number of input vectors for training and checking can be different:

	if(strcmp(Filename,TrainFile)==0){
		numofInputVectors=numOfInVecTraining;
	}
	else if(strcmp(Filename,CheckFile)==0){
		numofInputVectors=numOfInVecChecking;
	}
	else{
		cout << "please check name of files";
		exit (1);
	}

	// the returned matrix's dimension is different based on working with training or checking
	vector<vector<double>> dataMatrix(numofInputVectors,vector<double> (inputVectorLength));

	//Open file
	fstream myfile;

	//throw exception if the file cannot be opened
	myfile.exceptions(ifstream::failbit|ifstream::badbit);

	try{
		myfile.open(Filename,ios::in);
		//	while (!myfile.eof()) myfile.get();
		//	myfile.close();
	}
	catch(ifstream::failure &e){
		cerr << "Exception opening/reading/closing file\n";
	}

	//save the file in the dataMatrix
	for (int i=0; i <numofInputVectors ;i++){
		for (int j=0; j< inputVectorLength;j++){
			myfile >>dataMatrix[i][j];
		}
	}

	//Closing file
	myfile.close();

	vector<vector<double>> *train=&dataMatrix;

	return (train);
}*/
/******************************************************************/
//	trainingSet=readTimeSeries(TrainFile);
//vector<vector<double>>* trSet=&trainingSet;
//trSet=&trainingSet;
//trSet=readTimeSeries(TrainFile);
//return (trainingSet);

//checkingSet=readTimeSeries(CheckFile);
//return (checkingSet);
