/*
 * MFParam.cpp
 *
 *  Created on: Feb 18, 2016
 *      Author: sonbol
 */

#include "MFParam.h"

using namespace std;
MFParam::MFParam():commandLine(), prms(numVariate),MFparam(numVariate),MFparam1(numVariate), power(numVariate),pSinMfParam_allVar(numVariate) {
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
	for (int i=0; i<numOutput;i++){
		if(row <= numOfMF[i]){
			numOfMF[i]=row-1;
			cout<<endl<<"number of Membership function is greater than our frequencies, so we have changed it to frequency. The new mfs are"<<endl;
			cout<<numOfMF[i]<<endl;

		}

	}
	int b=1;
	for_each(numOfMF.begin(),numOfMF.end(),[&](int n){b*=n;});
	numRule=b;

	fstream myfile;

	//throw exception if the file cannot be opened
	myfile.exceptions(ifstream::failbit|ifstream::badbit);
	try{
		myfile.open("FinalParams.txt",ios::app|ios::out);
	}
	catch(fstream::failure &e){
		cerr << "Exception opening/reading/closing file\n";
	}
	myfile <<"numofMf:\t\t\t";
	for(auto i:numOfMF){
		myfile <<i<<" ";
	}
	myfile.close();
	for (int i=0 ; i<numVariate;i++){
		prms[i].resize(LengthSurrodata);
	}
	//(row, vector<pair<double,double>>(numVariate));//1 is for amplitude and 1 for phase

	//TODO: check mf with lengthsuroo
	for (int i=0; i< numVariate;i++){ //For each variate an independent fourier transform is taken

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

		MFparam[i].resize(numOfMF[i]); //We multiply by 2 because we want to size indexes as well
		MFparam1[i].resize(numOfMF[i]);
		for (int h=0; h < numOfMF[i];h++){
			MFparam1[i][h].resize(3); //real, image and index
		}
		for (int k=0; k< numOfMF[i];k++){ //index[0] is  for DC power that we ignore it
			//power[k]=ampltd[index[k+1]];

			MFparam[i][k]=(prms[i][index[k+1]]);
			MFparam1[i][k][0]=index[k+1];
			MFparam1[i][k][1]=get<0>(prms[i][index[k+1]]);
			MFparam1[i][k][2]=get<1>(prms[i][index[k+1]]);

		}

		fftw_free(out);
		fftw_destroy_plan(pln);


	}
	fftw_cleanup();
}
vector<vector<double>>* MFParam::getPower(){

	return(&power);
}

std::vector<std::vector<std::vector<double>>> * MFParam::initialMf(){

	for (int i=0; i<numVariate;i++){
		pSinMfParam_allVar[i].resize(numOfMF[i],vector<double>(numParamMF));
			//unique_ptr<vector <vector <double> > > pmfParam_foreach_Variate(new vector< vector <double> > (numOfMF[i], vector<double>(numParamMF)));
			for (int j=0; j< numOfMF[i];j++){

				initiateA();
				initiateB();
				initiateCD();
				(pSinMfParam_allVar)[i][j]={aS,bS,cS,dS};

			}
			//pSinMfParam_allVar[i]=move(pmfParam_foreach_Variate);
		}
	return(&pSinMfParam_allVar);
}
/******************************************************************
initiateA()

Use:				Initialize a in (mf) d*sin(a*teta+b)+c by using a random number generator mt19937 and over uniform distribution
Out:				Random value for a between ...
Status:				Private and called from initialMf()
PreConditions:
Postconditions:
Invariant:
 ******************************************************************/
void MFParam::initiateA(){



	random_device rd;			//seed
	mt19937_64 mt(rd());			//using mt19937 random generator
	//TODO: change it in a meaningful way
	uniform_real_distribution<double> dist(-per10/2,per10/2);		//a between 0-10 by uniform distribution
	aS=dist(mt);
	//	a=5;


}
/*****************************************************************/
/******************************************************************
initiateB()

Use:				Initialize b in (mf) d*sin(a*teta+b)+c by using a random number generator mt19937 and over uniform distribution
Out:				A random value for b between ...
Status:				Private and called from initialMf()
PreConditions:
Postconditions:
Invariant:
 ******************************************************************/
void MFParam::initiateB(){

	/*random_device rd;			//seed
	mt19937_64 mt(rd());			//using mt19937 random generator
	uniform_real_distribution<double> dist(bRange[0],bRange[1]);		//b between 0-100 by uniform distribution
	bS=dist(mt);*/
	//b=90;
	random_device rd;			//seed
	mt19937_64 mt(rd());			//using mt19937 random generator
	uniform_real_distribution<double> dist(0,nextafter (1,DBL_MAX));
	bS=dist(mt)*2*PI;
}
/*****************************************************************/
/******************************************************************
initiateCD()

Use:				Initialize c and d in (mf) d*sin(a*teta+b)+c by using a random number generator mt19937 and over uniform distribution
Out:				Random values between 0-1 (inclusive)
Status:				Private and called from initialMf()
PreConditions:
Postconditions:		0 =< d+c =< 1
					1 >= c >= d> >=0
Invariant:
 ******************************************************************/
void MFParam::initiateCD(){

	random_device rd;			//seed
	mt19937_64 mt(rd());			//using mt19937 random generator
	uniform_real_distribution<double> dist(0,nextafter (1,DBL_MAX));

	/* Initialize c and d to satisfy one of the condition and go to the while loop
	 * because we have multiple calls to initiateCD*/
	cS=0;
	dS=0.1;
	while ((0 > cS+dS  || cS+dS > 1) || (cS < dS)){
		dS=dist(mt);
		cS=dist(mt);

	}


}
/*****************************************************************/
void mfparamtest(){
	InputStream inS;
	MFParam mfPar;
	vector<vector<double>> * surodata;
	vector<vector<vector<double>>> *mfParam;
	inS.getOrigWindowN();
	surodata=inS.getSurWindowN();
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
