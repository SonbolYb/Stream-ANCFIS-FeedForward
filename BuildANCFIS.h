/*
 * BuildANCFIS.h
 *
 *  Created on: Feb 18, 2016
 *      Author: sonbol
 */

#ifndef BUILDANCFIS_H_
#define BUILDANCFIS_H_
#include "Dimension.h"
#include "header.h"
#include "BuildNet.h"
#include "MFParam.h"
#include "DelayVector.h"
#include "Delay.h"
#include "Dimension.h"
#include "InputStream.h"



class BuildANCFIS : public commandLine{
public:

	BuildANCFIS();
	std::vector<std::vector<double>> * getFinalWeight();
	std::vector<int>* getDimension();
	std::vector<int>* getDelay();
	std::vector<std::vector<std::vector<double>>>* getMf();
	int getLengthSurodata();
	virtual ~BuildANCFIS();
private:
	void findWeight();
	void findDV();
	void findMFParam();
	void saveDDMF();
	InputStream InS;
	DelayVector DV;
	Delay del;
	MFParam mfPar;
	Dimension dim;
	BuildNet buildNet;
	int numPassInV=0;
	std::vector<std::vector<double>> * inputOrigin;
	std::vector<std::vector<double>> * surodata;
	std::vector<std::vector<double>> * delayVectors;
	std::vector<double> *newDV;
	std::vector<std::vector<std::vector<double>>>* mfParam;
	std::vector<int>* dimension;
	std::vector<int>* delay;
	std::vector<double>* newData;
	std::vector<std::vector<double>> * finalWeight;
	int LengthSurodata=0;
	int LengthDVSet=0;
	std::vector<int> delay1={23};
	std::vector<int> dimension1={12};

};

#endif /* BUILDANCFIS_H_ */
