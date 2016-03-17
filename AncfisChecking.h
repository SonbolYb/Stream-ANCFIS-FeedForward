/*
 * AncfisChecking.h
 *
 *  Created on: Jul 9, 2015
 *      Author: sonbol
 */

#ifndef ANCFISCHECKING_H_
#define ANCFISCHECKING_H_

#include"header.h"
#include "inputVector.h"
#include "firingStrength.h"
#include "commandLine.h"
#include "timeSeries.h"
#include "inputVecWeight.h"

class AncfisChecking: public commandLine {

private:
	timeSeries checking;
	inputVector inputV;
	inputVecWeight inVW;

	std::vector<std::vector<double>>* checkMatrix;
	std::vector<double> * input;
	std::vector<double> *target;
	std::unique_ptr<std::vector<double>> pH;
	std::unique_ptr<std::vector<double>> & rpH;
	std::vector<std::vector<double>> checkError;
	std::vector<double> aveError;
	double finalChErroralter=0;
	double finalChErrorMain=0;
	std::vector<std::vector<double>> *weightTrn;


public:

	AncfisChecking();
	void output (std::vector<std::vector<double>>*);
	void buildNet(std::vector<double> &,int);
	void saveParams();
	friend void output_test(AncfisChecking &);
};
void output_test(AncfisChecking &);
#endif /* ANCFISCHECKING_H_ */
