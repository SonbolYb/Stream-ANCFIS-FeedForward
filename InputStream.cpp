/* * InputStream.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: sonbol
 */
using namespace std;
#include "InputStream.h"

//TODO: we can automate the separation of train and test. But not now. I think that even in future, we do not need it.
/*******************************************************************

 *******************************************************************/
InputStream::InputStream():commandLine(),origWindow(numVariate, vector<double>(per10)),
		origWindowN(numVariate, vector<double>(per10)),surWindow(numVariate),
		surWindowN(numVariate),newdata(numVariate),newdataN(numVariate),
		checking(numVariate,vector<double> (CheckSize)),checkingN(numVariate,vector<double> (CheckSize)),max(numVariate),min(numVariate),head(NULL),end(NULL) {
	// TODO Auto-generated constructor stub
//cout<<"inputStream1"<<endl;
}
/******************************************************************/
InputStream::~InputStream() {
	// TODO Auto-generated destructor stub
	//cout<<"inputStream2"<<endl;
}
/******************************************************************/
bool InputStream::EndofFile(){
	return(endoffile);
}
/*******************************************************************
readOrigWindow():

First, per10(10%) of data is considered as the first window
Then, add a new data to the origWindow and delete the the oldest data.
To do that, we need to have to pointers as head and end to know where
the new data should be added and from where the oldest data is deleted
We have implemented a circular array.
 *******************************************************************/
void InputStream::readOrigWindow(){

	if(numpassedInput==0){
	//	cout<<"this is input"<<endl;
		fstream input;
				input.exceptions(ifstream::failbit|ifstream::badbit);
				try{
					input.open(TrainFile1,ios::in);
				}
				catch(fstream::failure &e){
					cerr << "Exception opening/reading/closing file1\n";
				}
		//TODO: This line does not work with CSV files. It works just with delim of space. Make it work for csv with ; as delim

		/*Put per10 of data to the origWindow*/

		for (int i=0; i<per10;i++){

			for(int j=0; j< numVariate; j++){

				double data=0;

				if(!input.eof()){

					input >> data;
					origWindow[j][i]=data;

				}
			else{
					endoffile=true;
				}
			}

		}
		/*determine head and end pointers to be used in adding new data and deleting the oldest one*/
		numpassedInput=per10;	/*As we are getting the first per10 (10%) data, number of input passed is equal=10% at the end of the loop*/
		auto head1=origWindow[0].begin();
		auto end1=origWindow[0].end()-1;
		head=&(*head1);
		end=&(*end1);
		headInx=head1-origWindow[0].begin();
		endInx=end1-origWindow[0].begin();
		input.close();

	}
	else{
		/*Different conditions for making a circular array
		 * __ __ __ __ __
		 *  h |  |  |  |e
		 * __ __ __ __ __
		 *
		 * */

		if( endInx==(per10-1) &&headInx==0){
			end=head;
			head=head+1;
			endInx=0;
			headInx++;
			*end=newdata[0]; //The first variate is filled here
			for(int nV=1;nV<numVariate;nV++){ /*other variates are filled*/
				origWindow[nV][endInx]=newdata[nV];
			}

		}
				/*
				 * __ __ __ __ __
				 *  |  |  | e |h
				 * __ __ __ __ __
				 *
				 * */
		else if(headInx==(per10-1)&& endInx==(per10-2)){
			end=head;
			head=&(*origWindow[0].begin());
			endInx++;
			headInx=0;
			*end=newdata[0];
			for(int nV=1;nV<numVariate;nV++){
				origWindow[nV][endInx]=newdata[nV];
			}



		}
		else{
			end=head;
			head=head+1;
			endInx++;
			headInx++;
			*end=newdata[0];
			for(int nV=1;nV<numVariate;nV++){
				origWindow[nV][endInx]=newdata[nV];
			}
		}
		/*
		cout<<"this is new data"<<endl;
		for(auto i:newdata){
			cout<<i<<" "<<endl;
		}
		cout<<endl <<"this is new input after new data"<<endl;
		for(auto &i:origWindowL){
			for(auto j:i){
				cout << j<<" ";
			}
		}
		cout<<endl;*/
		//	cout<<"headInx= "<<headInx<<" endInx= "<<endInx<<endl;
		/*	cout<<"this is input"<<endl;
		for(auto i:origWindow){
			for(auto j:i){
				cout<<j <<" ";
			}
			cout<<endl;
		}
		 */

	}

}
/*******************************************************************
getCheckingN():

Read checking data from checking file and normalize them
 *******************************************************************/
vector<vector<double>>* InputStream::getCheckingN(){
	fstream input;
	input.exceptions(ifstream::failbit|ifstream::badbit);
	try{
		input.open(CheckFile1,ios::in);
	}
	catch(fstream::failure &e){
		cerr << "Exception opening/reading/closing file1\n";
	}
	for (int i=0; i<CheckSize;i++){
		for(int j=0; j< numVariate; j++){
			input >> checking[j][i];
		}
	}
input.close();
	vector<vector<double>> &p=checking;
	vector<vector<double>> &p1=checkingN;
	normalize(p,p1);
	return(&checkingN);
}
/*******************************************************************
getChecking():

Read checking data from checking file
 *******************************************************************/
vector<vector<double>>* InputStream::getChecking(){
	fstream input;
	input.exceptions(ifstream::failbit|ifstream::badbit);
	try{
		input.open(CheckFile1,ios::in);
	}
	catch(fstream::failure &e){
		cerr << "Exception opening/reading/closing file1\n";
	}
	for (int i=0; i<CheckSize;i++){
		for(int j=0; j< numVariate; j++){
			float data=0;
			input >> data;
			checking[j][i]=data;
		}
	}
	input.close();
	return(&checking);
}
/******************************************************************/
/*******************************************************************
getHeadEndInx: returns head and end of thecurrent window
 *******************************************************************/
vector<int>* InputStream::getHeadEndInx(){
	headEndInx={headInx,endInx};
	return(&headEndInx);
}
/******************************************************************/
/*******************************************************************
moveWindowbyOneNormal: First, we read a new data and then the new
data is added the Window so we can get the normalization value
of the new data with considering all the data in the window.

it is normalized case
 *******************************************************************/
std::vector<std::vector<double>> * InputStream::moveWindowbyOneNormal(){
	readNewData();
	return(getOrigWindowN());
}
/*******************************************************************
moveWindowbyOne: First, we read a new data and then the new
data is added the Window so we can get the normalization value
of the new data with considering all the data in the window.

it is non-normalize case
 *******************************************************************/
std::vector<std::vector<double>> * InputStream::moveWindowbyOne(){
	readNewData();
	return(getOrigWindow());
}
/*******************************************************************
readOrigWindowN: read the normalized vesion of original window
by using readorigWindow and normalized function
 *******************************************************************/
/******************************************************************/
void InputStream::readOrigWindowN(){
	readOrigWindow();
	vector<vector<double>> &p=origWindow;
	vector<vector<double>> &p1=origWindowN;
	normalize(p,p1);


}
/*******************************************************************
readNewData: it goes to a specific line in the input file based on
numpassedInput(number of passed input) and read that line

 *******************************************************************/
void InputStream::readNewData(){
	numpassedInput+=1;
	fstream input(TrainFile1);
	GotoLine(input,numpassedInput);

	for (int i=0; i<numVariate;i++){
		if(!input.eof()){
			input >> newdata[i];

		}
		else{
			endoffile=true;
		}


	}
	input.close();



}
/******************************************************************/
/*******************************************************************
 readSurData: it reads surrogate data for finding Fourier transform

 It first get the length of end to end (that is the length for Fourier
 transform that we can  be sure that there is no leak in fourier transform

 Then based on the length, we read data from original window. If the length
 of surrogate is not equal to the original window, we cut the data from
 beggining of the original data
The reason that we cut from the begining of the original window is
 because of the code of jump vector<double>::const_iterator first=x[m].begin()+(nj);

 We have to be sure that readOrigWindow is called before readSurData
 The reason that we do not call readOrigWindow here again is that
 it is duplicating reading file from input and mosylt, we read
 first origin window and then go for surrogate data
 *******************************************************************/
void InputStream::readSurData(){
	//TODO: we should be sure that readOrigWindow called before readSurData otherwise, we have to call readOrigWindow here as well which just take more time
	//to read the data from file again
	//readOrigWindow();
	/*if(origWindow.empty()){
		readOrigWindow();
	}*/
	endtoend();

	//TODO: We can do more modification here. For example consider the cases that the length of end to end is equal to the length of one of the variate


	for (int i=0; i< numVariate;i++){
		surWindow[i].resize(endtoendLength);
	}


	for (int j=0; j<numVariate; j++){
		int k=per10-endtoendLength;
		for (int i=0; i< endtoendLength;i++){	//The data cut is from the beginning not from end

			surWindow[j][i]=origWindow[j][k];
			k++;
		}
	}
}
/******************************************************************/
/*******************************************************************
readSurDataN: It is for normalized case.
 *******************************************************************/
void InputStream::readSurDataN(){
	//TODO: we need to be sure that readOrigWindowN is called once before calling this function.

	//readOrigWindowN();
	endtoend();
	//TODO: We can do more modification here. For example consider the cases that the length of end to end is equal to the length of one of the variate
	for (int i=0; i< numVariate;i++){
		surWindowN[i].resize(endtoendLength);
	}
	for (int j=0; j<numVariate; j++){
		int k=per10-endtoendLength;
		for (int i=0; i< endtoendLength;i++){

			surWindowN[j][i]=origWindowN[j][k];
			k++;
		}
	}
}
/******************************************************************/
/*******************************************************************
getOrigWindow: Retrun original window
 *******************************************************************/
vector<vector<double>> * InputStream::getOrigWindow(){
	//	read1stInput();
	readOrigWindow();
	vector<vector<double>> * pOrigWindow= &origWindow;
	return (pOrigWindow);
}
/******************************************************************/
/*******************************************************************
getOrigWindowN: Return normalization of original window
 *******************************************************************/
vector<vector<double>> * InputStream::getOrigWindowN(){
	//	read1stInput();
	readOrigWindowN();


	vector<vector<double>> * pOrigWindowN= &origWindowN;
	/*for (auto i:* pOrigWindowN){
		for(auto j:i){
			cout << j<<" ";
		}

	}
	cout<<endl;*/

	return (pOrigWindowN);
}
/*******************************************************************/
/*******************************************************************
 *******************************************************************/
vector<vector<double>> * InputStream::getSurWindow(){
	//read1stInput();
	readSurData();

	vector<vector<double>> * pSurWindow= &surWindow;
	return (pSurWindow);
}
/******************************************************************/
/*******************************************************************

 *******************************************************************/
vector<vector<double>> * InputStream::getSurWindowN(){
	//read1stInput();
	//readOrigWindowN();
	readSurDataN();

	vector<vector<double>> * pOrigWindow= &surWindowN;

	return (pOrigWindow);
}
/*******************************************************************
getNewData:Return new data
 *******************************************************************/
vector<double>* InputStream::getNewData(){
	readNewData();
	return(&newdata);

}
/******************************************************************/
/*******************************************************************

 *******************************************************************/

/*******************************************************************
normalize: Normalization function
 *******************************************************************/
//TODO:Check the file
void InputStream::normalize(const vector<vector <double>>& vec,vector<vector <double>> &vec2){
	for (int i=0; i <numVariate;i++){
		vector<double> vec1=vec[i];
		/*for (auto i:vec1){
			cout << i<<" ";
		}
		cout<<endl;*/
		max[i]=*max_element(vec1.begin(), vec1.end());
		min[i]=*min_element(vec1.begin(),vec1.end());
		//	cout <<"max"<<" "<<max<<" "<<"min"<<" "<<min<<endl;
		int m=0;
		for (auto j:vec1){
			vec2[i][m]=(j-min[i])/(max[i]-min[i]);
			m++;
		}
		/*for (auto j:vec2[i]){
			cout << j<<" ";
		}
		cout<<endl;*/

	}


}
/******************************************************************/
/*******************************************************************
endtoend: Based on Fortran code in Tisean
//TODO: My results for multivariate is different from endtoend in Tisean. Fix it or see why they are different
 *******************************************************************/
void InputStream::endtoend(){

	int nmax=(origWindow[0]).size();
	double wjump=0.5;
	int mcmax=numVariate;
	int nmaxp=nmax;
	double etotm=mcmax;
	double etot=0.0;
	int nx=nmax;
	int leng=0.0;
	double offset=0.0;
	double lost=0.0;
	double ejump=0,ejump1=0;
	double *pejump=&ejump;
	double eslip=0,eslip1=0;
	double *peslip=&eslip;
	double njump=0,njump1=0;
	double *pnjump=&njump;
	vector<vector<double>> x(mcmax,vector<double> (nmax));
	x=origWindow;

	do{
		etot=jump(nmax,nmaxp,nx,x,mcmax,wjump,pejump, peslip,pnjump);
		//	cout <<"this is etot"<<etot<<endl;
		if(etot < etotm){
			etotm=etot;
			leng=nmaxp;
			lost=((nmax-nmaxp)*100.0/nmax);
			ejump1=*pejump*100.0;
			eslip1=*peslip*100.0;
			njump1=*pnjump;
			offset=njump1;

		}
		if(etot < 1e-5)
			break;
		nmaxp-=1;
	}while(nmaxp>2);
	//cout <<"this is lost"<<lost<<endl;
	//cout << "jump" <<ejump1 <<endl<<"slip"<<eslip1<<endl<<"weighted"<<(etot*100.0)<<endl <<"offset"<<njump1<<endl;
	endtoendLength=leng;
	cout<<endl<<"this is endtoendLenght= "<<endtoendLength<<endl;
	fstream myfile;

		//throw exception if the file cannot be opened
		myfile.exceptions(ifstream::failbit|ifstream::badbit);
		try{
			myfile.open("FinalParams.txt",ios::app|ios::out);
		}
		catch(fstream::failure &e){
			cerr << "Exception opening/reading/closing file\n";
		}
		myfile<<endl<<"endtoendLength:\t\t\t"<<endtoendLength<<endl;
		myfile.close();
}
/******************************************************************/
/*******************************************************************

 *******************************************************************/
double InputStream::jump(int nmax, int nmaxp, int nx,  vector<vector<double>> x, int mcmax,  double wjump, double*pejump, double*peslip,double * pnjump){
	double etot=mcmax;
	for (int nj=0; nj<=(nmax-nmaxp);nj++){
		double xj=0;
		double sj=0;
		for (int m=0; m<mcmax;m++){
			vector<double>::const_iterator first=x[m].begin()+(nj);
			vector<double>::const_iterator last=x[m].end();
			vector<double> x1(first,last);
			xj=xj+xjump(nmaxp,x1);
			sj=sj+sjump(nmaxp,x1);
		}

		xj=xj/mcmax;
		sj=sj/mcmax;

		if((wjump*xj+(1-wjump)*sj)>etot){
			continue;

		}
		etot=wjump*xj+(1-wjump)*sj;
		*pejump=xj;
		*peslip=sj;
		*pnjump=nj;

	}
	return (etot);
}
/******************************************************************/
/*******************************************************************

 *******************************************************************/
double InputStream::xjump(int nmax,vector<double> x){
	double xjump1=0;
	double sum=accumulate(x.begin(),x.end(),0.0);
	double mean=sum/x.size();
	double std1=0;
	for(auto i:x){
		std1+=pow((i-mean),2);
	}
	double a=(x[0]-x[nmax-1]);
	xjump1=pow(a,2)/(std1);
	return(xjump1);
}
/******************************************************************/
/*******************************************************************

 *******************************************************************/
double InputStream::sjump(int nmax,vector<double> x){
	double sjump1=0;
	double sum=accumulate(x.begin(),x.end(),0.0);
	double mean=sum/x.size();
	double std1=0;;
	for(auto i:x){
		std1+=pow((i-mean),2);
	}
	double b=((x[nmax-1]-x[nmax-2])-(x[1]-x[0]));
	sjump1=pow(b,2)/(std1);
	return(sjump1);
}
/******************************************************************/
/*******************************************************************

 *******************************************************************/
int InputStream::nless(int nmaxp){
	int nless1=nmaxp;

	return(nless1);
}
/******************************************************************/
/*******************************************************************

 *******************************************************************/
fstream& InputStream:: GotoLine(std::fstream& file, unsigned int num){
	file.seekg(std::ios::beg);
	for(unsigned int i=0; i < (num - 1); ++i){
		file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
	}
	return (file);
}
/******************************************************************/

void testInputStream(){
	InputStream inS;
	//inS.readOrigWindow();
	//inS.readOrigWindowN();
	//inS.readSurData();
	//	inS.readSurDataN();

	inS.endtoend();
	cout << "This is original window"<<endl;
	for (auto i: *inS.getOrigWindow()){
		for (auto j: i){
			cout << j << " ";
		}
		cout << "   ";

	}
	cout << endl;
	cout<<"This is normalized window"<<endl;
	for (auto i: *inS.getOrigWindowN()){
		for (auto j: i){
			cout << j << " ";
		}
		cout << "   ";

	}
	cout << endl;
	cout<<"This is surogate window"<<endl;
	for (auto i: *inS.getSurWindow()){
		for (auto j: i){
			cout << j << " ";
		}
		cout << "   ";

	}
	cout << endl;
	cout<<"This is surogate Normalize window"<<endl;
	for (auto i: *inS.getSurWindowN()){
		for (auto j: i){
			cout << j << " ";
		}
		cout << "   ";

	}

	cout << "this is endtoend length"<<inS.endtoendLength<<endl;
	//	inS.getOrigWindowN();
}
