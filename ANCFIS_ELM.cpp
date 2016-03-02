//============================================================================
// Name        : ANCFIS_ELM.cpp
// Author      : Sonbol
// Version     :Spring 2015
//Description:
//	This is a new version of ANCFIS with new learning algorithm. This new algorithm is based on Extreme Learning Machine.
//	Actually we have two sets of parameters: {a,b,c,d} of membership functions that we determine them randomly from a continuous probability
//	distribution and output weights (antecedent params) that we determine them by RLS (Recursive Least Square).
//	 Moreover, ANCFIS is online and the output weights are selected sequentially
//	This is the main function. This function has the following procedure:
//	1. Save variates of a multivariate time series
//	2. Membership grades of each variates
//	3. Firing strength of each rule
//	4. Normalization of each rule
//	5. Dot product to convert the complex grades to real values
//	6. Sequential learning to obtain the output weights
//	7. Output

// Two commands need to be changed depends on if we want RMSE or MAE.
//	1. checking.outputRMSE in ANCFIS_ELM
//  2. inputVectorWeight.calError in AncfisNetRMSE
//============================================================================

#include <iostream>
#include "header.h"
#include "mfGrade.h"

#include "commandLine.h"
#include "timeSeries.h"
#include"inputVector.h"
#include"AncfisNet.h"
#include"firingStrength.h"
#include"AncfisChecking.h"
#include "InputStream.h"
#include"DelayVector.h"
#include "Delay.h"
#include "BuildANCFIS.h"
#include "Dimension.h"




using namespace std;

//void readCL(int, char **);
//void unitTest1();
//void unitTest2(timeSeries );

int main(int argc, char * argv[]) {
	setbuf(stdin,NULL);
	setbuf(stdout,NULL);
	const clock_t begin=clock();
	vector<vector<double >> * finalWeight;
	int finalDim=0;
	int finalDelay=0;
	vector<double> * finalMf;


	//Process command line arguments
	commandLine ComLine;

	ComLine.saveComLine(argc,argv);
	//	commandLine_test();


	InputStream inS;
	testInputStream();
	vector <int> delay {2,2};
	DimTest(inS.getOrigWindowN(),&delay);
	DV_test();
	testDelay();

	BuildANCFIS Ancfis;
	finalWeight=Ancfis.getFinalWeight();
	finalDim=Ancfis.getDimension();
	finalDelay=Ancfis.getDelay();
	finalMf=Ancfis.getMf();

	/*	InputStream inS;
		inS.read1stInput();
		GetDelayDImension DDim;
		DelayVector DV;
		double delay=0.0;
		double dimension=0.0;
		DDim.findDDim(inS.getOrigWindowN(),&delay,&dimension);
		DV.getDVV(inS.getOrigWindowN(),delay,dimension);

	 */
	//inS.endtoend();
	//cout << "this is endtoend length"<<inS.endtoendLength<<endl;
	//	testInputStream();
	//	testDelay();
	//commandLine_test();

	/*std::vector<std::vector<double>>*finalWeight;
		AncfisNet Ancfis;
		finalWeight=Ancfis.Cal_outputWeight();
		AncfisChecking checking;

		//Change to outpuRMSE if you want to work with RMSE
		checking.outputMAE(finalWeight);
		output_test(checking);

		fstream myfile;
		myfile.open("FinalParams.txt",ios::app|ios::out);
		myfile<<"time spent:\t\t\t"<< (float( clock () - begin ) /  CLOCKS_PER_SEC)<<endl;
		myfile<<"******************************************************"<<endl;
		myfile.close()*/;
		//std::cout << float( clock () - begin ) /  CLOCKS_PER_SEC;
		return 0;
}

/*
	//Variables
	int epoch =0;

	Process command line arguments
	commandLine ComLine;
	ComLine.saveComLine(argc,argv);

	//Define Variables after reading command line;
	vector<vector<double>> trainingData((ComLine.GetnumOfInVTr()),vector<double> (ComLine.GetinputVL()));	//matrix to save training time series. Its dimesions
																											//are obtained from variables obtained by commandline

	  //Save training file in trainingdata matrix
	 timeSeries training;
	  trainingData=training.readTimeSeries(ComLine.GetTrainFile());
	 // vector<double> & td=trainingData[0];
	// double *td=trainingData[0];
	//  double *td=&(trainingData[0][0]);



	  commandLine_test();
	  timeSeries_test();

	  for (int ep=0; ep<2;ep++){ //(TODO;ep<100)

		  //Shuffle the matrix before going to the next class
		  if (ep!=0){
			  randomShuffle(&trainingData);
		  }
		  vector<double> * td=&(trainingData[ep]);
		  inputVector inputV(td);
		  inputVector & inpp=inputV;
		  inputVector_test(inpp);
		  vector<double> & td=(trainingData[ep]);
		  		  inputVector inputV(td);
		  		  inputVector & inpp=inputV;
		  		 inputVector_test(inpp);


		  		  vector<double> &td2=trainingData[3];
		  		  inputVector inputV2(td2);
		  		  inputVector & inpp2=inputV2;
		  		  inputVector_test(inpp2);
	  }


	//Make an instance of timeSeries class and save datatrn.txt there
	timeSeries1 training;
vector <double> dataTrnInput;
	vector<double> dataTrnTarget;

	training.savetimeSeries1("datatrn.txt");
	dataTrnInput=training.readInput();
	dataTrnTarget=training.readTarget();
	unitTest_timeSeries1(training);

	unordered_map<int,vector<vector<double>>> dataTrn;
	vector<complex <double>>fV;

//unitTest1();

	 Make an instance of timeSeries class and save datatrn.txt there
 * readTimeSeries works based on map and you can access variates by key
 * saveTimeSeries, save data in a vector and it is faster.
	//dataTrn=training.readTimeSeries("datatrn.txt");






vector < vector <double>> v3= dataTrn.at(0);
for (auto z:v3){
	for(auto j: z){
		cout <<j;
	}
	cout<< endl;
}

vector<vector<complex<double>>> fv;
//for (int i=0; i < 1;i++){
//vector<vector<double>> &trn=dataTrn1[0];

Make a membership grade instance
	mfGrade v1;
	v1.MemFungrade(dataTrn1[0]);
	for (auto i:fv){
		for (auto j:i){
			cout <<j;
		}
		cout <<endl;
	}

//}
 */


