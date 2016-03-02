/*
 * DelayVector.cpp
 *
 *  Created on: Jan 28, 2016
 *      Author: sonbol
 */
#include "InputStream.h"
#include "DelayVector.h"
using namespace std;

DelayVector::DelayVector():duration(numVariate){
	// TODO Auto-generated constructor stub

}

DelayVector::~DelayVector() {
	// TODO Auto-generated destructor stub
}

void DelayVector::getInput(){
	//pOrigWindow=inS.getOrigWindow();

}

/*******************************************************************

 *******************************************************************/

/******************************************************************/

/*******************************************************************

 *******************************************************************/

/******************************************************************/
void DelayVector::getDelayVector(){

}
std::vector<double>* DelayVector::getNewDV(){

}
void DelayVector::addNewDV(std::vector<double>*){

}
void DelayVector::findDV(std::vector<std::vector<double>> *, int, int){

}
vector<vector<double>>* DelayVector::getDV(vector<vector<double>> * inpt,vector<int>* del, vector<int>* dim){
	delay=del;
	dimension=dim;
	input=inpt;
	findDur();
	resizeDV();
	makeDV();
	return (&delV);


}

vector<vector<double>>* DelayVector::getDVforDim(vector<vector<double>> * inpt,vector<int>* del, vector<int>* dim){
	delay=del;
	dimension=dim;
	input=inpt;
	findDur();
	resizeDVforDim();
	makeDVforDim();
	return (&delV);


}
void DelayVector::findDur(){

	for (int i=0; i< numVariate; i++){
		duration[i]=(*delay)[i]*((*dimension)[i]-1)+1;

	}
	maxDur=*max_element(duration.begin(),duration.end());
	cout <<endl<<"this is dur= "<<maxDur<<endl;
	size=((*input)[0]).size()-maxDur;
	int sum=0;
	for (auto i:*dimension){
		sum+=i;
	}
	sumDim=sum;
	cout << "dim "<<sumDim<<" size "<<size<<endl;


}
int DelayVector::getsize(){ //We use it in the dimension and in making NN box.
	return(size);

}
void DelayVector::resizeDV(){
	delV.resize(size);
	for(int i=0; i <size;i++){
		delV[i].resize(sumDim+numVariate);
	}
}
void DelayVector::resizeDVforDim(){
	delV.resize(size);
	output.resize(size);
	for(int i=0; i <size;i++){
		delV[i].resize(sumDim);
		output[i].resize(numVariate);
	}
}
void DelayVector::makeDV(){

	int k=0;
	int n=0;

	for (int i=maxDur; i<size+maxDur;i++){

		n=0;
		for(int l=0;l<numVariate;l++){
			for (int j=0; j< (*dimension)[l];j++){
				//cout << (*input)[l][0];
				//cout << (*input)[l][2];
				//cout <<" this"<<i-(*delay)[l]*(j-1)-1;
				delV[k][n]=(*input)[l][i-(*delay)[l]*(j-1)-2];

				//	cout <<delV[k][n];
				n++;

			}
			delV[k][n]=(*input)[l][i];
			//cout <<delV[k][n];
			n++;

		}

		k=k+1;


	}


}
void DelayVector::makeDVforDim(){ //It is just for use in the dimension class. Because it does not have prediction in the delay vectors
	int k=0;
	int n=0;
	int h=0;
	for (int i=maxDur; i<size+maxDur;i++){

		n=0;
		for(int l=0;l<numVariate;l++){
			for (int j=0; j< (*dimension)[l];j++){
				//cout << (*input)[l][0];
				//cout << (*input)[l][2];
				//cout <<" this"<<i-(*delay)[l]*(j-1)-1;
				delV[k][n]=(*input)[l][i-(*delay)[l]*(j-1)-2];

				//	cout <<delV[k][n];
				n++;

			}
			//TODO:not sure just check it the output if it works
			output[h][l]=(*input)[l][i];

		}
		h++;
		k=k+1;


	}


}

vector<vector<double>> * DelayVector::getOutputforDim(){
	return (&output);
}
void DV_test(){
	InputStream inS;
	DelayVector DV;
	int ** allIndices;
	allIndices=new int*[10];
	for (int i=0; i<10;i++){
		allIndices[i]=new int[4];
	}
	vector<vector<double>> *result;
	vector<int> delay {1,1};
	vector<int> dim {2,2};
	vector<int>* del=&delay;
	vector<int>* di=&dim;


	result=DV.getDV(inS.getOrigWindowN(),del,di);
	double *v1=&((*result)[0][0]);
	cout << (*result)[0][0];
	cout <<"this is delay vector"<<endl;
	for (auto i:*result){
		for (auto j:i){
			cout << j << " ";
		}
		cout <<endl;
	}
	cout <<"this is delay vector2"<<endl;
		for (int i=0;i <40;i++){
			cout <<*(v1+i)<<" ";
		}

	//	flann::Matrix_ dataset1(10,4);
	//flann::Matrix<double> data1(5);
	flann:: Matrix<double> dataset;
	flann::Matrix<double> query;
	dataset=flann::Matrix<double> (new double [10*4],10,4);
	query=flann::Matrix<double>(new double [1*4],1,4);
	int nn=3;

	flann::Matrix<double> *dd=&dataset;


	for (int i=0; i< 10;i++){
		for (int j=0; j<4;j++){
			dataset[i][j]=(*result)[i][j];
		}
	}
	double *v=&((*result)[0][0]);
	for (int j=0;j<10;j++){
		for(int i=0; i<4;i++){
			query[0][i]=(*result)[j][i];

			cout<<query[0][i];
		}


		cout<<endl<<"rows  "<<query.rows<<endl;
		flann::Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
		flann::Matrix<double> dists(new double[query.rows*nn],query.rows,nn);


		//	flann::Index<flann::L2<float> > index((const flann::Matrix<double>&) dataset, (const flann::IndexParams &)flann::KDTreeIndexParams(4),flann::FLANN_DIST_EUCLIDEAN);

		flann::Index<flann::L2<double>> index(dataset,flann::KDTreeIndexParams(4));
		index.buildIndex();

		index.knnSearch(query,indices,dists,nn,flann::SearchParams(128));
	//	allIndices[j]=indices[0];

		for (int i=0 ; i< query.rows;i++){
			for (int k=0; k<nn;k++){
				/*cout << indices[i][j]<< " ";
				cout <<allIndices[i][j]<<" ";*/
				cout<< *((indices[i])+k)<<" ";
				allIndices[j][k]=*((indices[i])+k);

			}
		}
		cout <<endl;
		//index.knnSearch(query,indices,dists,nn);


		delete[] indices.ptr();
		delete[] dists.ptr();
	}
	for (int i=0; i<10;i++){
		for(int j=0; j<nn;j++){
			cout << allIndices[i][j]<<" ";
		}
		cout <<endl;
	}


	delete[] dataset.ptr();
	delete[] query.ptr();

}
