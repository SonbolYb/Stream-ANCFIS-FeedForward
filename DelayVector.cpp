/*
 * DelayVector.cpp
 *
 *  Created on: Jan 28, 2016
 *      Author: sonbol
 */
#include "InputStream.h"
#include "DelayVector.h"

using namespace std;

DelayVector::DelayVector():pOrigWindow(NULL),input(NULL),duration(numVariate),delV(1),delVDim(1),dimension(NULL),delay(NULL),output(1){
	// TODO Auto-generated constructor stub

}

DelayVector::~DelayVector() {
	// TODO Auto-generated destructor stub
}

/*void DelayVector::getInput(){
	//pOrigWindow=inS.getOrigWindow();

}*/

/*******************************************************************

 *******************************************************************/

/******************************************************************/

/*******************************************************************

 *******************************************************************/

/******************************************************************/
/*void DelayVector::getDelayVector(){

}*/
std::vector<double>* DelayVector::getNewDV(vector<double>* newData, std::vector<int>* del, std::vector<int>* dim){ //newdata is a vector of size of numofvariable becuase eachtime for each variable a new data is coming

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
	return (&delVDim);




}
void DelayVector::findDur(){

	for (int i=0; i< numVariate; i++){
		duration[i]=(*delay)[i]*((*dimension)[i]-1)+1;

	}
	maxDur=*max_element(duration.begin(),duration.end());
	//cout <<endl<<"this is dur= "<<maxDur<<endl;
	size=((*input)[0]).size()-maxDur;
	if(size <0){
		size=0;
	}
	int sum=0;
	for (auto i:*dimension){
		sum+=i;
	}
	sumDim=sum;
//	cout << "dim "<<sumDim<<" size "<<size<<endl;


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
	delVDim.resize(size);
	output.resize(size);
	for(int i=0; i <size;i++){
		delVDim[i].resize(sumDim);
		output[i].resize(numVariate);
	}
}
void DelayVector::makeDV(){

	int k=0;
	int n=0;
cout <<endl<<"maxDur= "<<maxDur<<"size= "<<size<<endl;
	for (int i=maxDur; i<size+maxDur;i++){

		n=0;
		for(int l=0;l<numVariate;l++){
			for (int j=0; j< (*dimension)[l];j++){
				//cout << (*input)[l][0];
				//cout << (*input)[l][2];
				//cout <<" this"<<i-(*delay)[l]*(j-1)-1;
				delV[k][n]=(*input)[l][i-(*delay)[l]*(j)-1];

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
				delVDim[k][n]=(*input)[l][i-(*delay)[l]*(j)-1];

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

bool DelayVector::compare(double a,double b){
	return(a);
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

	//flann::Matrix<double> *dd=&dataset;


	for (int i=0; i< 10;i++){
		for (int j=0; j<4;j++){
			dataset[i][j]=(*result)[i][j];
		}
	}
	//double *v=&((*result)[0][0]);
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

		for (unsigned int i=0 ; i< query.rows;i++){
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


	//fftw_complex  *out;
	fftw_plan p;
	vector<vector<double>> * inp=inS.getOrigWindowN();
	cout << "size "<<(*inp)[1].size();
	vector<double> inp2=(*inp)[0];
	double * inp1=&(inp2[0]);
	cout <<"this is input "<< *inp1<<endl;
	//TODO: Change N based on input size
	//int N=16;
	int N;
	int n1=(*inp)[1].size();
	float n2=log2(n1);
	cout << "n1"<<n1<<"n2"<<n2<<endl;
	if(floor(n2)==n2){
		N=n1;
	}
	else{
		N=pow(2,floor(n2)+1);
	}
	cout << "this is N "<<N<<endl;
	N=n1;
	cout << "this is N "<<N<<endl;
	fftw_complex  *out=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*floor(N/2+1));//TODO: Add N/2
	p=fftw_plan_dft_r2c_1d(N,inp1,out,FFTW_ESTIMATE);
	fftw_execute(p);

	vector<double> abs(floor(N/2+1));
	vector<double> power(floor(N/2+1));

	cout<<"this is out of fourier"<<endl;
	for (int i=0; i<floor(N/2+1);i++){
		double sum=0;
		int j=0;
		for ( j=0; j<2;j++){
			sum+=pow(out[i][j],2);
			cout <<out[i][j]<< " ";
		}
		//abs[i]=(sum)/N;
		abs[i]=(sum);
	}
	cout <<endl;
	for (int i=0; i<floor(N/2+1);i++){

		cout <<abs[i]<< " ";
		//	cout <<out[i][2]<< " ";

	}


	//Find greatest power
	vector<int> index (floor(N/2+1));
	for (int i=0; i<floor(N/2+1); i++){
		index[i]=i;
	}
	sort(index.begin(),index.end(),[&abs](size_t i1,size_t i2){return abs[i1]>abs[i2]; });
	//sort(abs.begin(),abs.end(),DV.compare); //I cannot use the sort because it is frequency and order is important
	/*vector<vector<int>>::iterator row;
		vector<int>::iterator col;

		sort(out.begin(),out.end());//,DV.compare);*/
	//vector<int>::iterator result=max_element(begin(abs),end(abs));

	int numofMF=2;
	cout<<endl<<"this is greatest power"<<endl;
	//Ignore the first frequency as it is zero frequency and it is summation of all the data.
	for (int i=0;i<numofMF;i++){
		//cout << abs[15-1-i] <<" ";
		cout << index[i+1] << " ";
		cout << abs[index[i+1]]<<" ";
		//Then we can make MF by using the abs
	}


	fftw_destroy_plan(p);

	fftw_free(out);

}
