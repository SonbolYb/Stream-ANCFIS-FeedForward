/*
 * AncfisNet.h
 *
 *  Created on: Jun 16, 2015
 *      Author: sonbol
 */

#ifndef ANCFISNET_H_
#define ANCFISNET_H_

#include "header.h"
#include "timeSeries.h"
#include "inputVecWeight.h"


class AncfisNet: public commandLine {

	private:
	timeSeries training;
	inputVecWeight inputVectorWeight;
	inputVecWeight & rinputVectorWeight;

	std::vector<std::vector<double>>* trainMatrix;

public:
	AncfisNet();
	std::vector<std::vector<double>>* Cal_outputWeight();
};

#endif /* ANCFISNET_H_ */
