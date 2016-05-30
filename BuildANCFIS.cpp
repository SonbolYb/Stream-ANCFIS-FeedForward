/*
 * BuildANCFIS.cpp
 *
 *  Created on: Feb 18, 2016
 *      Author: sonbol
 */

#include "BuildANCFIS.h"
using namespace std;
BuildANCFIS::BuildANCFIS():commandLine(),dimension(NULL),
		delay(NULL),newData(NULL),finalWeight(NULL),sumSign(numVariate),bool1(50,0),bool2(50,0){
	inputOrigin=new vector<vector<double>> [per10];
	surodata=new vector<vector<double>> [1];
	delayVectors=new vector<vector<double>> [1];
	newDV=new vector<double> [1];
	mfParam=new vector<vector<vector<double>>> [1];
	//	cout<<"BuildAncfis1"<<endl;
}

BuildANCFIS::~BuildANCFIS() {
	// TODO Auto-generated destructor stub
	//	cout<<"BuildAncfis2"<<endl;
	delete[] surodata;
	delete [] inputOrigin;
	delete[] delayVectors;
	delete [] newDV;

}

void BuildANCFIS::findWeight(){

	/*It simulate the stream data becuase here we have a file that we want pass data one by one*/
	while(!InS.EndofFile()){
		/*Original window*/
		if (InS.numpassedInput==0){

			*inputOrigin=*(InS.getOrigWindowN());
			findDV();
			findMFParam();
			buildNet.build(delayVectors, mfParam,dimension,LengthSurodata,LengthDVSet);
		}

		//TODO: here instead of sliding window, I will work with the whole window moved not the sliding window for comparing windows for power spectral
		if(InS.numpassedInput >= per10){	//for new data coming
			//TODO: by coming new data points min and max can be different which change normalization and can change the all input

			*(inputOrigin)=*(InS.moveWindowbyOneNormal());
			//		findMFParamConceptDrift(inputOrigin);
			if(winCouter==per10-1){//We have a new non-overlapping window
				//	cout<<"# of data passed"<<InS.numpassedInput<<endl;
				findMFParamConceptDrift(inputOrigin);
				winCouter=0;
			}
			winCouter++;
			vector<int> * headEndInx=InS.getHeadEndInx();
			*newDV=*(DV.getNewDV(inputOrigin,delay,dimension,headEndInx));
			finalWeight=buildNet.updataWeight(newDV,mfParam,dimension,LengthSurodata,LengthDVSet);
			//	inputOrigin=InS.moveWindowbyOne();
			/*cout<<"this is input by move"<<endl;-
		for(auto i:*inputOrigin){
			for (auto j:i){
				cout <<j <<" ";
			}
			cout<<endl;
		}
		cout<<endl;*/
			//	DV.addNewDV(newData);

			/*cout << "this is headEndInx"<<endl;
		for(auto i:*headEndInx){
			cout<<i<<" ";
		}*/

			/*cout<<"this is new DV"<<endl;
		for(auto i:*newDV){
			cout<<i <<" ";
		}*/
			//delayVectors=DV.getNewDelayVe/cs(inputOrigin,delay,dimension,headEndInx);
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
			/*for(auto i:(*mfParam)){
				for(auto j:i){
					for(auto k:j){
						cout<<k<<" ";
					}
				}
			}
			cout<<endl;*/
		}
	}
	closeParamFile();
	buildNet.getStreamParam();
	PowerAnalysis();
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
	*delayVectors=*(DV.getDV(inputOrigin,delay,dimension));
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

	*surodata=*(InS.getSurWindow(inputOrigin));
	//surodata=InS.getSurWindow();
	//	mfPar.findMfparam(surodata);
	LengthSurodata=((*surodata)[0].size())/2+1;
	//LengthSurodata=((*surodata)[0].size());
	for (int i=0; i<numOutput;i++){
		if(LengthSurodata <= numOfMF[i]){
			numOfMF[i]=LengthSurodata-1;
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
	myfile<<endl;
	myfile.close();

	*mfParam=*(mfPar.getMfparam(surodata));
	//*mfParam=*(mfPar.getMfparam(inputOrigin));
	saveDDMF();
	oldPower=*(mfPar.getPower());
	//cout<<" oldpower="<<(oldPower)[0][0];
	saveMFparam(mfParam,mfPar.getPower());
}
void BuildANCFIS::findMFParamConceptDrift(vector<vector<double>>* inputWin){

	vector<vector<double>> *surodataCheck=InS.getSurWindow(inputWin);
	vector<vector<vector<double>>> mfParamCheck=*mfPar.getMfparam(surodataCheck);

	//vector<vector<vector<double>>> mfParamCheck=*mfPar.getMfparam(inputWin);


	vector<vector<double>> *power=mfPar.getPower();


	if(CheckDrift2(power)){
		/*
		for(auto i:(*mfParam)){
						for(auto j:i){
							for(auto k:j){
								cout<<k<<" ";
							}
						}
					}
					cout<<endl;*/
		*mfParam=mfParamCheck;
		oldPower=*power;
		saveMFparam(&mfParamCheck,power);
		/*for(auto i:oldPower){
			for(auto j:i){
				cout<<"oldpower= "<<j<<endl;
			}
		}*/
		/*for(auto i:(*mfParam)){
			for(auto j:i){
				for(auto k:j){
					cout<<k<<" ";
				}
			}
		}
		cout<<endl;*/
	}
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
	/*cout<<endl<<"this is MF params"<<endl;
	for(auto i: *mfParam){
		for(auto j:i){
			for(auto k:j){
				cout<<k<<" ";
			}

		}
	}
	cout<<endl;*/
	fstream myfile;

	//throw exception if the file cannot be opened
	myfile.exceptions(ifstream::failbit|ifstream::badbit);
	try{
		myfile.open("FinalParams.txt",ios::app|ios::out);
	}
	catch(fstream::failure &e){
		cerr << "Exception opening/reading/closing file\n";
	}

	myfile<<endl<<"Delay:"<<endl;
	for(auto i:*delay){
		myfile<<i<<" ";
	}
	myfile<<endl<<"Dimension"<<endl;
	for(auto i:*dimension){
		myfile<<i<<" ";
	}
	/*myfile<<endl<<"MfParams"<<endl;
	for(auto i:*mfParam){
		for(auto j:i){
			for(auto k:j){
				myfile<<k<<" ";
			}

		}
		myfile<<endl;
	}*/
	myfile<<endl;
	myfile.close();



}
bool BuildANCFIS::CheckDrift2(vector<vector<double>> * power){

	fstream myfile,myfile2;
	myfile.exceptions(fstream::badbit|fstream::failbit);
	try{
		myfile.open("FinalParams.txt",ios::app|ios::out);
	}
	catch(fstream::failure &e){
		cerr << "Exception opening/reading/closing file\n";
	}
	myfile2.exceptions(fstream::badbit|fstream::failbit);
	try{
		myfile2.open("MFparams.txt",ios::app|ios::out);
	}
	catch(fstream::failure &e){
		cerr << "Exception opening/reading/closing file\n";
	}

	bool out2=false;
	for (int j=0; j<numVariate;j++){
		if((*power)[j][0] < 0.5*(oldPower)[j][0]){

			myfile<<"concept of Drift1"<<endl;
			myfile<<"variate= "<<j<<endl;
			myfile<<"# of data passed:\t"<<InS.numpassedInput<<endl;
			myfile<<"number of window passed:\t"<<InS.numpassedInput/per10<<endl;
			myfile2<<"concept of Drift1"<<endl;
			myfile2<<"variate= "<<j<<endl;
			myfile2<<"# of data passed:\t"<<InS.numpassedInput<<endl;
			myfile2<<"number of window passed:\t"<<InS.numpassedInput/per10<<endl;

			out2=true;
		}
		else if ((*power)[j][0]*0.5> (oldPower)[j][0]){
			myfile<<"concept of Drift2"<<endl;
			myfile<<"variate= "<<j<<endl;
			myfile<<"# of data passed:\t"<<InS.numpassedInput<<endl;
			myfile<<"number of window passed:\t"<<InS.numpassedInput/per10<<endl;
			myfile2<<"concept of Drift2"<<endl;
			myfile2<<"variate= "<<j<<endl;
			myfile2<<"# of data passed:\t"<<InS.numpassedInput<<endl;
			myfile2<<"number of window passed:\t"<<InS.numpassedInput/per10<<endl;

			out2=true;
		}
		else{
			out2=false;
		}


	}
	myfile.close();
	return(out2);
}
bool BuildANCFIS::CheckDrift(vector<vector<double>> * power){
	int m=50;
	bool out=false;
	//	cout<<"oldPower= "<<(oldPower)[0][0];
	for (int j=0; j<numVariate;j++){

		if(signContainer.size()==5){
			sumSign[j]-=(signContainer.front());
			signContainer.pop_front();
			if(sumSign[j]==5 || sumSign[j]==-5){
				cout<<"concept drift type graduale"<<endl;
			}
		}
		if(checkDrft.size()==m-1){
			//		vector<bool> bool1 (12,false);
			//	cout<<checkDrft[0]<<" "<<checkDrft[1]<<" "<<checkDrft[2]<<" "<<checkDrft[3]<<" "<<checkDrft[4]<<" "<<checkDrft[5]<<" "<<checkDrft[6]<<" "<<checkDrft[7]<<" "<<checkDrft[8]<<" "<<  endl;
			for(int k=0;k<m;k++){

				if (k==0){
					bool1[0]=1;

				}
				else if(k==m/2){
					if(checkDrft[k-1]==1 ||checkDrft[k-1]==-1){
						bool1[k]=1;


					}

					else{
						bool1[k]=0;

					}

					///cout<<bool1[k]<<" ";
				}
				else{
					if(checkDrft[k-1]==0){
						bool1[k]=1;
					}
					else{
						bool1[k]=0;
					}
					//	cout<<bool1[k]<<" ";
				}

			}
			int sum=0;
			int sum1;
			for (auto it=bool1.begin()+1; it != bool1.end();it++){
				if(*it ==1){
					sum++;
				}
				if(it >= bool1.begin()+(m/2) &&it <= bool1.end() &&*it==1){
					sum1++;
				}
			}

			if (sum==m-1){
				cout<<"real pulse drift111"<<endl;
				out=true;
			}
			if (sum1==m/2 &&sum!=m-1){
				cout<<"real pulse drift222"<<endl;
				out=true;
			}


			if(InS.numpassedInput <11+m/2-1+per10){
				//	cout<<checkDrft[0]<<" "<<checkDrft[1]<<" "<<checkDrft[2]<<" "<<checkDrft[3]<<" "<<checkDrft[4]<<" "<<checkDrft[5]<<" "<<checkDrft[6]<<" "<<checkDrft[7]<<" "<<checkDrft[8]<<" "<<  endl;
				for(int k=0;k<m/2+1;k++){
					if(k==0){
						bool2[k]=1;
					}
					else if(k==1){
						if(checkDrft[k-1]==1 ||checkDrft[k-1]==-1){
							bool2[k]=1;


						}

						else{
							bool2[k]=0;

						}
					}
					else{
						if(checkDrft[k-1]==0){
							bool2[k]=1;
						}
						else{
							bool2[k]=0;
						}

					}

				}
			}

			int sum3=0;
			for (auto it=bool2.begin()+1; it!=bool2.begin()+m/2+1;it++){
				//cout<<*it<<" ";
				if(*it ==1){
					sum3++;
				}}
			if(sum3==m/2){
				cout<<"real pulse drift333"<<endl;
				out=true;
			}
			//	cout<<checkDrft[0]<<" "<<checkDrft[1]<<" "<<checkDrft[2]<<" "<<checkDrft[3]<<" "<<checkDrft[4]<<" "<<checkDrft[5]<<" "<<checkDrft[6]<<" "<<checkDrft[7]<<" "<<checkDrft[8]<<" "<<  endl;
			/*		if(checkDrft[0]==0&&checkDrft[1]==0&&checkDrft[2]==0&&checkDrft[3]==0&&(checkDrft[4]==1||checkDrft[4]==-1)&&checkDrft[5]==0&&checkDrft[6]==0&&checkDrft[7]==0&&checkDrft[8]==0){
				cout<<"real pulse drift1"<<endl;
				out=true;
			}
			else if((checkDrft[4]==1||checkDrft[4]==-1)&&checkDrft[5]==0&&checkDrft[6]==0&&checkDrft[7]==0&&checkDrft[8]==0){
				cout<<"real pulse drift2"<<endl;
				out=true;
			}
			if(checkDrft.size()==9 && InS.numpassedInput <15+per10){//15=first data points (11)+first half(4): if it happens in the first half of first data we cannot catch it
				if((checkDrft[0]==1||checkDrft[0]==-1)&&checkDrft[1]==0&&checkDrft[2]==0&&checkDrft[3]==0&&(checkDrft[4]==0)){
					cout<<"real pulse drift3"<<endl;
					out=true;
				}

			}*/


			checkDrft.pop_front();

		}


		if((*power)[j][0] < 0.3*(oldPower)[j][0]){
			signContainer.push_back(-1);
			checkDrft.push_back(-1);
			sumSign[j]+=-1;

		}
		else if ((*power)[j][0]> 1.7*(oldPower)[j][0]){
			signContainer.push_back(1);
			checkDrft.push_back(1);
			sumSign[j]+=1;
		}
		else{
			signContainer.push_back(0);
			checkDrft.push_back(0);
			sumSign[j]+=0;
		}

	}
	//TODO: Problem is here
	oldPower=*power;
	//cout<<"oldPower1= "<<(oldPower)[0][0];
	return (out);
}
void BuildANCFIS::PowerAnalysis(){
	//work with powerStore



	/*
	for(int j=0; j< numVariate;j++){
		deque<int> signContainer1;
		deque<int> check;
		int sum=0;
		for(int i=1; i<InS.getNumpassedWindow();i++){
			vector<vector<double>> & w1=powerStore[i];
			vector<vector<double>> &w2=powerStore[i-1];

			if(signContainer1.size()==5){
				sum-=(signContainer1.front());
				signContainer1.pop_front();
				if(sum==5 || sum==-5){
				//	cout<<"concept drift type graduale"<<endl;
				}

			}

			if(check.size()==9){
				//		cout<<check[0]<<" "<<check[1]<<" "<<check[2]<<" "<<check[3]<<" "<<check[4]<<" "<<check[5]<<" "<<check[6]<<" "<<check[7]<<" "<<check[8]<<" "<<  endl;
				if(check[0]==0&&check[1]==0&&check[2]==0&&check[3]==0&&(check[4]==1||check[4]==-1)&&check[5]==0&&check[6]==0&&check[7]==0&&check[8]==0){
				//	cout<<"real pulse drift1"<<endl;
				}
				else if((check[4]==1||check[4]==-1)&&check[5]==0&&check[6]==0&&check[7]==0&&check[8]==0){
			//		cout<<"real pulse drift2"<<endl;
				}
				if(check.size()==9 && i <15){//15=first data points (11)+first half(4): if it happens in the first half of first data we cannot catch it
					if((check[0]==1||check[0]==-1)&&check[1]==0&&check[2]==0&&check[3]==0&&(check[4]==0)){
			//			cout<<"real pulse drift3"<<endl;
					}

				}

				if(check[0]==0 &&(check[1]==1||check[1]==-1)&&check[2]==0){
			//		cout<<"j= "<<j<<"i= "<<i<<"real pulse drift"<<endl;


				}
				check.pop_front();

			}
			//cout<<"w1= "<<w1[j][0]<<"1.2w2= "<<w2[j][0]<<endl;
			if(w1[j][0]<0.5*w2[j][0]||w1[j][0]>1.5*w2[j][0]){
			//	cout<<"j= "<<j<<"i= "<<i<<"concept drift type pulse"<<endl;
				cout<<"w1= "<<w1[j][0]<<"1.2w2= "<<w2[j][0]<<endl;
			}
			//TODO: work on these conditions for gradual concept drift. they make sense for pulse
			if(w1[j][0] < 0.5*w2[j][0]){
				signContainer1.push_back(-1);
				check.push_back(-1);
				sum+=-1;

			}
			else if (w1[j][0]> 1.5*w2[j][0]){
				signContainer1.push_back(1);
				check.push_back(1);
				sum+=1;
			}
			else{
				signContainer1.push_back(0);
				check.push_back(0);
				sum+=0;
			}

			if(w1[j][0] < 0.8*w2[j][0]||w1[j][0]> 1.2*w2[j][0]){
				cout<<"concept Drift"<<endl;
			}
		}



		//	comparePower(&powerStore[i],&powerStore[i-1]);

	}*/
}
void BuildANCFIS::saveMFparam(vector<vector<vector<double>>> * mfPar,vector<vector<double>>* power){
	//TODO: Find a better way to do instead of push_back because it is time consuming
	powerStore.push_back(*power);

	cout<<endl<<"this is MF params"<<endl;
	for(auto i: *mfParam){
		for(auto j:i){
			for(auto k:j){
				cout<<k<<" ";
			}

		}
	}
	cout<<endl;
	fstream myfile,myfile2;
	myfile2.exceptions(ifstream::failbit|ifstream::badbit);
	try{
		myfile2.open("MFparams.txt",ios::app|ios::out);
	}
	catch(fstream::failure &e){
		cerr << "Exception opening/reading/closing file\n";
	}
	myfile.exceptions(ifstream::failbit|ifstream::badbit);
	try{
		myfile.open("FinalParams.txt",ios::app|ios::out);
	}
	catch(fstream::failure &e){
		cerr << "Exception opening/reading/closing file\n";
	}
	myfile2<<"MF Params:"<<endl;
	myfile<<"MF Params:"<<endl;
	for(auto i:*mfPar){
		for(auto j:i){
			for(auto k:j){
				myfile2<<k<<" ";
				myfile<<k<<" ";
			}
		}

	}
	myfile<<endl;
	myfile2<<endl<<"Power:"<<endl;
	for(auto i:*power){
		for (auto j:i){
			myfile2<<j<<" ";
		}
	}
	myfile2<<endl;
	/*myfile2<<"number of data passed:"<<endl;
	myfile2<<InS.numpassedInput<<endl;*/
	/*myfile2<<"number of window passed:"<<endl;
	myfile2<<InS.numpassedInput/per10<<endl;*/

	myfile2.close();
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
void BuildANCFIS::closeParamFile(){
	fstream myfile1;
	myfile1.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		myfile1.open("MFparams.txt",ios::app|ios::out);
	}
	catch (fstream::failure &e){
		cerr << "Exception opening/reading/closing file\n";
	}
	myfile1 <<"***************************************"<<endl;
	myfile1.close();
}
int BuildANCFIS::getLengthSurodata(){
	return(LengthSurodata);
}
