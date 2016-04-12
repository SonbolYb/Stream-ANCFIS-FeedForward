/*
 * BuildANCFIS.cpp
 *
 *  Created on: Feb 18, 2016
 *      Author: sonbol
 */

#include "BuildANCFIS.h"
using namespace std;
BuildANCFIS::BuildANCFIS():commandLine(),inputOrigin(NULL),surodata(NULL),delayVectors(NULL),newDV(NULL),mfParam(NULL),dimension(NULL),
		delay(NULL),newData(NULL),finalWeight(NULL){
//	cout<<"BuildAncfis1"<<endl;
}

BuildANCFIS::~BuildANCFIS() {
	// TODO Auto-generated destructor stub
//	cout<<"BuildAncfis2"<<endl;
}

void BuildANCFIS::findWeight(){

	/*It simulate the stream data becuase here we have a file that we want pass data one by one*/
	while(!InS.EndofFile()){
		/*Original window*/
	if (InS.numpassedInput==0){

		//inputOrigin=InS.getOrigWindowN();
		inputOrigin=InS.getOrigWindow();
		findDV();
		findMFParam();
		//finalWeight=buildNet.build(delayVectors, mfParam,dimension,LengthSurodata,LengthDVSet);
		buildNet.build(delayVectors, mfParam,dimension,LengthSurodata,LengthDVSet);


	}
	if(InS.numpassedInput >= per10){	//for new data coming
//TODO: by coming new data points min and max can be different which change normalization and can change the all input
		//newData=InS.getNewDataN();
		//inputOrigin=InS.moveWindowbyOneNormal();
		inputOrigin=InS.moveWindowbyOne();
		/*cout<<"this is input by move"<<endl;
		for(auto i:*inputOrigin){
			for (auto j:i){
				cout <<j <<" ";
			}
			cout<<endl;
		}
		cout<<endl;*/
	//	DV.addNewDV(newData);
		vector<int> * headEndInx=InS.getHeadEndInx();
		/*cout << "this is headEndInx"<<endl;
		for(auto i:*headEndInx){
			cout<<i<<" ";
		}*/
		newDV=DV.getNewDV(inputOrigin,delay,dimension,headEndInx);
		/*cout<<"this is new DV"<<endl;
		for(auto i:*newDV){
			cout<<i <<" ";
		}*/
		//delayVectors=DV.getNewDelayVecs(inputOrigin,delay,dimension,headEndInx);
		/*cout<<endl<<"this is delayVectors2"<<endl;
		for(auto i:*delayVectors){
			for(auto j:i){
				cout<<j<<" ";
			}
			cout<<endl;
		}
		cout<<endl;*/
		//TODO: Ask Scott which one we should use
	//finalWeight=buildNet.build(delayVectors, mfParam,dimension,LengthSurodata,LengthDVSet);
		finalWeight=buildNet.updataWeight(newDV,mfParam,dimension,LengthSurodata,LengthDVSet);

	}
}
buildNet.getStreamParam();

}
/*******************************************************************
findDV:

It has calls to Delay and Dimension class to find delay and dimension.
Then use delay and dimension in DelayVector class to obtain delayvectors
 *******************************************************************/
void BuildANCFIS::findDV(){

	delay=del.getDelay(inputOrigin);
	dimension=dim.getDim(inputOrigin,delay);
	//vector<int> delay1={11};
	//vector<int> dimension1={9};
	//delay=&delay1;
//	dimension=&dimension1;
	//*delay={11};
	//*dimension={9};
	delayVectors=DV.getDV(inputOrigin,delay,dimension);
	LengthDVSet=DV.getLengthDVSet();

	/*cout<<endl<<"this is delayVectors1 in findDV"<<endl;
			for(auto i:*delayVectors){
				for(auto j:i){
					cout<<j<<" ";
				}
				cout<<endl;
			}
			cout<<endl;*/

}
void BuildANCFIS::findMFParam(){

	//surodata=InS.getSurWindowN();
	surodata=InS.getSurWindow();
	//	mfPar.findMfparam(surodata);
	LengthSurodata=((*surodata)[0].size())/2+1;
	//LengthSurodata=((*surodata)[0].size());
	mfParam=mfPar.getMfparam(surodata);
	saveDDMF();
}
void BuildANCFIS::saveDDMF(){
	cout<<endl<<"this is Delay"<<endl;
	for(auto i:*delay){
		cout<<i<<" ";
	}
	cout<<endl<<"this is Dimension"<<endl;
	for(auto i:*dimension){
		cout<<i<<" ";
	}
	cout<<endl<<"this is MF params"<<endl;
	for(auto i:* mfParam){
		for(auto j:i){
			for(auto k:j){
				cout<<k<<" ";
			}

		}
	}
	cout<<endl;
	fstream myfile;

		//throw exception if the file cannot be opened
		myfile.exceptions(ifstream::failbit|ifstream::badbit);
		try{
			myfile.open("FinalParams.txt",ios::app|ios::out);
		}
		catch(fstream::failure &e){
			cerr << "Exception opening/reading/closing file\n";
		}
		myfile<<"----------Results----------"<<endl;
		myfile<<endl<<"Delay:"<<endl;
		for(auto i:*delay){
				myfile<<i<<" ";
			}
		myfile<<endl<<"Dimension"<<endl;
		for(auto i:*dimension){
				myfile<<i<<" ";
			}
		myfile<<endl<<"MfParams"<<endl;
		for(auto i:*mfParam){
				for(auto j:i){
					for(auto k:j){
						myfile<<k<<" ";
					}

				}
				myfile<<endl;
			}
			myfile<<endl;

}
std::vector<std::vector<double>> * BuildANCFIS::getFinalWeight(){
	findWeight();
	return(finalWeight);
}
vector<int>* BuildANCFIS:: getDimension(){
return(dimension);
}
vector<int>*  BuildANCFIS::getDelay(){
	return(delay);

}
vector<vector<vector<double>>>*  BuildANCFIS::getMf(){
	return(mfParam);

}
int BuildANCFIS::getLengthSurodata(){
	return(LengthSurodata);
}
