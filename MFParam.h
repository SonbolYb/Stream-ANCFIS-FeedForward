/*
 * MFParam.h
 *
 *  Created on: Feb 18, 2016
 *      Author: sonbol
 */

#ifndef MFPARAM_H_
#define MFPARAM_H_

#include "header.h"
#include "InputStream.h"
class MFParam: public commandLine {
public:
	MFParam();
	std::vector<std::vector<std::vector<double>>> *getMfparam(std::vector<std::vector<double>> *);
	std::vector<std::vector<double>>* getPower();
	std::vector<std::vector<std::vector<double>>> * initialMf();
	virtual ~MFParam();
private:
	std::vector<std::vector<std::pair<double,double>>> prms;
	std::vector<std::vector<std::pair<double,double>>> MFparam;
	std::vector<std::vector<std::vector<double>>> MFparam1;
	std::vector<std::vector<double>> power;
	void calMFparam(std::vector<std::vector<double>> *);
	void initiateA();
	void initiateB();
	void initiateCD();
	double aS=0;
	double bS=0;
	double cS=0;
	double dS=0;
	std::vector<std::vector<std::vector<double>>> pSinMfParam_allVar;
friend void mfpramtest();
};

void mfparamtest();
#endif /* MFPARAM_H_ */
