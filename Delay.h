/*
 * Delay.h
 *
 *  Created on: Jan 28, 2016
 *      Author: sonbol
 */

#ifndef DELAY_H_
#define DELAY_H_
#include"header.h"


class Delay: public commandLine {
public:
	Delay();
	virtual ~Delay();
	std::vector<int>* getDelay( const std::vector<std::vector<double>> * );
	std::vector<int> delays; 	//A vector of delay for multivariate time series

private:

	void findDelay();
	double make_cond_entropy(long t);
	void mutual();
	void calDelay();
	//TODO: define partitions some where
	long partitions=16;
	std::vector<std::vector<double>> mutualArray;
	long *array,*h1,*h11,**h2;
	unsigned long length=per10;
	std::vector<std::vector<double>> pOrigWindow;



	friend void testDelay();
};
void testDelay();
#endif /* DELAY_H_ */
