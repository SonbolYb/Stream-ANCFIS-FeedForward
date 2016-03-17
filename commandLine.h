/*
 * commandLine.h
 *
 *  Created on: Jun 8, 2015
 *      Author: sonbol
 *
 *
 *   The class is base class for other classed
 */

#ifndef READCOMLINE_H_
#define READCOMLINE_H_

#include "header.h"

class commandLine {

protected:

	static int numVariate;
	static int numOutput;
	static std::vector<int> numOfMF;
	static std::vector<int> lengthOfVariate;
	static int inputVectorLength;
	static int inputlength;
	static int targetlength;
	static int ChunkSize;
	static int numOfInVecTraining;
	static int numOfInVecChecking;
	static const char *TrainFile;
	static const char *CheckFile;
	static int numEpoch;
	static double threshold;
	static int numRule;
	static int numParamMF;
	static int numCore;
	static double lambdaRLS;
	static double initError;
	static std::vector<double> aRange;
	static std::vector<double> bRange;
	static std::vector<std::unique_ptr<std::vector<std::vector<double>>>> pSinMfParam_allVar;
	static double aS;
	static double bS;
	static double cS;
	static double dS;

	static const char* TrainFile1;
	static const char* CheckFile1;
	static int TrainSize;
	static int CheckSize;
	static int per10;
	static long corrlength;
	static int dmax;
	static int numSetDim;
	static double forget;
	static int numWeight;
	static int LengthSurrodata;
private:

	std::vector<std::vector<int>> rule;
	void calculateOthers();
	void initialMf();
	void initiateA();						//initiate A with random number
	void initiateB();						//Initiate B with random number
	void initiateCD();		//Initiate c and d by satisfying the condition between them
public:

	void saveComLine(int, char **);
	void saveParameters();
	std::shared_ptr<std::vector<std::vector<int>>> ruleStruct()const;
	friend void commandLine_test();


};

#endif /* READCOMLINE_H_ */

void commandLine_test();
