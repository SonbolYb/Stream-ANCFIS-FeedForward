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
#include "GetDelayDImension.h"
#include "MFParam.h"
#include "DelayVector.h"
#include "Delay.h"
#include "Dimension.h"
#include "InputStream.h"



class BuildANCFIS : public commandLine{
public:

	BuildANCFIS();
	std::vector<std::vector<double>> * getFinalWeight();
	int getDimension();
	int getDelay();
	std::vector<double>* getMf();
	virtual ~BuildANCFIS();
private:
	void findWeight();
	void findDV();
	void findMFParam();
	InputStream InS;
	DelayVector DV;
	Delay del;
	MFParam mfPar;
	Dimension dim;
	GetDelayDImension DDim;
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


};

#endif /* BUILDANCFIS_H_ */
