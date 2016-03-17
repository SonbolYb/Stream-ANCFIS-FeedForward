/*
 * GetDelayDImension.h
 *
 *  Created on: Feb 17, 2016
 *      Author: sonbol
 */

#ifndef GETDELAYDIMENSION_H_
#define GETDELAYDIMENSION_H_
#include "Dimension.h"
#include "Delay.h"
#include "header.h"
//#include "Delay.h"
//#include "Dimension.h"
//class Delay;
class GetDelayDImension : public commandLine{
public:
	GetDelayDImension();
	int getDelay();
	int getDimension();
	void findDDim(std::vector<std::vector<double>> *);

		virtual ~GetDelayDImension();
private:
	Delay * del;
	Dimension dim;
	std::vector<std::vector<double>> * input;
	std::vector<double> * delay;
	std::vector<double> *dimension;

};

#endif /* GETDELAYDIMENSION_H_ */
