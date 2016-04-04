/*
 * Dimension.h
 *
 *  Created on: Jan 29, 2016
 *      Author: sonbol
 */

#ifndef DIMENSION_H_
#define DIMENSION_H_
#include"header.h"
#include "DelayVector.h"
/*#include <flann/flann.hpp>

#include <flann/algorithms/dist.h>
#include <flann/algorithms/kdtree_index.h>
#include <flann/util/params.h>
*/


class Dimension:public DelayVector {
public:
	Dimension();
	std::vector<int> dim;
	std::vector<int>* getDim(std::vector<std::vector<double>> *, std::vector<int>*);
	//DelayVector DV;
	int getNumWeight();
	int getInputVL();
	std::vector<int>* getLengthofVar();
	std::vector<std::vector<double>>* getDV(std::vector<std::vector<double>> *, std::vector<int>*, std::vector<int>*);
		std::vector<std::vector<double>>* getOutput();
	virtual ~Dimension();
private:
	//std::vector<std::vector<double>> * input;
	std::vector<int> * del;
	int KNear=3;
	std::vector<std::vector<int>> vectordim;
	std::vector<std::vector<double>>* delayVec;
	int NNboxwidth=0;
	int NNboxHeight=0;
	int ** AllIndices;
	double errRMSE=0;
	std::vector<double> errorDV;
	int indexofMin=-1;
	int numWeight=0;
	int inputVL=0;
	std::vector<int> lengthofVar;


	void findDimKDD();
	void vectorDim();
	void find3kd();
	void predict1step();
	void findminError();


	void makeDV();
	void resizeDV();
	friend void DimTest(std::vector<std::vector<double>>*,std::vector<int>* );

};
void DimTest(std::vector<std::vector<double>>*,std::vector<int>* );
#endif /* DIMENSION_H_ */
