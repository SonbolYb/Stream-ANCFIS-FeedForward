/*
 * mfGrade.h
 *
 *  Created on: Jun 19, 2015
 *      Author: sonbol
 */

#ifndef MFGRADE_H_
#define MFGRADE_H_
#include"header.h"

class mfGrade:public commandLine {
private:



	std::vector<std::unique_ptr<std::vector<std::complex<double> >>> pConvolution;
	std::vector<std::unique_ptr<std::vector<std::complex<double> >>>  pNormalize;

	//void convolutionFn(const std::vector<double>& inputVector); //one input vector comes to convolution (one by one)
	void convolutionFn(std::vector<double>* const inputVector); //one input vector comes to convolution (one by one)
	void elliotFn();//Returning normalized mf by Elliot function. Limited to a unit disk

public:
	mfGrade();
	std::vector<std::unique_ptr<std::vector<std::complex<double> >>> MemFungrade( std::vector<double> * const inputVector);

	//friend void mfGrade_test(const std::vector<double> &);
//	friend void mfGrade_test(std::vector<double> *);
	friend void mfGrade_test(mfGrade &);
};

//void mfGrade_test(const std::vector<double> &);
void mfGrade_test(mfGrade &);

#endif /* MFGRADE_H_ */

//std::vector<double> & inputVector;
//std::vector<std::unique_ptr<std::vector<double>>> myContainer;
//std::vector<std::shared_ptr<std::complex<double> []>> myContainer;
//double sinFunction(std::vector<double>);	//Calculating sinusoidal membership function with a,b,c,d
//double samplingSin(double);					//returning sampling of the sinusoidal function
//std::vector< std::vector<std::complex<double>>> convolutionFn(std::vector<double> inputVector);	//Calculate the convolution to get complex membership function
//std::vector< std::vector<std::complex<double>>> elliotFn(std::complex<double>);
