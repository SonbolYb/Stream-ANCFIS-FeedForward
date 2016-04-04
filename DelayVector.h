/*
 * DelayVector.h
 *
 *  Created on: Jan 28, 2016
 *      Author: sonbol
 */

#ifndef DELAYVECTOR_H_
#define DELAYVECTOR_H_
#include "header.h"
#include "commandLine.h"
#include <flann/flann.hpp>
#include <flann/algorithms/dist.h>
#include <flann/algorithms/kdtree_index.h>
#include <flann/util/params.h>


class DelayVector:public commandLine {
public:
	DelayVector();
	virtual ~DelayVector();
	//void getDelayVector();
	std::vector<double>* getNewDV(std::vector<std::vector<double>>*, std::vector<int>*, std::vector<int>*,std::vector<int>*);
	std::vector<std::vector<double>>* getNewDelayVecs(std::vector<std::vector<double>>*, std::vector<int>*, std::vector<int>*,std::vector<int>*);

	int getsize();

	//InputStream inS;
	//void getDV(Delay , Dimension );
	//void findDV(std::vector<std::vector<double>> *, int, int);
	virtual std::vector<std::vector<double>>* getDV(std::vector<std::vector<double>> *, std::vector<int>*, std::vector<int>*);


	bool compare(double,double);
	int getLengthDVSet();
protected:
	//void delay();
	//void dimension();

	void deletOldDV();
	//void getInput();
	 void findDur();

	virtual void makeDV();

	virtual void resizeDV();

	void addNewDV(std::vector<double>*);
  void findNewDV(std::vector<int>*);
	void replaceNewDV();



/*	extern "C"{
	void mutual();
	double make_cond_entropy(long t);
	double * get_series();
	void rescale_data();
	}*/
	std::vector<std::vector<double>> *pOrigWindow;
	std::vector<std::vector<double>> *input;
	std::vector<double> duration;
	std::vector<std::vector<double>> delV;
	std::vector<std::vector<double>> delVDim;
	std::vector<int> *dimension;
	std::vector<int> *delay;
	int maxDur=0;
	int size=0;
	int sumDim=0;
	std::vector<std::vector<double>> output;
	std::vector<double> delayVecNew;
	std::vector<double>* headDV;
	std::vector<double>*endDV;
	int headInxDV=0;
	int endInxDV=0;
	int LengthDVSet=0;
	friend void  DV_test();


};

#endif /* DELAYVECTOR_H_ */
void DV_test();
