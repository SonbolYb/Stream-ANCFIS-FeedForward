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
	//cout<<"BuildNet1"<<endl;
}

BuildNet::~BuildNet() {
	// TODO Auto-generated destructor stub
	//cout<<"BuildNet2"<<endl;
}
//TODO: It could be my code from ANCFIS-ELM
vector<vector<double>>* BuildNet:: build(std::vector<std::vector<double>> * delayvector, std::vector<vector<vector<double>>>* MFparam,vector<int>* dim,int lengthSurodata,int LengthDVSet){

	//For a number of epoch, we get the output weight with this delay vector
	//Then we have the final weight.
	int sizeDV=delayvector->size();
	int epochPass=0;

	int inputVL=0;
		for(auto i:*dim){
			inputVL+=i;
		}
		inputVectorWeight.numWeight=numRule+numRule*inputVL;
		inputVectorWeight.resetVectors();


	for (int epoch=0; epoch <numEpoch;epoch++ ){


		if (epoch!=0){

			shuffleDV(delayvector);
		}

		for (int indexInV=0;indexInV<sizeDV;indexInV++){
			vector<double> & inputVec=(*delayvector)[indexInV];
			//cout<<endl<<"indexInv"<<indexInV<<"  epoch"<<epoch<<endl;
			/*cout<<"this is inputVec"<<endl;
			for(auto i:inputVec){
				cout<<i<<" ";
			}
			cout<<endl;*/
			inputVectorWeight.calculateWeight(inputVec,indexInV,MFparam,dim,lengthSurodata,LengthDVSet);	//indexinV goes as iteration for membership function
			inputVectorWeight.calErrorTr(epoch);

		}

		inputVectorWeight.calError(epoch);							//Calculate the error at the end of epoch
		//	calError_test(inputVectorWeight,epoch);
		//checkCondition();
	//	cout<<inputVectorWeight.getAveBest() ;
		if ((epoch==(numEpoch-1)) || (inputVectorWeight.getAveBest() <= threshold)){
			epochPass=epoch;
			break;
		}

	}
	inputVectorWeight.saveParams(epochPass);
	//inputVectorWeight.resetValueforUpdateWeight();

	return(inputVectorWeight.getWBest());
}
vector<vector<double>>* BuildNet::updataWeight(std::vector<double>* NewDelayVector,std::vector<vector<vector<double>>>* MFparam, vector<int>*dim, int lengthSurodata, int lengthDVSet){
	vector<double> & NewDV=(*NewDelayVector);
	//cout<<"h1";
	inputVectorWeight.resetValueforUpdateWeight();
	inputVectorWeight.calculateWeight(NewDV,0,MFparam,dim,lengthSurodata,lengthDVSet);
	//inputVectorWeight.calErrorTr(0);
	inputVectorWeight.calErrorStream();
	//inputVectorWeight.saveParams(0);
	return(inputVectorWeight.getWBest());

}
void BuildNet::getStreamParam(){
	inputVectorWeight.saveStreamParams();
}
void BuildNet::shuffleDV(std::vector<std::vector<double>> * delV){
	shuffle(delV->begin(),delV->end(), default_random_engine(seed));
}
