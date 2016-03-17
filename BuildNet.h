/*
 * BuildNet.h
 *
 *  Created on: Feb 18, 2016
 *      Author: sonbol
 */

#ifndef BUILDNET_H_
#define BUILDNET_H_
#include "header.h"
#include "inputVecWeight.h"

class BuildNet:public commandLine {
public:
	BuildNet();
	std::vector<std::vector<double>>* build(std::vector<std::vector<double>> *, std::vector<std::vector<std::vector<double>>>*);
	std::vector<std::vector<double>> * getOutputWeight();
	void updataWeight(std::vector<double>*);
	inputVecWeight inputVectorWeight;
	virtual ~BuildNet();
private:
	unsigned seed;
	void shuffleDV(std::vector<std::vector<double>> *);
};

#endif /* BUILDNET_H_ */
