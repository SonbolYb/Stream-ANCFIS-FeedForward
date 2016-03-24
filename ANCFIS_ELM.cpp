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
#include "commandLine.h"
#include "Delay.h"
#include "Dimension.h"
#include "InputStream.h"
#include "mfGrade.h"
#include"inputVector.h"
#include"firingStrength.h"
#include"AncfisChecking.h"
#include "InputStream.h"
#include"DelayVector.h"
#include "MFParam.h"
#include "BuildNet.h"
#include "BuildANCFIS.h"

using namespace std;

int main(int argc, char * argv[]) {
	//TODO: test MF params
	setbuf(stdin,NULL);
	setbuf(stdout,NULL);
	const clock_t begin=clock();
	vector<vector<double >> * finalWeight;

	//Process command line arguments
	commandLine ComLine;
	ComLine.saveComLine(argc,argv);
	commandLine_test();
	BuildANCFIS Ancfis;

	finalWeight=Ancfis.getFinalWeight();
	cout << "this is final out weight"<<endl;
	for (auto i:(*finalWeight)){
		for (auto j:i){
			cout << j<< " ";
		}
	}
	vector<int>* finalDim=Ancfis.getDimension();
	vector<int>* finalDelay=Ancfis.getDelay();
	vector<vector<vector<double>>>* finalMf=Ancfis.getMf();
	AncfisChecking checking;
	checking.output(finalWeight,finalDelay,finalDim,finalMf);
	//	commandLine_test();
		//InputStream inS;
	//	testInputStream();
		//DimTest(inS.getOrigWindowN(),&delay);
		//DV_test();
		//testDelay();
	//	mfparamtest();
	std::cout << float( clock () - begin ) /  CLOCKS_PER_SEC;
		return 0;
}

