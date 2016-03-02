/*
 * PassInputtoDV.h
 *
 *  Created on: Feb 17, 2016
 *      Author: sonbol
 */

#ifndef PASSINPUTTODV_H_
#define PASSINPUTTODV_H_
#include "header.h"
#include "InputStream.h"
class PassInputtoDV : public commandLine{
public:
	InputStream inS;
	void inputToDV();
	int inputPased=0;

	PassInputtoDV();
	virtual ~PassInputtoDV();
};

#endif /* PASSINPUTTODV_H_ */
