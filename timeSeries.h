/*
 * timeSeries.h
 *
 *  Created on: Jun 10, 2015
 *      Author: sonbol
 *
 *      This class is a child of commandLine class.
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include "commandLine.h"
#include "header.h"

class timeSeries: public commandLine {
private:
	std::vector<std::vector<double>> trainingSet;
	std::vector<std::vector<double>> checkingSet; //TODO: Call file in the directory

	void readChecking();
	void readTraining();
	unsigned seed;

public:
	timeSeries();

	void charaTimeSeries();		//TODO:calculate average, min and max of time series
	std::vector<std::vector<double>> * const getTraining();
	std::vector<std::vector<double>> * const getChecking();

	std::vector<std::vector<double>>* const shuffleTraining();
	void friend timeSeries_test();


};

void timeSeries_test();
#endif /* TIMESERIES_H_ */
