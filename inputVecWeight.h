/*
 * inputVecWeight.h
 *
 *  Created on: Jun 17, 2015
 *      Author: sonbol
 */

#ifndef INPUTVECWEIGHT_H_
#define INPUTVECWEIGHT_H_
#include "header.h"
#include "inputVector.h"
#include "firingStrength.h"

class inputVecWeight: public commandLine {
private:
	inputVector inputV;

	//std::unique_ptr<std::vector<double>>& input;
	std::unique_ptr<std::vector<double>> target;


	std::unique_ptr<std::vector<double>> pH;
	std::unique_ptr<std::vector<double>>& rpH;
	std::vector<std::unique_ptr<std::vector<std::vector<double>>>> pM_new;
	std::vector<std::unique_ptr<std::vector<std::vector<double>>>> pM_old;
	std::vector<std::vector<double>> G;
	std::vector<double> alpha;
	std::vector<std::unique_ptr<std::vector<double>>> pW_old;
	std::vector<std::unique_ptr<std::vector<double>>> pW_new;


	std::vector<std::vector<double>> errorEpochLastdata;	//Error of the last input vector for each epoch
	//std::vector<double> bestErrorLastdata;
	double bestErrorLastdata=0;
	std::vector<double> aveErrorLastdata;
	std::vector<double> aveErrorEpoch;
	double bestaveFinaAlter=0;
	double aveBest;
	int epochBest;
	std::vector<std::vector<double>>* vectorTrainBestEpoch;
	std::vector<std::vector<double>>  bestW;
	std::vector<std::vector<std::vector<double>>> bestPm;
	std::vector<double> bestFS;
	double aveFinalMain=0.0;	//average error of training for one epoch
	double aveFinalalter=0.0;
	std::vector<std::vector<std::vector<double>>> trainingErrorAll; //for training error of all the input vectors in all the epochs
	int itera=0;
	std::vector<double> ErrorStream;
	//int iterat=0;

	int LengthDVSet=0;

	//std::vector<std::vector<double>* >   rbestW;
	//std::vector<std::vector<std::vector<double>>*> bestM;

	//firingStrength fS;

	void identityM();
	void RLS();

	void CalPn();
	void Cal_MV(std::unique_ptr<std::vector<std::vector<double>>> &,std::unique_ptr<std::vector<double>>&,std::vector<double> &);
	void Cal_VM(std::unique_ptr<std::vector<double>>&,std::unique_ptr<std::vector<std::vector<double>>> &,std::vector<double> &);
	void Cal_VVM(std::vector<double> &,std::vector<double> &, std::vector<std::vector<double>> &);

	void Cal_MtoS(std::vector<std::vector<double>> &,double,std::vector<std::vector<double>> &);
	void Cal_VtoS(std::vector<double> &,double,std::vector<double> &);
	std::unique_ptr<std::vector<std::vector<double>>>  Cal_Msub(std::unique_ptr<std::vector<std::vector<double>>> &,std::vector<std::vector<double>>& );

	void CalGn();
	void CalAlpha();

	void CalW();
	std::unique_ptr<std::vector<double>> Cal_Vadd( std::unique_ptr<std::vector<double>>&,std::vector<double> &);

	void Replace_Old_New();
public:
	double Cal_VVS(std::vector<double> &,std::unique_ptr<std::vector<double>>&);
	inputVecWeight();
	virtual ~inputVecWeight();
	void calculateWeight(const std::vector<double> &,int,std::vector<std::vector<std::vector<double>>> *,std::vector<int>*,int,int);
	void calError(int epoch);
	double getAveBest();
	std::vector<std::vector<double>> * getWBest();
	//std::vector<std::vector<double>*> * rgetWBest();
	void saveParams(int);
	void saveStreamParams();

	friend void inputWeight_test1(inputVecWeight &);
	friend void calError_test(inputVecWeight &,int);
	void calErrorTr(int epoch);
	void resetValueforUpdateWeight();
	void calErrorStream();
	void resetVectors();
	int numWeight=0;

};
void inputWeight_test1(inputVecWeight &);
void calError_test(inputVecWeight &,int );


#endif /* INPUTVECWEIGHT_H_ */
