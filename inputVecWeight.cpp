/*
 * inputVecWeight.cpp
 *
 *  Created on: Jun 17, 2015
 *      Author: sonbol
 */

#include "inputVecWeight.h"
using namespace std;

/*******************************************************************
inputVecWeight():

input:			 the pointer to inputs of an input vector
target:			 the pointer to targets of an input vector
pH: 			 a unique pointer to firing strength of rules
rpH:			 reference to pH
pM_old:			 a vector of unique pointers to P(n-1) (M) in RLS for all the outputs
pM_new:			 a vector of unique pointers to P(n) (M) in RLS for all the outputs
G:				 A vector of g(n) in RLS for all the outputs
alpha: 			 a vector of a(n) in RLS for all the outputs
pW_old:			 a vector of unique pointers to W(n-1)in RLS for all the outputs
pW_new:			 a vector of unique pointers to W(n)in RLS for all the outputs
errorEpoch:		 A vector to save error of all the epochs
bestError:		 A vector of the best error for all the outputs
aveBest:		 The best error average
bestW:			 A vector of weights of the best error.


Note:

	RLS (Recursive Least Square) is based on p.248, chapter 5 of Haykin book.

pM_old and pW_old are initialized to their initial values.
	P(0)=(lamba)^-1*I ===> It is done by the identityM method.
	W(0)=0

 *******************************************************************/
inputVecWeight::inputVecWeight():commandLine(),rpH(pH),pM_new(numOutput),pM_old(numOutput),G(numOutput,vector<double>(1)),
		alpha(numOutput,0), pW_old(numOutput),pW_new(numOutput),
		aveBest(initError),epochBest(0),vectorTrainBestEpoch(NULL),bestW(numOutput, vector<double>(1)),bestPm(numOutput,vector<vector<double>> (1)),bestFS(numRule),ErrorStream(1)
		/*trainingErrorAll(numEpoch),vector<vector<double>> (LengthDVSet,vector<double>(numOutput)))*/

{
	//errorEpoch(numEpoch,vector<double>(numOutput)),aveErrorTrn(numEpoch),

//	cout<<"inputVecWeight1"<<endl;
}
inputVecWeight::~inputVecWeight(){
//	cout<<"inputVecWeight2"<<endl;
}
/*******************************************************************/
/*******************************************************************
identityM()

Use:			Initialize P(0) to P(0)=(lamba)^-1*I
Out:			P(0)
Status:			Private called from constructor to initialize P (P(0))
PreConditions:	The size of the vector (P(0)) is numofOutput
Postconditions:	We have an identity matrix as initial of P(0)
Invariant:
 *******************************************************************/
void inputVecWeight::identityM(){

	unique_ptr<vector<vector<double>>> mat;

	for(int i=0; i< numOutput;i++){
		//		unique_ptr<vector<vector<double>>> mat(new vector<vector<double>> (numRule, vector<double> (numRule)));


		mat.reset(new vector<vector<double>> (numWeight, vector<double> (numWeight)));

		for(int j=0; j<numWeight;j++){
			for(int k=0; k<numWeight;k++){
				if (j==k){
					(*mat)[j][k]=(1/lambdaRLS);
				}
				else{
					(*mat)[j][k]=0;
				}
			}
		}
		pM_old[i].reset();
		pM_old[i]=move(mat);

	//	mat.reset(nullptr);
	//	pM_new[i]=unique_ptr<vector<vector<double>>> (new vector<vector<double>> (numWeight, vector<double> (numWeight,1)));

	}

}
/*******************************************************************/
void inputVecWeight::resetValueforUpdateWeight(){
	epochBest=0;
	aveBest=initError;
	bestErrorLastdata=initError;
	aveBest=initError;
	bestaveFinaAlter=initError;
	for (int nV=0;nV<numOutput;nV++){
		//TODO: important: we put the best w of the previous window as old weight of the current window

		*(pW_old[nV])=bestW[nV];
		*(pM_old[nV])=bestPm[nV];
	}
}
/*******************************************************************/
/*******************************************************************
calculateWeight():

It is the main function.
We define a new instance of firing strength member here. So, by each call to this function, we have a new firingstrenght object.
It makes sense as each calling to the function is equivalent to reading a new input vector and assigning a new fs to it.
pH is pointer to the firing strenght and the we call RLS algorithm.

Use:	It reads an input vector to get input and target of it and then for the input vector, obtains firing strength of rules.
		Then applies RLS to update P(n) and W(n) and replace W(n-1) and P(n-1) with W(n) and P(n) in order to use the values obtained
		for the current input vector as initial (previous) values for the next coming input vector.

Out:	It calculates W(n) and P(n) for each input vector based on RLS algorithm.

Status:	public and Called in ANCFISNet

PreConditions: An object of inputVector class has been instantiated in the constructor of this class


Postconditions:	1.An object of firing strength is instantiate in the function for each coming input vector.
				2. W(n) and P(n) is calculated
				3. W(n-1) and P(n-1) are replaced with W(n) and P(n) to be used for the next input vector.

Invariant:
 *******************************************************************/

void inputVecWeight::calculateWeight(const vector<double> & inputVec, int iter,vector<vector<vector<double>>> *MFparams,vector<int>* dim,int lengthSurodata,int lengthDVSet){
	int inputVL=0;
	for(auto i:*dim){
		inputVL+=i;
	}
	/*cout<<endl<<"this is pW_old"<<endl;
	for(auto &i:pW_old){
		for(auto j:*i){
			cout<<j<<" ";
		}
	}
	cout<<endl;*/
//	numWeight=numRule+numRule*inputVL;
	LengthDVSet=lengthDVSet;
	//cout<<endl<<"inputVL from calculateWeight= "<<inputVL<<endl;
	//TODO: It is wrong because we have to reset the input vectors all the time
	//resetVectors();
	itera=iter;
	inputV.readData(inputVec,dim);

	target.reset();
	target=move(inputV.readTarget());
	std::unique_ptr<std::vector<double>> input=move(inputV.readInput());

	/*cout<<endl<<"this is input "<<endl;
	for(auto i:*input){
		cout<<i<<" ";
	}
	cout<<endl<<"this is target"<<endl;
	for(auto i:*target){
		cout<<i<<" ";
	}
	cout<<endl;*/
	//vector<double> &rinput=*(input);
	firingStrength fS;
	//pH=fS.cal_firingStrenght(rinput);
	pH.reset();
	pH=fS.cal_firingStrenght(input,iter,MFparams,dim,lengthSurodata);
	/*cout<<"fs in CalculateWieght= "<<endl;
	for(auto i:*pH){
		cout<<i<<" ";
	}
	cout<<endl;*/
	//FS_test(fS);

	RLS();
	//for(int nV=0; nV<numVariate;nV++){
	/*for(auto i:*(pM_old[nV])){
				for(auto j:i)
					cout<<j<<" ";
				cout<<endl;
			}
	}*/
	Replace_Old_New();
//	cout<<endl<<"really why"<<endl;
	/*for(auto i:*(pM_old[nV])){
					for(auto j:i)
						cout<<j<<" ";
					cout<<endl;
				}*/
		//}

}
/*******************************************************************/
void inputVecWeight::resetVectors(){


	for(int i=0;i<numOutput;i++){

		G[i].resize(numWeight);
		bestW[i].resize(numWeight);
		bestPm[i].resize(numWeight);
		pW_old[i]=unique_ptr <vector<double>> (new vector<double> (numWeight, 0));
		pW_new[i]=unique_ptr <vector<double>> (new vector<double> (numWeight, 0));
		pM_new[i]=unique_ptr<vector<vector<double>>> (new vector<vector<double>> (numWeight, vector<double> (numWeight,1)));

	}
	for(int j=0;j<numOutput;j++){

	for(int i=0; i<numWeight;i++){
		bestPm[j][i].resize(numWeight);
	}

	}
	identityM();
	//TODO: lots of loop here.
	/*trainingErrorAll.resize(numEpoch);
	for(int i=0; i<numEpoch;i++){
		trainingErrorAll[i].resize(LengthDVSet);
	}
	for(int i=0;i<numEpoch;i++){
		for(int j=0;j<LengthDVSet;j++){
			trainingErrorAll[i][j].resize(numOutput);
		}
	}*/


}
/*******************************************************************
RLS()

Use:	calculate RLS algorithm in P.248 in Haykin book

Out:	Calculation of:
						P(n), g(n), a(n) and W(n)

Status:	private and called in calculateWeight method

PreConditions:
Postconditions:
Invariant:
 *******************************************************************/
void inputVecWeight::RLS(){	//TODO: change this one

	CalPn();
	CalGn();
	CalAlpha();
	CalW();

}
/*******************************************************************/
void inputVecWeight::calErrorStream(){

	double aveErr=0;
	vector<double> errorEpoch1 (numOutput,initError);
	for (int nV=0;nV<numOutput;nV++){
		double v=0;
		vector<double> &rb=(*pW_old[nV]);
		v=((*target)[nV]-Cal_VVS(rb,rpH));
		//	errorEpoch[ep][nV]=sqrt(0.5*pow(v,2)+0.5*lambdaRLS*norm);
		errorEpoch1[nV]=abs(v);
		aveErr+=abs(v);
	}
	ErrorStream.push_back((aveErr/numOutput));
//	cout<<endl<<"this is error stream= "<<ErrorStream<<endl;
	for (int nV=0;nV<numOutput;nV++){

		bestW[nV]=(*pW_old[nV]);
		bestPm[nV]=(*pM_old[nV]);


		/*//We cannot have rbestW because Pw_old going out of scope by coming new input vector so the pointers pointing to somewhere unknown so rbestW.
						rbestW[nV]=(pW_old[nV]).get();*/
	}

}
/*******************************************************************/

/*******************************************************************
Replace_Old_New()

Use: Replace W(n-1) and P(n-1) with W(n) and P(n) to be used for the next input vector.
Out:
Status:
PreConditions:	unique_ptr to P(n) is valid
				unique_ptr to W(n) is valid

Postconditions: unique_ptr to P(n-1) is valid
				unique_ptr to W(n-1) is valid
				unique_ptr to P(n) is deleted
				unique_ptr to W(n) is deleted
Invariant:
 *******************************************************************/
void inputVecWeight::Replace_Old_New(){




	/*for(auto i:pM_new){
		for(auto j:*i){
			cout<< j<< " ";
		}
	}*/

	for (int nV=0; nV<numOutput; nV++){
		/*cout<<endl<< "pmnew"<<endl;

		for(auto i:*(pM_new[nV])){
			for(auto j:i){
				cout<<j<<" ";
			}
		}
		cout<< endl<<"pmold"<<endl;

		for(auto i:*(pM_old[nV])){
			for(auto j:i){
				cout<<j<<" ";
			}
		}

		cout<< endl<<"pwnew"<<endl;
		for(auto i:*(pW_new[nV])){
			cout<<i<<" ";
		}
		cout<<endl<< "pwold"<<endl;
		for(auto i:*(pW_old[nV])){
			cout<<i<<" ";
		}*/
		/*pW_old[nV]=move(pW_new[nV]);
		pM_old[nV]=move(pM_new[nV]);*/
		/**(pW_old[nV])=*(pW_new[nV]);
		*(pM_old[nV])=*(pM_new[nV]);*/


		/*cout<<endl<< "pwold"<<endl;
		for(auto i:*(pW_old[nV])){
			cout<<i<<" ";
		}*/

		//pW_old[nV].reset(new vector<double> (numWeight, 1));
		//identityM();
		pW_old[nV].reset();
		pM_old[nV].reset();

		pW_old[nV]=move(pW_new[nV]);
		pM_old[nV]=move(pM_new[nV]);
		//pW_old[nV].reset((pW_new[nV]).release());
		//pM_old[nV].reset((pM_new[nV]).release());

		pW_new[nV].reset();
		pM_new[nV].reset();
		//pW_new[nV].reset(new vector<double> (numWeight, 1));
		//pM_new[nV].reset(new vector<vector<double>> (numWeight, vector<double> (numWeight,1)));
	//	cout<<endl<<" pmoldSize  "<<(pM_old[nV])->size()<<" pmnew  "<< pM_new[nV]->size()<<endl;

		//pM_old[nV].reset((pM_new[nV]).release());
	/*	cout<< endl<<"pmold"<<endl;
		for(auto i:*(pM_old[nV])){
			for(auto j:i){
				cout<<j<<" ";
			}
		}*/
		/*pW_new[nV].reset(nullptr);
			pM_new[nV].reset(nullptr);*/
	}

}

/*******************************************************************
calError()
Use:				Calculate error of the training set in each epoch and obtain the average of error by (e.g. for 3 variates:
						E=(e1)^2+(e2)^2+(e3)^2 	==> 	E_ave=sqrt(E/numVariate(=3))
					Each time save weight of the best epoch in terms of error. The best weight is used as weights of system for checking set.
Out:				Weights are used in building system for checking set.
Status:				Public and called in AncfisNet after each epoch
PreConditions:		pW_old, rpH, target are valid and have values of the last inputvector in the training set
Postconditions:		bestError, aveBest and bestW may change if the condition is satisfied (aveErr <= aveBest)
Invariant:			numOutput, pW_old, rpH, target
 *******************************************************************/
void inputVecWeight::calError(int ep){

	double aveErr=0;
	vector<double> errorEpoch1 (numOutput,initError);
	for (int nV=0;nV<numOutput;nV++){
		double v=0;
		vector<double> &rb=(*pW_old[nV]);
		v=((*target)[nV]-Cal_VVS(rb,rpH));
		//	errorEpoch[ep][nV]=sqrt(0.5*pow(v,2)+0.5*lambdaRLS*norm);
		errorEpoch1[nV]=abs(v);
		aveErr+=abs(v);
	}
	errorEpochLastdata.push_back(errorEpoch1);

	aveErr=(aveErr/numOutput);
	aveErrorLastdata.push_back(aveErr);
	aveFinalalter=aveFinalalter/LengthDVSet;
	aveFinalMain=(aveFinalMain/(LengthDVSet*numVariate));
	aveErrorEpoch.push_back(aveFinalMain);

	//aveBest=aveFinalMain;

	if(aveFinalMain <= aveBest){
		epochBest=ep;
		bestErrorLastdata=aveErr;
		aveBest=aveFinalMain;
		bestaveFinaAlter=aveFinalalter;
		bestFS=*(pH);

		//vectorTrainBestEpoch=&(trainingErrorAll[epochBest]); //TODO: It should be a pointer

		for (int nV=0;nV<numOutput;nV++){

			bestW[nV]=(*pW_old[nV]);
			bestPm[nV]=(*pM_old[nV]);
			/*//We cannot have rbestW because Pw_old going out of scope by coming new input vector so the pointers pointing to somewhere unknown so rbestW.
				rbestW[nV]=(pW_old[nV]).get();*/
		}
	}
}
/*******************************************************************/
void inputVecWeight::calErrorTr(int ep){
	fstream myfile;
	myfile.open("TrainError.txt",ios::app|ios::out);
	double aveErr=0;
	for (int nV=0;nV<numOutput;nV++){
		double v=0;
		vector<double> &rb=(*pW_old[nV]);
		v=((*target)[nV]-Cal_VVS(rb,rpH));
		aveErr+=abs(v);
		//TODO: it was making problem. Please fix it.
		//trainingErrorAll[ep][itera][nV]=v;
		myfile<< (*target)[nV]<<"\t"<<Cal_VVS(rb,rpH)<<"\t"<<v<<endl;
	}



	aveFinalalter+=(aveErr/numOutput);
	aveFinalMain+=aveErr;
	/*cout<<"inputV"<<" ";
	for (auto i:*target){
		cout<<i<<" ";
	}
	cout<<"iteration= "<<itera;
	cout<<"fs in CalculateWieght= "<<endl;
		for(auto i:*pH){
			cout<<i<<" ";
		}
		cout<<endl;*/
}

void inputVecWeight::saveStreamParams(){
	cout<<endl<<"This is the last error= "<<ErrorStream.back()<<endl;
	/*cout<<endl<<"this is the best weight"<<endl;
	for (auto i:(bestW)){
			for (auto j:i){
				cout << j<< " ";

			}
			cout<<endl;
		}*/
	fstream myfile;
	myfile.exceptions(ifstream::failbit|ifstream::badbit);
		try{
			myfile.open("FinalParams.txt",ios::app|ios::out);
		}
		catch(fstream::failure &e){
			cerr << "Exception opening/reading/closing file\n";
		}
		myfile<<endl<<"WBest_Trn_afterStreaming:"<<endl;

					for(auto i:bestW ){
						for(auto j:i)
							myfile<<j<<" ";
						myfile<<endl;
					}
		myfile<<endl<<"FinalErrorStream_Trn:\t\t"<<ErrorStream.back();
				myfile.close();
		fstream myfile3;
			myfile3.exceptions(ifstream::failbit|ifstream::badbit);
			try{
				myfile3.open("FinalError.txt",ios::app|ios::out);
			}
			catch(fstream::failure &e){
				cerr << "Exception opening/reading/closing file\n";
			}
			myfile3<<endl<<"FinalErrorStream_Trn:\t"<<ErrorStream.back();
			myfile3.close();
			fstream myfile4;
				try{
					myfile4.open("ChkError.txt",ios::app|ios::out);
				}
				catch(fstream::failure &e){
					cerr << "Exception opening/reading/closing file\n";
				}
				myfile4<<endl<<"FinalErrorStream_Trn:\t"<<ErrorStream.back();
}

/*******************************************************************
saveParams()
Use:	It saves the params of the best weights obtained through epochs. The weights are used in checking set.
Out:	A file called FinalParamsUsedForChecking.txt is created and filled.
Status:				Public and called in AncfisNet after all the epochs
PreConditions:		FinalParamsUsedForChecking.txt is created or valid
Postconditions:		FinalParamsUsedForChecking.txt has filled by the params of the set of parameter exploration.
Invariant:
 *******************************************************************/
void inputVecWeight::saveParams(int epochPass){
	cout<<endl<<"this the best epoch= "<<epochBest<<endl;
	//	cout<<endl<<"this is the error vector best"<<endl;
	//	for(auto i:(bestError)){
	//		cout<<i<<" ";
	//	}
	cout<<"this is bestError Trn= "<<aveBest<<endl;
	cout<<"this is the best final ater Trn= "<<bestaveFinaAlter<<endl;
	//cout<<"error of all the training"<<endl;
	/*for (auto i:*vectorTrainBestEpoch){
		for (auto j:i){
			cout << j<<" ";
		}
	}*/
	cout <<endl<<"this is error of last data Trn="<<bestErrorLastdata<<endl;

		/*cout<<endl<<"this is W of best"<<endl;

		for(auto i:bestW ){
			for(auto j:i)
				cout<<j<<" ";
			cout<<endl;
		}
		cout<<endl;
*/

	fstream myfile;

	//throw exception if the file cannot be opened
	myfile.exceptions(ifstream::failbit|ifstream::badbit);
	try{
		myfile.open("FinalParams.txt",ios::app|ios::out);
	}
	catch(fstream::failure &e){
		cerr << "Exception opening/reading/closing file\n";
	}
	//myfile<<"----------Results----------"<<endl;
	myfile<<"numWeight:\t\t"<<numWeight;
	myfile<<endl<<"WBest_Trn_beforStreaming:"<<endl;

	for(auto i:bestW ){
		for(auto j:i)
			myfile<<j<<" ";
		myfile<<endl;
	}
	myfile<<"pM best before streaming:"<<endl;
	for(auto i:bestPm){
		for(auto j:i){
			for(auto k:j){
				myfile<<k<<" ";
			}
		}
	}
	myfile<<endl<<"epochPassed:\t\t\t"<<epochPass;
	myfile<<endl<<"EpochBest_Trn:\t\t\t"<<epochBest;
	/*myfile<<endl<<"ErrorVectorBest_Trn:\t\t";
	for(auto i:(bestError)){
		myfile<<i<<" ";
	}*/
	myfile<<endl<<"AveErrorLastdata:\t\t"<<bestErrorLastdata;
	myfile<<endl<<"AveErrorBestAlter_Trn:\t\t"<<bestaveFinaAlter;
	myfile<<endl<<"AveErrorBest_Trn:\t\t"<<aveBest;
	myfile<<endl<<"firingStrength:\t\t"<<endl;
			for(auto i:bestFS){
				myfile<<i<<" ";
			}
			myfile<<endl;



	fstream myfile3;
	myfile3.exceptions(ifstream::failbit|ifstream::badbit);
	try{
		myfile3.open("FinalError.txt",ios::app|ios::out);
	}
	catch(fstream::failure &e){
		cerr << "Exception opening/reading/closing file\n";
	}
	myfile3<<"******************************************************"<<endl;
	myfile3<<endl<<"EpochBest_Trn:\t\t"<<epochBest;
	myfile3<<endl<<"AveErrorBest_Trn:\t"<<aveBest;
	myfile3.close();

	fstream myfile2;
	//throw exception if the file cannot be opened
	myfile2.exceptions(ifstream::failbit|ifstream::badbit);
	try{
		myfile2.open("TrainEpochError.txt",ios::app|ios::out);
	}
	catch(fstream::failure &e){
		cerr << "Exception opening/reading/closing file\n";
	}
	myfile2<<"******************************************************"<<endl;
	myfile2<<endl<<"TrainError:"<<endl;
	for(auto i: aveErrorEpoch){
		myfile2<<i<<" ";
	}
	myfile2<<endl;
	/*myfile2<<endl<<"TrainErrorVec"<<endl;
	for(auto i:errorEpoch){
		for(auto j:i){
			myfile2<<j<<" ";
		}
		myfile2<<endl;
	}*/

	myfile2<<"******************************************************"<<endl;
	myfile2.close();

	fstream myfile5;
	try{
		myfile5.open("TrainingErrorofBestEpoch.txt",ios::app|ios::out);
	}
	catch (fstream::failure &e){
		cerr << "Exception opening/reading/closing file\n";
	}
	myfile5 <<"******************************************************"<<endl;
	/*myfile5<<endl<<"trainingErrorofBestEpoch"<<endl;
	for (auto i:*vectorTrainBestEpoch){
		for (auto j:i){
			myfile5 << j<<" ";
		}
		myfile5<<endl;
	}*/
	myfile5.close();
	fstream myfile4;
	try{
		myfile4.open("ChkError.txt",ios::app|ios::out);
	}
	catch(fstream::failure &e){
		cerr << "Exception opening/reading/closing file\n";
	}
	myfile4 <<"******************************************************"<<endl;
	myfile4<<endl<<"EpochBest_Trn:\t\t"<<epochBest;
	myfile4<<endl<<"AveErrorBest_Trn:\t"<<aveBest;
	myfile4.close();
}
/*******************************************************************/
/*******************************************************************
getAveBest()
Use:	It returns the best average of error to Ancfisnet class to be compared by threshold
Status:	Public and called in AncfisNet
 *******************************************************************/
double inputVecWeight::getAveBest(){
	return(aveBest);
}
/*******************************************************************/
/*******************************************************************
getWBest()
Use:	It returns a pointer to the weights corresponding to the best average error
		and this weights are used in the network and checking set.
Status:	Public and called in AncfisNet
 *******************************************************************/
vector<vector<double>> * inputVecWeight::getWBest(){
	vector<vector<double>> * pWbest=&bestW;
	return(pWbest);
}
/*******************************************************************/

/*******************************************************************
CalPn()

Use:			Calculate P(n) in p.248, ch.5, Hykin book by using 6 methods:
					Cal_MV, Cal_VM, Cal_VVM, Cal_VVS, Cal_MtoS, Cal_Msub
Out:			Obtain P(n)
Status:			Private and called from RLS()
PreConditions:	pM_old and rpH are valid pointers
Postconditions:	pM_new is filled
Invariant:

p(n)=p(n-1)-num/denum
num=p(n-1)*H(n)*Ht(n)*p(n-1)
denum=1+Ht(n)*p(n-1)*H(n)

Cal_MV:	p(n-1)*H(n)
Cal_VM:	Ht(n)*p(n-1)
Cal_VVM:	num
Cal_VVS:	Ht(n)*Cal_VM=Ht(n)*p(n-1)*H(n)
Cal_MtoS:	num/denum
Cal_Msub:	p(n)

As this one used for online learning, we use a forget coefficient in RLS equation:
p(n)=1/forget*(p(n-1)-num/(denum+forget))

 *******************************************************************/
void inputVecWeight::CalPn(){


/*cout<<endl<<"pmNew"<<endl;*/
	for(int nV=0; nV<numOutput;nV++){


		vector<double> a(numWeight,0);
		vector<double> b(numWeight,0);
		vector<vector<double>> c(numWeight,vector<double> (numWeight,0));
		vector<vector<double>> e(numWeight,vector<double> (numWeight,0));

		vector<double> &ra=a;
		vector<double> &rb=b;
		vector<vector<double>>& rc=c;
		vector<vector<double>> &re=e;
		//unique_ptr<vector<double>>& rpH=pH;

		double d=0;
		unique_ptr<vector<vector<double>>> & rpM=pM_old[nV];
		//unique_ptr<vector<vector<double>>> & rpMN=pM_new[nV];

		/*cout<<endl<<"ph"<<endl;
		for(auto i:*pH){
			cout<<i<<" ";
		}
		cout<<endl<<"PMold"<<endl;
		for(auto i:*pM_old[nV]){
			for(auto j:i){
				cout<<j<<" ";
			}
		}
		cout<<endl;*/

		double denum=0;
		if (pM_old[nV] && pH){
			Cal_MV(rpM,rpH,ra);
		//	cout<<"hi1"<<endl;
			Cal_VM(rpH,rpM,rb);
		//	cout<<"hi2"<<endl;
			Cal_VVM(ra,rb,rc);
		//	cout<<"hi3"<<endl;
			d=Cal_VVS(rb,rpH);
			//cout<<"hi4"<<endl;
			//cout<<"it is d= "<<d<<endl;
			denum=d+forget;
		//	cout<<"it is denum= "<<denum<<endl;
			//cout<<"hi5"<<endl;
			Cal_MtoS(rc,denum,re);
			/*cout<<"rc"<<" ";
			for(auto i:rc){
				for(auto j:i){
					cout<<j<<" ";
				}
			}
			cout<<endl;
			cout<<" re"<<" ";
			for(auto i:re){
							for(auto j:i){
								cout<<j<<" ";
							}
						}
			cout<<endl;*/
		//	cout<<"hi6"<<endl;
			pM_new[nV].reset();
			pM_new[nV]=(Cal_Msub(rpM,re));
		//	cout<<"hi7"<<endl;
		/*	for(auto i:*rpM){
				for(auto j:i){
					cout<<j<<" ";
				}
			}
			cout<<endl;*/
		}
		else{
			cout<<endl<<"Error in pointer handling 1"<<endl;
			exit(1);
		}

	}



}
/*******************************************************************/
/*******************************************************************
CalGn: p(n)*H(n)
Use:				Calculate g(n) in p.248, ch.5, Hykin book. g(n)=p(n)*H(n)
Out:				Obtain g(n)
Status:				private and called from RLS()
PreConditions:		p(n) is calculated by the CalPn() and H(n) is obtain from cal_firingStrenght.
Postconditions:		Fill g(n)
Invariant:
 *******************************************************************/
void inputVecWeight::CalGn(){

	//unique_ptr<vector<double>>& rpH=pH;

	for(int nV=0;nV<numOutput;nV++){

		vector<double> &ra=G[nV];
		unique_ptr<vector<vector<double>>> & rpM=pM_new[nV];
		if(pM_new[nV]&& pH){
			Cal_MV(rpM,rpH,ra);
			//G[nV]=ra;
		}
		else{
			cout<<endl<<"Error in pointer handling 2"<<endl;
			exit(1);
		}

	}
/*cout<<endl<<"gn"<<endl;*/
	/*for(int nV=0;nV<numOutput;nV++){

			vector<double> &re=G[nV];
			vector<double> a(numWeight,0);
			vector<double> &ra=a;
			vector<double> b(numWeight,0);
			vector<double> &rb=b;
			double d=0;
			unique_ptr<vector<vector<double>>> & rpM=pM_old[nV];
			double denum=0;
			if(pM_old[nV]&& pH){
			Cal_MV(rpM,rpH,ra);
			Cal_VM(rpH,rpM,rb);
			d=Cal_VVS(rb,rpH);
							//cout<<"hi4"<<endl;
			denum=d+forget;
			Cal_VtoS(ra,denum,re);
				//G[nV]=ra;
			cout<<"G= ";
			for(auto i:G[nV]){
				cout<<i<<" ";
			}
			}
			else{
				cout<<endl<<"Error in pointer handling 2"<<endl;
				exit(1);
			}

		}*/
}
/*******************************************************************/
/*******************************************************************
CalAlpha()
Use:				Calculate a(n)= d(n)-wt(n-1)*H(n)
Out:				a(n)
Status:				Private and called from RLS()
PreConditions:		d(n)(:target), pW_old(:w(n-1)) and H(n) are valid
Postconditions:
Invariant:
 *******************************************************************/
void inputVecWeight::CalAlpha(){

	/*cout<<endl<<"alpha"<<endl;*/
	for (int nV=0; nV<numOutput;nV++){

		vector<double> &rb=(*pW_old[nV]);
		if(pW_old[nV] && pH){
			alpha[nV]=(*target)[nV]-Cal_VVS(rb,rpH);
		//	alpha[nV]=Cal_VVS(rb,rpH)-(*target)[nV];
			/*cout<<alpha[nV]<<" ";*/
		}
		else{
			cout<<endl<<"Error in pointer handling 3"<<endl;
			exit(1);
		}

	}
}
/*******************************************************************/
/*******************************************************************
CalW
Use:				Calculate w(n)=w(n-1)+g(n)*a(n)
Out:				update wieght vector, w(n)
Status:				Private and called from RLS()
PreConditions:		pW_old and G and a are valid.
Postconditions:		return w(n)
Invariant:
 *******************************************************************/
void inputVecWeight::CalW(){

	/*cout<<endl<<"pwNew"<<endl;*/
	for(int nV=0;nV<numOutput;nV++){
		vector<double>  mul(numWeight,0);
		vector<double>&  rmul=mul;
		unique_ptr<vector<double>>& rWold=pW_old[nV];
		//unique_ptr<vector<double>>& rWnew=pW_new[nV];

		if(pW_old[nV]){
			//cout<<endl<<"rmul";
			int j=0;
			for(auto i:G[nV]){
				rmul[j]=i*alpha[nV];
			/*	cout<<"rmul= ";
				cout<<rmul[j]<<" ";
				cout<<alpha[nV]<<" ";*/
				j++;
			}

			pW_new[nV].reset();
			pW_new[nV]=(Cal_Vadd(rWold,rmul));
			/*for(auto i:*pW_new[nV]){
				cout<<i<<" ";
			}*/

		}
		else{
			cout<<endl<<"Error in pointer handling 4"<<endl;
			exit(1);
		}

	}
}
/*******************************************************************/
/*******************************************************************
Cal_Vadd
Use:				Calculate summation of two vector. It actually calculates w(n-1)+g(n)*a(n)
Out:				return the updated weights
Status:				Private and called from CalW()
PreConditions:		rWold, rmul, rWnew are valid
Postconditions:		fill rWnew with result of summation of two vectors
Invariant:

Note:
	It works based on multi-threading.the numbers of items in  the vectors are divided by numCore  giving the threads.
	So each thread is responsible for calculation of some rows.

 *******************************************************************/
unique_ptr<vector<double>> inputVecWeight::Cal_Vadd(unique_ptr<vector<double>>& rWold,vector<double> &rmul){

	unique_ptr<vector<double>> rout (new vector<double> (numWeight));

	if(numCore>1){
		mutex lck;
		int numThread=numCore;
		int rows=numWeight/numThread;
		int extras=numWeight%numThread;
		int start=0;
		int end=rows;

		vector<thread> matrix;
		for (int t=0; t<numThread;t++){

			if (t==(numThread-1)){
				end+=extras;
			}
			matrix.push_back(thread([start,end,&rWold,&rmul,&rout,this](){
				for(int i=start; i<end;i++){

					try{
						(*rout).at(i)=0;
					}
					catch(const std::out_of_range& oor){
						std::cerr <<'\n'<< "Out of Range error:Output matrix is out of range" << oor.what() << '\n';
					}
					//lck.lock();
					(*rout)[i]=(*rWold)[i]+rmul[i];
					//lck.unlock();




				}

			}));
			start=end;
			end+=rows;
		}

		for (thread &t:matrix){
			t.join();
		}
		//rWnew=move(rout);
		//rout.reset(nullptr);
		return(move(rout));
	}
	else{
		for (int i=0 ; i < (numWeight); i++){
			//(*rout).at(i)=0;
			(*rout)[i]=(*rWold)[i]+rmul[i];
		}
		//rWnew=move(rout);
		//rout.reset(nullptr);
		return(move(rout));
	}



}
/*******************************************************************/
/*******************************************************************

Cal_MV():
Use:				Calculate multiplication of matrix to vector. It actually calculates:
 	 	 	 	 	 	 p(n-1)*H(n) in CalPn()
 	 	 	 	 	 	 P(n)*H(n)   in CalGn()
Out:				Change vector a.
Status:				Private and called from CalPn() and CalGn()
PreConditions:		rpM, rpH and a are valid references
Postconditions:		We fill a with the the result of Matrix*vector
Invariant:

Note:
	H(n) is a vector in C++, and we can see it as either 1*n or n*1 matrix
	It works based on multi-threading. the numbers of rows in the matrix is divided by numCore  giving the threads.
	So each thread is responsible for calculation of some rows.

 *******************************************************************/
void inputVecWeight::Cal_MV(unique_ptr<vector<vector<double>>>&rpM,unique_ptr<vector<double>>&rpH,vector<double> &a){

	if (numCore>1){
		mutex lck;
		int numThread=numCore;
		int rows=numWeight/numThread;
		int extras=numWeight%numThread;
		int start=0;
		int end=rows;
		vector<thread> matrix;

		for (int t=0; t<numThread;t++){
			if (t==(numThread-1)){
				end+=extras;
			}
			matrix.push_back(thread([start,end,&rpM,&rpH,&a,&lck,this](){
				for(int i=start; i<end;i++){

					try{
						a.at(i)=0;

					}
					catch(const std::out_of_range& oor){
						std::cerr <<'\n'<< "Out of Range error:Output matrix is out of range" << oor.what() << '\n';
					}
					for (int j=0; j<numWeight; j++){

						//	lck.lock();
						a[i]+=(*rpM)[i][j]*(*rpH)[j];
						//	lck.unlock();

					}
				}

			}));
			start=end;
			end+=rows;
		}

		for (thread &t:matrix){
			t.join();
		}
	}
	else{
		for(int i=0 ; i <(numWeight);i++){
			a.at(i)=0;
			for(int j=0; j<(numWeight);j++){
				//cout<<"j "<< j <<"i "<< i <<"rpm "<<(*rpM)[i][j]<<"rph  "<<(*rpH)[j];
				//cout<<"rpm "<<(*rpM)[i][j]<<"rph  "<<(*rpH)[j];
				a[i]+=(*rpM)[i][j]*(*rpH)[j];
			}
		}
	}

}

/*******************************************************************/
/*******************************************************************
Cal_VM()
Use:				Calculate multiplication of vector to matrix. It actually calculate Ht(n)*p(n-1)
Out:				Change vector b
Status:				Private and called from CalPn()
PreConditions:		rpM, rpH and b are valid references
Postconditions:		We fill b with the the result of vector*matrix
Invariant:

Note:
	H(n) is a vector in C++, and we can see it as either 1*n or n*1 matrix
	It works based on multi-threading. the numbers of columns in the matrix is divided by numCore  giving the threads.
	So each thread is responsible for calculation of some column.
 *******************************************************************/
void inputVecWeight::Cal_VM(unique_ptr<vector<double>>&rpH,unique_ptr<vector<vector<double>>>&rpM,vector<double> &b){
	if(numCore>1){
		mutex lck;
		int numThread=numCore;
		int cols=numWeight/numThread;
		int extras=numWeight%numThread;
		int start=0;
		int end=cols;
		vector<thread> matrix;

		for (int t=0; t<numThread;t++){
			if (t==(numThread-1)){
				end+=extras;
			}
			matrix.push_back(thread([start,end,&rpH,&rpM,&b,&lck,this](){

				for(int i=start; i<end;i++){
					//double result=0;
					try{
						//C[i][j]=0;
						b.at(i)=0;

					}
					catch(const std::out_of_range& oor){
						std::cerr <<'\n'<< "Out of Range error:Output matrix is out of range" << oor.what() << '\n';
					}
					for (int j=0; j<numWeight; j++){
						//	lck.lock();

						b[i]+=(*rpH)[j]*(*rpM)[j][i];

						//	lck.unlock();
					}


				}

			}));
			start=end;
			end+=cols;
		}

		for (thread &t:matrix){
			t.join();
		}

	}
	else{
		for (int i=0 ; i <(numWeight);i++){
			b.at(i)=0;
			for (int j=0; j<(numWeight);j++){
				b[i]+=(*rpH)[j]*(*rpM)[j][i];
			}
		}
	}


}
/*******************************************************************/
/*******************************************************************
Cal_VVM():
Use:				Calculate multiplication of vector to vector giving a matrix. It actually calculates num= Cal_MV*Cal_VM
Out:				change matrix C
Status:				Private and called from CalPn()
PreConditions:		a and b and C are valid references
Postconditions:		We fill C with the the result of vector*vector=Matrix
Invariant:
Note:
	It works based on multi-threading. the numbers of items in the first vector is divided by numCore  giving the threads.
	So each thread is responsible for calculation of some items of first vector.
 *******************************************************************/
void inputVecWeight::Cal_VVM(vector<double> &a,vector<double> &b, vector<std::vector<double>> &C){

	if (numCore>1){
		mutex lck;
		int numThread=numCore;
		int rows=numWeight/numThread;
		int extras=numWeight%numThread;
		int start=0;
		int end=rows;
		vector<thread> matrix;

		for (int t=0; t<numThread;t++){
			if (t==(numThread-1)){
				end+=extras;
			}
			matrix.push_back(thread([start,end,&a,&b,&C,&lck,this](){
				for(int i=start; i<end;i++){
					for (int j=0; j<numWeight; j++){
						try{
							//C[i][j]=0;
							C.at(i).at(j)=0;

						}
						catch(const std::out_of_range& oor){
							std::cerr <<'\n'<< "Out of Range error:Output matrix is out of range" << oor.what() << '\n';
						}


						//	lck.lock();
						//C[i][j]+=a[i]*b[j];
						C[i][j]=a[i]*b[j];
						//	lck.unlock();

					}


				}

			}));
			start=end;
			end+=rows;
		}

		for (thread &t:matrix){
			t.join();
		}
	}
	else{
		for (int i=0 ; i < (numWeight);i++){
			for (int j=0; j<(numWeight);j++){
				C[i][j]=a[i]*b[j];
			}
		}
	}



}
/*******************************************************************/
/*******************************************************************
Cal_VVS()

Use:				Calculate multiplication of vector to vector giving a single value. It actually calculates:
 	 	 	 	 	 	 Ht(n)*Cal_VM=Ht(n)*p(n-1)*H(n) in CalPn()
 	 	 	 	 	 	 wt(n-1)*H(n) in CalAlpha()
Out:				Return a double number as result of the multiplication
Status:				public and called from CalPn(), CalAlpha and AncfisChecking object
PreConditions:		a and rpH are valid references
Postconditions:
Invariant:
 *******************************************************************/
double inputVecWeight::Cal_VVS(std::vector<double> & a,unique_ptr<vector<double>>&rpH){
	double result=0;

	for(int i=0;i< numWeight;i++){
		result+=a[i]*(*rpH)[i];
	}

	return(result);
}
/*******************************************************************/
/*******************************************************************
Cal_MtoS()

Use:				Calculate division of a matrix by a number. It actually calculates num/denum
Out:				It changes matrix e.
Status:				private and called from CalPn()
PreConditions:		c and e are valid references
Postconditions:		We fill e with result of Matrix/double=matrix
Invariant:
 *******************************************************************/
void inputVecWeight::Cal_MtoS(std::vector<std::vector<double>> &c,double denum,vector<vector<double>> &e){
	int k=0, l=0;

	for (auto i:c){
		l=0;
		for(auto j:i){
			e[k][l]=j/denum;

			l++;
		}
		k++;
	}

}
/*******************************************************************/
void inputVecWeight::Cal_VtoS(std::vector<double> &c,double denum,std::vector<double> &e){
	int k=0;
	for(auto i:c){
		e[k]=i/denum;
		k++;
	}

}
/*******************************************************************/
/*******************************************************************
Cal_Msub():
Use:				Calculate subtraction of a matrix from another matrix. It acually calculates p(n)=p(n-1)-e
Out:				Updates P(n)
Status:				private and called from CalPn()
PreConditions:		rpM, B,rpMN are valid references
Postconditions:		Fill p(n) with Matrix-Matrix=Matrix
Invariant:
Note:
	It works based on multi-threading. the numbers of rows in the first matrix is divided by numCore  giving the threads.
	So each thread is responsible for calculation of some rows of first matrix.
 *******************************************************************/
unique_ptr<vector<vector<double>>>  inputVecWeight::Cal_Msub(unique_ptr<vector<vector<double>>> & rpM,vector<vector<double>> &B){

	unique_ptr<vector<vector<double>>> pmNew (new vector<vector<double>> (numWeight, vector<double> (numWeight)));
	if (numCore>1){
		mutex lck;
		int numThread=numCore;
		int rows=numWeight/numThread;
		int extras=numWeight%numThread;
		int start=0;
		int end=rows;
		vector<thread> matrix;
		for (int t=0; t<numThread;t++){
			if (t==(numThread-1)){
				end+=extras;
			}
			matrix.push_back(thread([start,end,&rpM,&B,&pmNew,&lck,this](){
				for(int i=start; i<end;i++){
					//double result=0;

					for (int j=0; j<numWeight; j++){
						try{
							//C[i][j]=0;
							(*pmNew).at(i).at(j)=0;
							//	(*pmNew)[i][j]=0;

						}
						catch(const std::out_of_range& oor){
							std::cerr <<'\n'<< "Out of Range error:Output matrix is out of range" << oor.what() << '\n';
						}

						//	(*pM_new[nV])[i][j]=move((*pM_old[nV])[i][j]-B[i][j]);
						//	lck.lock();
						(*pmNew)[i][j]=(1/forget)*((*rpM)[i][j]-B[i][j]);
						//	lck.unlock();


					}


				}

			}));
			start=end;
			end+=rows;
		}

		for (thread &t:matrix){
			t.join();
		}
	//	rpMN=(move(pmNew));
		return(move(pmNew));
	//	pmNew.reset(nullptr);
	}
	else{

		for (int i=0; i<(numWeight);i++){

			for (int j=0; j<(numWeight);j++){
				//(*pmNew).at(i).at(j)=0;
				//cout<<"this is Msub1 "<<(1/forget)*((*rpM)[i][j]-B[i][j])<<endl;
			//	cout<<"this is Msub2 "<<((*rpM)[i][j]-B[i][j])<<endl;
				(*pmNew)[i][j]=(1/forget)*((*rpM)[i][j]-B[i][j]);
				//(*rpMN)[i][j]=(1/forget)*((*rpM)[i][j]-B[i][j]);

			}
		}
	/*	cout<<endl<<"pmNew"<<endl;
		for(auto i:*pmNew){
			for(auto j:i){
				cout<<j<<" ";
			}
		}*/
	//	rpMN=(move(pmNew));
		//pmNew.reset(nullptr);
		return(move(pmNew));

	}


}



/*******************************************************************/
void inputWeight_test1(inputVecWeight & inpW){
	cout<<endl<<"this is Pold"<<endl;

	for(int nV=0; nV<3;nV++){
		for(auto i:*(inpW.pM_old[nV])){
			for(auto j:i)
				cout<<j<<" ";
			cout<<endl;
		}
		cout<<endl;
	}
	cout<<"pH"<<endl;

	for(auto i: *(inpW.pH)){
		cout <<" "<<i;
	}
	cout<<endl<<"this is Wold"<<endl;
	for(int nV=0; nV<3;nV++){
		for(auto i:*(inpW.pW_old[nV])){
			cout<<i<<" ";
		}
		cout<<endl;
	}
}
/*******************************************************************/
void calError_test(inputVecWeight &inpW, int ep){

	/*cout<<"this is error of all epochs"<<endl;

	for(int i=0;i<=ep;i++){

		for(auto j:inpW.errorEpoch[i]){
			cout<<j<<" ";
		}
		cout<<endl;
	}*/

	cout<<endl<<"this is avebest"<<inpW.aveBest<<endl;

	/*cout<<endl<<"this is bestError"<<endl;
	for(auto i:inpW.bestError){
		cout<<i<<" ";
	}*/

	cout<<endl<<"this is W of best"<<endl;
	for(int nv=0;nv<3;nv++){
		for(auto i:(inpW.bestW[nv]) ){
			cout<<i<<" ";
		}
		cout<<endl;
	}

}
//vector<double>  input={1,6,2,3,7,4,5,8};
//vector<double>&  rrinput=input;
//	inputVecWeight inpW;
/*inpW.inputV.readData(input);
	inpW.target=inpW.inputV.readTarget();
	inpW.input=inpW.inputV.readInput();
	vector<double> &rinput=*(inpW.input);
	firingStrength fS;
	inpW.pH=fS.cal_firingStrenght(rinput);


	cout<<"We are in inputWeight_test"<<endl;

	cout<<"pH"<<endl;

	for(auto i: *(inpW.pH)){
		cout <<" "<<i;
	}
	// CAl_PN test

	vector<double> a(6,0);
	vector<double> b(6,0);
	vector<vector<double>> c(6,vector<double> (6,0));
	vector<vector<double>> e(6,vector<double> (6,0));

	vector<double> &ra=a;
	vector<double> &rb=b;
	vector<vector<double>>& rc=c;
	vector<vector<double>> &re=e;
	//unique_ptr<vector<double>>& rpH=pH;

	double d=0;*/

/*for(int nV=0; nV<3;nV++){
			unique_ptr<vector<vector<double>>> & rpM=inpW.pM_old[nV];
			unique_ptr<vector<vector<double>>> & rpMN=inpW.pM_new[nV];
			inpW.Cal_MV(rpM,inpW.rpH,ra);
			cout<<endl<<"this is a"<<endl;
			for(auto i:ra){
				cout<<i<<" ";
			}
			inpW.Cal_VM(inpW.rpH,rpM,rb);
			cout<<endl<<"this is b"<<endl;
			for(auto i:rb){
				cout<<i<<" ";
			}
			inpW.Cal_VVM(ra,rb,rc);
			cout<<endl<<"this is c"<<endl;
					for(auto i:rc){
						for(auto j:i)
							cout<<j<<" ";
						cout<<endl;
					}
			d=inpW.Cal_VVS(rb,inpW.rpH);
			cout<<endl<<"this is d= "<<d<<endl;
			double denum=0;
			denum=d+1;
			inpW.Cal_MtoS(rc,denum,re);
			cout<<endl<<"this is e"<<endl;
			for(auto i:re){
				for(auto j:i)
					cout<<j<<" ";
				cout<<endl;
			}
			//Cal_Msub(re,nV);
			inpW.Cal_Msub(rpM,re,rpMN);
			cout<<endl<<"this is rpmn"<<endl;
			for(auto i:*rpMN){
				for(auto j:i)
					cout<<j<<" ";
				cout<<endl;

		}
		}*/

/*	inpW.CalPn();
		cout<<endl<<"this is pMn"<<endl;
		for(int nV=0; nV<3; nV++){
			for(auto i:*(inpW.pM_new[nV])){
				for(auto j:i)
					cout<<j<<" ";
				cout<<endl;

			}
		}


//CAL_GN test
		inpW.CalGn();
		cout<<"this is GN"<<endl;
		for (int nV=0; nV<3;nV++){
			for(auto i:inpW.pG[nV]){
				cout<<i<<" ";
			}
			cout<<endl;
		}
	//CalAPLHA
		inpW.CalAlpha();
		cout<<endl<<"this is target"<<endl;
		for (auto i:*(inpW.target)){
			cout<<i<<" ";
		}
		cout<<endl<<"this is alpha"<<endl;
		for(int nV=0 ; nV<3;nV++){
		cout<<inpW.alpha[nV]<<" ";
		}

		//CAL_W
		cout<<endl<<"this is wold"<<endl;
		for(int nV=0; nV<3;nV++){
			for(auto i:*(inpW.pW_old[nV])){
				cout<<i<<" ";
			}
			cout<<endl;
		}
		inpW.CalW();
		for(int nV=0; nV<3;nV++){
			for(auto i:*(inpW.pW_new[nV])){
				cout<<i<<" ";
			}
			cout<<endl;
		}*/
//Replace_old_new
/*inpW.RLS();
	cout<<endl<<"this is Pold"<<endl;
	for(int nV=0; nV<3;nV++){
		for(auto i:*(inpW.pM_old[nV])){
			for(auto j:i)
				cout<<j<<" ";
			cout<<endl;
		}
		cout<<endl;
	}
	cout<<endl<<"this is Pnew "<<endl;
	for(int nV=0; nV<3;nV++){
		for(auto i:*(inpW.pM_new[nV])){
			for(auto j:i)
				cout<<j<<" ";
			cout<<endl;
		}
		cout<<endl;
	}
	cout<<endl<<"this is Wold"<<endl;
	for(int nV=0; nV<3;nV++){
		for(auto i:*(inpW.pW_old[nV])){
			cout<<i<<" ";
		}
		cout<<endl;
	}
	cout<<endl<<"this is Wnew"<<endl;
	for(int nV=0; nV<3;nV++){
		for(auto i:*(inpW.pW_new[nV])){
			cout<<i<<" ";
		}
		cout<<endl;
	}
	inpW.Replace_Old_New();
	cout<<endl<<"this is Pold after Replace"<<endl;
	for(int nV=0; nV<3;nV++){
		for(auto i:*(inpW.pM_old[nV])){
			for(auto j:i)
				cout<<j<<" ";
			cout<<endl;
		}
		cout<<endl;
	}
	cout<<endl<<"this is wold after Replace"<<endl;
	for(int nV=0; nV<3;nV++){
		for(auto i:*(inpW.pW_old[nV])){
			cout<<i<<" ";
		}
		cout<<endl;
	}*/
/*	cout<<endl<<"this is wNew after Replace"<<endl;
				for(int nV=0; nV<3;nV++){
							for(auto i:*(inpW.pW_new[nV])){
								cout<<i<<" ";
							}
							cout<<endl;
						}*/

////////////////////////////////////////////////////////////////////////////

/*void inputWeight_test(){
	inputVecWeight inpW;

	vector<vector<double>> a={{1,2},{2,1}};
	vector<vector<double>> b={{3,4},{1,5}};
	vector<vector<double>> a={{1,2},{2,1},{3,4}};
	vector<vector<double>> b={{3,4,1},{1,5,9}};

	vector<vector<double>> &ra=a;
	vector<vector<double>> &rb=b;

	for(auto i:a){
		for (auto j:i){
			cout <<" "<<j;
		}
		cout <<endl;
	}

	for(auto i:b){
		for (auto j:i){
			cout <<" "<<j;
		}
		cout <<endl;
	}

	vector<vector<double>> c(3,vector<double> (3,0));
	vector<vector<double>> &rc=c;
	inpW.matrixMulti(ra,rb,rc,3,3,2);

	cout<<endl<<"Matrix ="<<endl;

	for(auto i:c){
		for (auto j:i){
			cout <<" "<<j;
		}
		cout <<endl;
	}
	vector<vector<double>> aa={{1,2},{2,1},{3,4}};
	vector<vector<double>> bb={{3,1},{1,1},{5,4}};
	vector<vector<double>> &raa=aa;
	vector<vector<double>> &rbb=bb;
	for(auto i:aa){
		for (auto j:i){
			cout <<" "<<j;
		}
		cout <<endl;
	}

	for(auto i:bb){
		for (auto j:i){
			cout <<" "<<j;
		}
		cout <<endl;
	}
	vector<vector<double>> cc(2,vector<double> (2,0));
	vector<vector<double>> &rcc=cc;
	inpW.matrixMultiwithT1(raa,rbb,rcc,3,2,2);
	cout<<endl<<"Matrix2 ="<<endl;

	for(auto i:cc){
		for (auto j:i){
			cout <<" "<<j;
		}
		cout <<endl;
	}

	vector<vector<double>> ccc(3,vector<double> (3,0));
	vector<vector<double>> &rccc=ccc;
	inpW.matrixMultiwithT2(raa,rbb,rccc,2,3,3);
	cout<<endl<<"Matrix3 ="<<endl;

	for(auto i:ccc){
		for (auto j:i){
			cout <<" "<<j;
		}
		cout <<endl;
	}
}*/
