/*
 * inputVector.h
 *
 *  Created on: Jun 10, 2015
 *      Author: sonbol
 *
 *      This class is a child class of commandLine.
 *
 *      It has a member variable that is a reference because we want to use a dataset from main and we want to prevent copying it, so pass it by refrence.
 *
 */

#ifndef INPUTVECTOR_H_
#define INPUTVECTOR_H_
#include "header.h"


class inputVector: public commandLine {
private:

	std::unique_ptr<std::vector<double>> input;
	std::unique_ptr<std::vector<double>> target;
	int inputVL=0;
/*	std::vector<double> & pinput;
	std::vector<double> & ptarget;*/
public:


	inputVector();
	virtual ~inputVector();
	std::unique_ptr<std::vector<double>>&   readInput();
	std::unique_ptr<std::vector<double>> & readTarget();
	void readData(const std::vector<double>&,std::vector<int>*);
	friend void inputVector_test(std::vector<double> );
	/*	std::vector<double> & readInput();
		std::vector<double>& readTarget();*/

};

void inputVector_test(std::vector<double>);

#endif /* INPUTVECTOR_H_ */
