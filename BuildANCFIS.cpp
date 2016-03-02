/*
 * BuildANCFIS.cpp
 *
 *  Created on: Feb 18, 2016
 *      Author: sonbol
 */

#include "BuildANCFIS.h"
using namespace std;
BuildANCFIS::BuildANCFIS() {
	// TODO Auto-generated constructor stub

}

BuildANCFIS::~BuildANCFIS() {
	// TODO Auto-generated destructor stub
}

void BuildANCFIS::findWeight(){

	if (InS.numpassedInput==0){	//For origin Window

		inputOrigin=InS.getOrigWindowN();
		findDV();
		findMF();
		buildNet.build(delayVectors, mfParam);



	}
	if(InS.numpassedInput >per10){	//for new data coming

		newData=InS.getNewDataN();
	//	DV.addNewDV(newData);
		newDV=DV.getNewDV();
		buildNet.updataWeight(newDV);

	}
}

void BuildANCFIS::findDV(){

//TODO: we do not need getDelayDimension class. Delete it.
	/*DDim.findDDim(inputOrigin);
	delay=DDim.getDelay();
	dimension=DDim.getDimension();*/
	//DV.findDV(inputOrigin,delay,dimension);
	delay=del.getDelay(inputOrigin);
	dimension=dim.getDim(inputOrigin,delay);
	//delayVectors=DV.getDV(inputOrigin,delay,dimension);
}
void BuildANCFIS::findMF(){

	surodata=InS.getSurWindowN();
//	mfPar.findMfparam(surodata);
	mfParam=mfPar.getMf();
}

std::vector<std::vector<double>> * BuildANCFIS::getFinalWeight(){

}
	int BuildANCFIS:: getDimension(){

	}
	int BuildANCFIS::getDelay(){

	}
	std::vector<double>* BuildANCFIS::getMf(){

	}
