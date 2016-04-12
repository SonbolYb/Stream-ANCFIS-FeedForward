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
	static int numEpoch;
	static double threshold;
	static int numRule;

	static int numCore;
	static double lambdaRLS;
	static double initError;
	static const char* TrainFile1;
	static const char* CheckFile1;
	static int TrainSize;
	static int CheckSize;
	static int per10;
	static long corrlength;
	static int dmax;
	static int numSetDim;
	static double forget;
	static double per10Coeff;


	/*These parameters are assigned in Delay vector class after having the delay vectors
	 * */
	//static int numWeight;
	//static int LengthSurrodata;
	//static int LengthDVSet;
	//static int inputVL;
	//static std::vector<int> lengthofVar;
private:

	std::vector<std::vector<int>> rule;
	void calculateOthers();
public:

	void saveComLine(int, char **);
	void saveParameters();
	std::unique_ptr<std::vector<std::vector<int>>> ruleStruct();
	friend void commandLine_test();


};

#endif /* READCOMLINE_H_ */

void commandLine_test();
