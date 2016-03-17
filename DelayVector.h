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
#include "DelayVector.h"
#include <flann/flann.hpp>
#include <flann/algorithms/dist.h>
#include <flann/algorithms/kdtree_index.h>
#include <flann/util/params.h>


class DelayVector:public commandLine {
public:
	DelayVector();
	virtual ~DelayVector();
	//void getDelayVector();
	std::vector<double>* getNewDV(std::vector<double>*, std::vector<int>*, std::vector<int>*);

	int getsize();

	//InputStream inS;
	//void getDV(Delay , Dimension );
	//void findDV(std::vector<std::vector<double>> *, int, int);
	std::vector<std::vector<double>>* getDV(std::vector<std::vector<double>> *, std::vector<int>*, std::vector<int>*);
	std::vector<std::vector<double>>* getDVforDim(std::vector<std::vector<double>> *, std::vector<int>*, std::vector<int>*);
	std::vector<std::vector<double>>* getOutputforDim();
	bool compare(double,double);
private:
	//void delay();
	//void dimension();

	void deletOldDV();
	//void getInput();
	void findDur();
	void findDurforDim();
	void makeDV();
	void makeDVforDim();
	void resizeDV();
	void resizeDVforDim();
	void addNewDV(std::vector<double>*);

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
	friend void  DV_test();


};

#endif /* DELAYVECTOR_H_ */
void DV_test();
