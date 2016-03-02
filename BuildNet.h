/*
 * BuildNet.h
 *
 *  Created on: Feb 18, 2016
 *      Author: sonbol
 */

#ifndef BUILDNET_H_
#define BUILDNET_H_
#include "header.h"

class BuildNet {
public:
	BuildNet();
	void build(std::vector<std::vector<double>> *, std::vector<double>*);
	void updataWeight(std::vector<double>*);
	virtual ~BuildNet();
};

#endif /* BUILDNET_H_ */
