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
//TODO: this number should be defined as a variable
InputStream::InputStream():commandLine(),origWindowS(per10),origWindow(numVariate, vector<double>(per10)),origWindowN(numVariate, vector<double>(per10)),surWindow(numVariate),
		surWindowN(numVariate),newdata(numVariate),newdataN(numVariate),max(numVariate),min(numVariate) {
	// TODO Auto-generated constructor stub

}
/******************************************************************/
InputStream::~InputStream() {
	// TODO Auto-generated destructor stub
}
/*******************************************************************

 *******************************************************************/
void InputStream::readOrigWindow(){

	fstream input;
	input.exceptions(ifstream::failbit|ifstream::badbit);
	try{
		input.open(TrainFile1,ios::in);
	}
	catch(fstream::failure &e){
		cerr << "Exception opening/reading/closing file1\n";
	}
	//TODO: This line does not work with CSV files. It works just with delim of space. Make it work for csv with ; as delim

	//TODO: Change everything to i and j instead of j and i

	for (int i=0; i<per10;i++){
		for(int j=0; j< numVariate; j++){
			input >> origWindow[j][i];

		}
	}



	numpassedInput=per10;

}
/******************************************************************/
void InputStream::readOrigWindowN(){
	readOrigWindow();
	vector<vector<double>> &p=origWindow;
	vector<vector<double>> &p1=origWindowN;
	normalize(p,p1);


}
/*******************************************************************

 *******************************************************************/
void InputStream::readNewData(){
	numpassedInput+=1;
	fstream input(TrainFile1);
	GotoLine(input,numpassedInput);

	for (int i=0; i<numVariate;i++){
		input >> newdata[i];
	}


}
/******************************************************************/
/*******************************************************************

 *******************************************************************/
//TODO:NewdataN is based on max and min of the original window. Is it right to keep it as this or we should define and update the normalization some how?
void InputStream::readNewDataN(){
	readNewData();
	for(int i=0; i<numVariate;i++){

		newdataN[i]=(newdata[i]-min[i])/(max[i]-min[i]);
	}


}
/******************************************************************/
/*******************************************************************

 *******************************************************************/
vector<double>* InputStream::getNewData(){
readNewData();
return(&newdata);

}
/******************************************************************/
/*******************************************************************

 *******************************************************************/
vector<double>* InputStream::getNewDataN(){
readNewDataN();
return(&newdataN);

}
/******************************************************************/
/*******************************************************************
//TODO: for surogate data, I cur the data from begining because of the code of jump vector<double>::const_iterator first=x[m].begin()+(nj);
 *******************************************************************/
void InputStream::readSurData(){
	//TODO: we should be sure that readOrigWindow called before readSurData otherwise, we have to call readOrigWindow here as well which just take more time
	//to read the data from file again
	//SreadOrigWindow();
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

 *******************************************************************/
void InputStream::readSurDataN(){
	readSurData();
	//readOrigWindowN();
	//endtoend();
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

 *******************************************************************/
vector<vector<double>> * InputStream::getOrigWindow(){
	//	read1stInput();
	readOrigWindow();
	vector<vector<double>> * pOrigWindow= &origWindow;
	return pOrigWindow;
}
/******************************************************************/
/*******************************************************************

 *******************************************************************/
vector<vector<double>> * InputStream::getOrigWindowN(){
	//	read1stInput();
	readOrigWindowN();


	vector<vector<double>> * pOrigWindowN= &origWindowN;

	return (pOrigWindowN);
}
/*******************************************************************/
/*******************************************************************

 *******************************************************************/
vector<vector<double>> * InputStream::getSurWindow(){
	//read1stInput();
	readSurData();

	vector<vector<double>> * pSurWindow= &surWindow;
	return pSurWindow;
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

 *******************************************************************/
//TODO:Check the file
void InputStream::normalize(const vector<vector <double>>& vec,vector<vector <double>> &vec2){
	for (int i=0; i <numVariate;i++){
		vector<double> vec1=vec[i];
		 max[i]=*max_element(vec1.begin(), vec1.end());
		 min[i]=*min_element(vec1.begin(),vec1.end());
		//	cout <<"max"<<" "<<max<<" "<<"min"<<" "<<min<<endl;
		int m=0;
		for (auto j:vec1){
			vec2[i][m]=(j-min[i])/(max[i]-min[i]);
			m++;
		}

	}


}
/******************************************************************/
/*******************************************************************
Based on Fortran code in Tisean
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
	double ejump, ejump1;
	double *pejump=&ejump;
	double eslip,eslip1;
	double *peslip=&eslip;
	double njump,njump1;
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
	return etot;
}
/******************************************************************/
/*******************************************************************

 *******************************************************************/
double InputStream::xjump(int nmax,vector<double> x){
	double xjump1=0;
	double sum=accumulate(x.begin(),x.end(),0.0);
	double mean=sum/x.size();
	double std1;
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
	double std1;
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
	return file;
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
