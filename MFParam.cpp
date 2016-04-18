/*
 * MFParam.cpp
 *
 *  Created on: Feb 18, 2016
 *      Author: sonbol
 */

#include "MFParam.h"

using namespace std;
MFParam::MFParam():commandLine(), prms(numVariate),MFparam(numVariate),MFparam1(numVariate), power(numVariate),powerCheck(numVariate) {
	// TODO Auto-generated constructor stub
	//cout<<"mfParam1"<<endl;
}

MFParam::~MFParam() {
	// TODO Auto-generated destructor stub
	//cout<<"mfParam2"<<endl;
}


/*  It is in a case of univariate
 * 1. Pass the surrogate date (endtoend certified) to fourier transfrom
 * 2. select frequencies with highest power. Num of frequencies is euqal to MF
 * 3. MF is predetermined in this version, so based on num of MF, we select the sine wave with the highest power and used their phase and amplitude
 * 3.
 * In a case of multivariate, we should note that for each variable, we need different set of membership functions
 * 1. We need to have different inputs for each variate or at least they should be selected
 * 2. We pass each variate to fast fourier transform and select the sine with highets power
 * 3. use their amplitude, phase, and frequency as our MF parameters.*/
std::vector<vector<vector<double>>> * MFParam::getMfparam(std::vector<std::vector<double>> * input){

	calMFparam(input);
	return(&MFparam1);

}

void MFParam::calMFparam(std::vector<std::vector<double>> * input){
	int row=((*input)[0].size())/2+1;
	//TODO: this variable is defined in commandline but I should change it to be added in MFparam because it is kind of global here
	int	LengthSurrodata=row;


	vector<int> numOfMFCheck (numOutput);






	/*for (int i=0 ; i<numVariate;i++){
		prms[i].resize(LengthSurrodata);
	}*/
	//(row, vector<pair<double,double>>(numVariate));//1 is for amplitude and 1 for phase

	//TODO: check mf with lengthsuroo
	for (int i=0; i< numVariate;i++){ //For each variate an independent fourier transform is taken

		if(row <= numOfMF[i]){
			numOfMFCheck[i]=LengthSurrodata-1;

		}
		else{
			numOfMFCheck[i]=numOfMF[i];
		}
		prms[i].resize(LengthSurrodata);

		fftw_plan pln;
		int N=(*input)[i].size();
		int Nout=floor(N/2+1);

		double * inpt=&((*input)[i][0]);
		fftw_complex  *out=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*Nout);
		//vector<double> power (numOfMF[i]);
		vector<double> ampltd (Nout);
		power[i].resize(Nout);
		//Getting Fourier Transform
		pln=fftw_plan_dft_r2c_1d(N,inpt,out,FFTW_ESTIMATE);
		fftw_execute(pln);
		//Get power for each data point
		for (int j=0;j< Nout;j++){

			double real=out[j][0];
			double img=out[j][1];
			double amplitude=sqrt(pow(real,2)+pow(img,2));
			ampltd[j]=amplitude;
			power[i][j]=amplitude;
			//	cout<<"real"<<real<<" "<<img<<" ";

			//double phase=atan2(img,real);
			prms[i][j]=make_pair(real,img);

		}

		//Max of powers
		vector<int> index (Nout);

		//save index for use in sort function
		for (int j=0; j<Nout; j++){
			index[j]=j;

		}
		//using lambda to get the index of max power
		sort(index.begin(),index.end(),[&ampltd](size_t ii, size_t jj){return (ampltd[ii]>ampltd[jj]);});

		MFparam[i].resize(numOfMFCheck[i]); //We multiply by 2 because we want to size indexes as well
		MFparam1[i].resize(numOfMFCheck[i]);
		powerCheck[i].resize(numOfMFCheck[i]); //powerCheck saves the greates power
		for (int h=0; h < numOfMFCheck[i];h++){
			MFparam1[i][h].resize(4); //real, image and index, DC
		}
		//	cout<<"mf"<<numOfMF[i]<<" ";

		for (int k=0; k< numOfMFCheck[i];k++){ //index[0] is  for DC power that we ignore it
			//power[k]=ampltd[index[k+1]];
			//cout<<"real"<<get<0>(prms[i][index[k+1]])<<" "<<get<1>(prms[i][index[k+1]]);;
			MFparam[i][k]=(prms[i][index[k+1]]);
			powerCheck[i][k]=power[i][index[k+1]];
			MFparam1[i][k][0]=index[k+1];
			MFparam1[i][k][1]=get<0>(prms[i][index[k+1]]);
			MFparam1[i][k][2]=get<1>(prms[i][index[k+1]]);
			MFparam1[i][k][3]=sqrt(power[i][0]);

		}

		fftw_free(out);
		fftw_destroy_plan(pln);


	}
	fftw_cleanup();
}
vector<vector<double>>* MFParam::getPower(){

	return(&powerCheck);
}
void mfparamtest(){
	InputStream inS;
	MFParam mfPar;
	vector<vector<double>> * surodata;
	vector<vector<vector<double>>> *mfParam;

	surodata=inS.getSurWindow(inS.getOrigWindowN());
	cout<< "this is surrogate data"<<endl;
	for (auto i: *surodata){
		for (auto j:i){
			cout << j<< " ";
		}
	}

	mfParam=mfPar.getMfparam(surodata);
	cout<<endl<<"this is power"<<endl;
	vector<vector<double>> *pw=mfPar.getPower();
	for (auto i: *(pw)){
		for (auto j: i){
			cout << j<< " ";
		}
	}
	cout <<endl<<"this is mf params coming out of MFParam" <<endl;
	for (auto i:*mfParam){
		for (auto j: i){
			for (auto k: j){
				cout << k << " ";
			}
		}
	}
}
