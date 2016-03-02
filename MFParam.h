/*
 * MFParam.h
 *
 *  Created on: Feb 18, 2016
 *      Author: sonbol
 */

#ifndef MFPARAM_H_
#define MFPARAM_H_

#include "header.h"
class MFParam {
public:
	MFParam();
	void findMfparam(std::vector<std::vector<double>> *);
	std::vector<double> *getMf();
	virtual ~MFParam();
};

#endif /* MFPARAM_H_ */
