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
#include "InputStream.h"
#include "inputVecWeight.h"
#include "DelayVector.h"

class AncfisChecking: public commandLine {

private:
	InputStream checking;
	inputVector inputV;
	inputVecWeight inVW;
	DelayVector DV;

	std::vector<std::vector<double>>* checkMatrix;
//	std::unique_ptr<std::vector<double>>& input;
	//std::unique_ptr<std::vector<double>>& target;
	std::unique_ptr<std::vector<double>> target;
	std::unique_ptr<std::vector<double>> pH;
	std::unique_ptr<std::vector<double>> & rpH;
	std::vector<std::vector<double>> checkError;
	std::vector<double> aveError;
	double finalChErroralter=0;
	double finalChErrorMain=0;
	std::vector<std::vector<double>> *weightTrn;
	std::vector<std::vector<double>>* checkingDV;
	int numOfInVecChecking=0;
	int numWeight=0;
	std::vector<int>* dimension;
	int lengthSurdata=0;


public:

	AncfisChecking();
	virtual ~AncfisChecking();
	void output (std::vector<std::vector<double>>*,std::vector<int> *,std::vector<int>*,std::vector<std::vector<std::vector<double>>> *,int);
	void buildNet(std::vector<double> &,int,std::vector<std::vector<std::vector<double>>> *);
	void saveParams();
	friend void output_test(AncfisChecking &);
};
void output_test(AncfisChecking &);
#endif /* ANCFISCHECKING_H_ */
