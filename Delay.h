/*
 * Delay.h
 *
 *  Created on: Jan 28, 2016
 *      Author: sonbol
 */

#ifndef DELAY_H_
#define DELAY_H_
#include"header.h"
#include "InputStream.h"
/*extern "C"{
void mutual();
double make_cond_entropy(long t);
}*/
class Delay: public commandLine {
public:
	Delay();
	virtual ~Delay();
	std::vector<int>* getDelay(std::vector<std::vector<double>> *);
	std::vector<int> delays; 	//A vector of delay for multivariate time series

private:

	void findDelay();
	double make_cond_entropy(long t);
	void mutual();
	void calDelay();
	long partitions=16;		//I should define it somewhere
	std::vector<std::vector<double>> mutualArray;
	long *array,*h1,*h11,**h2;
	//TODO: It should go to the commandline
//	long corrlength=20;
	unsigned long length=per10;
	std::vector<std::vector<double>> *pOrigWindow;



	friend void testDelay();
};
void testDelay();
#endif /* DELAY_H_ */
