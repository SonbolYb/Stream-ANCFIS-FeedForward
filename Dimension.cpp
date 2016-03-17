/*
 * Dimension.cpp
 *
 *  Created on: Jan 29, 2016
 *      Author: sonbol
 */

#include "Dimension.h"
using namespace std;
//using namespace flann;
Dimension::Dimension(): dimension(numVariate),input(NULL),delay(NULL),vectordim(numSetDim, vector<int> (numVariate)),delayVec(NULL),AllIndices(NULL),errorDV(numSetDim) {
	// TODO Auto-generated constructor stub

}

Dimension::~Dimension() {
	// TODO Auto-generated destructor stub
}

vector<int>* Dimension::getDim(vector<vector<double>> *inp,vector<int> * del){
	input=inp;
	delay=del;

	findDimKDD();
	return (&dimension);

}

void Dimension::findDimKDD(){
	//cout<<"hello"<<endl;
//	cout <<per10<<endl;
//	cout<<numSetDim<<endl;
	vectorDim(); //fill vectordim
	for(int i=0; i< numSetDim; i++){	//for different sets of dimension
		/*for(auto l:vectordim[i]){
				cout<<l<<" ";
			}
			cout<<endl;*/
		delayVec=DV.getDVforDim(input,delay,&(vectordim[i]));

		int sum=0;
		//cout <<"this is vectorDim";
		for (auto j:vectordim[i]){
			sum+=j;
		//	cout << j<< " ";
		}
	//	cout << endl;
		NNboxwidth=sum;
		NNboxHeight=DV.getsize();
		//TODO: start from here

		if(NNboxHeight > KNear+1){
			AllIndices=new int *[NNboxHeight];
			for(int i=0; i <NNboxHeight;i++){
				AllIndices[i]=new int [KNear+1];
			}
			find3kd();
			predict1step();
			errorDV[i]=errRMSE;	//errRMSE is coming from predict1step and it gives error of a given set of dimension

		}
		else{
			errorDV[i]=1000;
		//	cout<<errorDV[i]<<endl;
		}
	//	cout<<i<<endl;
		//TODO:Initialize all Indices here.
		/*AllIndices.resize(NNboxHeight);
		for(auto i:AllIndices){
			i.resize(4);
		}*/


		//Find 3 nearest neighbors of each delay vector
		//find one step ahead prediction for each variate in the three neighbors
		//save results
	}

	findminError();
	dimension=vectordim[indexofMin];





}

void Dimension::find3kd(){
	flann:: Matrix<double> dataset;
	flann::Matrix<double> query;
	dataset=flann::Matrix<double> (new double [NNboxHeight*NNboxwidth],NNboxHeight,NNboxwidth);
	query=flann::Matrix<double>(new double [1*NNboxwidth],1,NNboxwidth); 		//we want NN of one delay vector compare to others thats why here we have [1*NNboxwidth]
	//TODO: One of the closest one is the row itself and we should ignore it. e.g. here, nn=3 just gives us 2 real NN because the other one is itself
	//int nn=4;
	//TODO: change the following lines in a way that we do not need to copy delayvec to dataset and also we have to search NN for all the delay vectors so query should go in a llop
	for (int i=0; i< NNboxHeight;i++){
		for (int j=0; j<NNboxwidth;j++){
			dataset[i][j]=(*delayVec)[i][j];
		}
	}

	for (int j=0; j<NNboxHeight;j++){ 		//each row in delay vector is considered as query once
		for(int i=0; i<NNboxwidth;i++){
			query[0][i]=(*delayVec)[j][i];
		}

		//cout<<query.rows;
		flann::Matrix<int> indices(new int[query.rows*(KNear+1)], query.rows, KNear+1);
		flann::Matrix<double> dists(new double[query.rows*(KNear+1)],query.rows,KNear+1);
		flann::Index<flann::L2<double>> index(dataset,flann::KDTreeIndexParams(4));
		index.buildIndex();
		index.knnSearch(query,indices,dists,KNear+1,flann::SearchParams(128));
		//TODO:Save indices for all of the delay vectords
		//TODO:where we need to delete All Indices?
		for (unsigned int i=0; i<query.rows;i++){
			for (int k=0; k<KNear+1;k++){
				AllIndices[j][k]=indices[i][k];
				cout<<AllIndices[j][k]<<" ";
			}
		}
		delete[] indices.ptr();
		delete[] dists.ptr();
	}


	//	flann::Index<flann::L2<float> > index((const flann::Matrix<double>&) dataset, (const flann::IndexParams &)flann::KDTreeIndexParams(4),flann::FLANN_DIST_EUCLIDEAN);


	//flann::save_to_file(indices,"myresult","result");
	//index.knnSearch(query,indices,dists,nn,flann::SearchParams(128));

	//index.knnSearch(query,indices,dists,nn);

	delete[] dataset.ptr();
	delete[] query.ptr();


}
//TODO:Start from here
void Dimension::predict1step(){

	vector<vector<double>> *output;
	output=DV.getOutputforDim();		//has output of all the delay vectors so we need to use indices for it
	vector<double> finalError(numVariate);
	double err=0;

	for (int j=0; j<numVariate;j++){
		double sum=0;
		for (int k=0; k < NNboxHeight;k++){
			int mypoint=AllIndices[k][0];

			for (int i=1; i<KNear+1;i++){

				int kNIndice=AllIndices[k][i];
			//	cout <<mypoint<<"   "<<kNIndice<<"   ";
				sum+=abs((*output)[mypoint][j]-(*output)[kNIndice][j]);
			//	cout <<(*output)[mypoint][j]<<"   "<<(*output)[kNIndice][j]<<"   ";

			}

		}
		finalError[j]=sum/(KNear*NNboxHeight);
		err+=pow(finalError[j],2);

	}
	errRMSE=err/numVariate;

	/*for (int j=0; j< numVariate;j++){
			int sum=0;
			for (int i=0; i< KNear+1;i++){
				error[i][j]=output[mypoint][j]-output[i+1][j];
				sum+=error[i][j];
			}
			finalError[j]=sum/KNear;
			err+=pow(finalError[j],2);

		}
		RMSEError.push_back(sqrt(err/KNear));
	}*/







}
void Dimension::findminError(){
	auto min=min_element(begin(errorDV),end(errorDV));
	indexofMin=distance(begin(errorDV),min);

}
void Dimension::vectorDim(){	//Give us different set of dimension that we want to test for KDD approach
	int N=numSetDim;	//the number of different set of dimensions that we have
	int r=0, c=0;

	for (int i=0; i< numVariate;i++){
		N=N/dmax;
		while(r<numSetDim){
			for (int j=0; j<dmax;j++){
				for (int k=0;k<N;k++){
					vectordim[r][c]=j+1;

					r++;
				}

			}
		}
		c++;
		r=0;
	}
}

void DimTest(vector<vector<double>>* input,vector<int>* delay){
	Dimension dd;
	vector<int>*dim;
	dim=dd.getDim(input,delay);

	cout << "this is dim\n";
	for (auto i:*dim){
		cout <<i<< " ";
	}
	cout <<endl;


}
