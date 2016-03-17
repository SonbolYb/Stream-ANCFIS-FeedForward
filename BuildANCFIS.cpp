/*
 * BuildANCFIS.cpp
 *
 *  Created on: Feb 18, 2016
 *      Author: sonbol
 */

#include "BuildANCFIS.h"
using namespace std;
BuildANCFIS::BuildANCFIS():commandLine(){
	// TODO Auto-generated constructor stub
//,inputOrigin(NULL),surodata(NULL),delayVectors(NULL),newDV(NULL),mfParam(NULL),dimension(NULL),delay(NULL),newData(NULL),finalWeight(NULL)
}

BuildANCFIS::~BuildANCFIS() {
	// TODO Auto-generated destructor stub
}

void BuildANCFIS::findWeight(){

	if (InS.numpassedInput==0){	//For origin Window

		inputOrigin=InS.getOrigWindowN();
		findDV();
		findMFParam();
		finalWeight=buildNet.build(delayVectors, mfParam);



	}
	if(InS.numpassedInput >= per10){	//for new data coming

		newData=InS.getNewDataN();
	//	DV.addNewDV(newData);
		newDV=DV.getNewDV(newData,delay,dimension);
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
	delayVectors=DV.getDV(inputOrigin,delay,dimension);

}
void BuildANCFIS::findMFParam(){

	surodata=InS.getSurWindowN();
	//	mfPar.findMfparam(surodata);
	mfParam=mfPar.getMfparam(surodata);
}

std::vector<std::vector<double>> * BuildANCFIS::getFinalWeight(){
	findWeight();
	return(finalWeight);
}
int BuildANCFIS:: getDimension(){

}
int BuildANCFIS::getDelay(){

}
std::vector<double>* BuildANCFIS::getMf(){

}
