/*
 * BuildNet.cpp
 *
 *  Created on: Feb 18, 2016
 *      Author: sonbol
 */

#include "BuildNet.h"
using namespace std;
BuildNet::BuildNet():commandLine() {
	// TODO Auto-generated constructor stub
	seed = std::chrono::system_clock::now().time_since_epoch().count();
}

BuildNet::~BuildNet() {
	// TODO Auto-generated destructor stub
}
//TODO: It could be my code from ANCFIS-ELM
vector<vector<double>>* BuildNet:: build(std::vector<std::vector<double>> * delayvector, std::vector<vector<vector<double>>>* MFparam){
	//For a number of epoch, we get the output weight with this delay vector
	//Then we have the final weight.
	//TODO: Check if the size of delay vector is correct
	int sizeDV=delayvector->size();
	cout<<" size DV= "<< sizeDV<<endl;
	int epochPass=0;
	for (int epoch=0; epoch <numEpoch;epoch++ ){
		//TODO: shuffle
		if (epoch!=0){
			//TODO
			shuffleDV(delayvector);
		}

		for (int indexInV=0;indexInV<sizeDV;indexInV++){
			vector<double> & inputVec=(*delayvector)[indexInV];
			inputVectorWeight.calculateWeight(inputVec,indexInV,MFparam);	//indexinV goes as iteration for membership function
			inputVectorWeight.calErrorTr(epoch);

		}
		inputVectorWeight.calError(epoch);							//Calculate the error at the end of epoch
		//	calError_test(inputVectorWeight,epoch);
		//checkCondition();
		if ((epoch==(numEpoch-1)) || (inputVectorWeight.getAveBest() <= threshold)){
			epochPass=epoch;
			break;
		}

	}
	inputVectorWeight.saveParams(epochPass);


	return(inputVectorWeight.getWBest());
}
void BuildNet::updataWeight(std::vector<double>*){

}
void BuildNet::shuffleDV(std::vector<std::vector<double>> * delV){
	shuffle(delV->begin(),delV->end(), default_random_engine(seed));
}
