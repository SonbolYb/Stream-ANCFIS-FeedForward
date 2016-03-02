/*
 * Delay.cpp
 *
 *  Created on: Jan 28, 2016
 *      Author: sonbol
 */

#include "Delay.h"
using namespace std;
Delay::Delay():commandLine(),delays(numVariate),mutualArray(numVariate, vector<double>(corrlength)),array(NULL),h1(NULL),h11(NULL),h2(NULL),pOrigWindow(NULL) {
	// TODO Auto-generated constructor stub


}

Delay::~Delay() {
	// TODO Auto-generated destructor stub
}



/*******************************************************************

 *******************************************************************/
double Delay::make_cond_entropy(long t){
	long i,j,hi,hii,count=0;
	double hpi,hpj,pij,cond_ent=0.0,norm;

	for (i=0;i<partitions;i++) {
		h1[i]=h11[i]=0;
		for (j=0;j<partitions;j++)
			h2[i][j]=0;
	}
	for (unsigned i=0;i<length;i++)
		if (i >= t) {
			hii=array[i];
			hi=array[i-t];
			h1[hi]++;
			h11[hii]++;
			h2[hi][hii]++;
			count++;
		}

	norm=1.0/(double)count;
	cond_ent=0.0;

	for (i=0;i<partitions;i++) {
		hpi=(double)(h1[i])*norm;
		if (hpi > 0.0) {
			for (j=0;j<partitions;j++) {
				hpj=(double)(h11[j])*norm;
				if (hpj > 0.0) {
					pij=(double)h2[i][j]*norm;
					if (pij > 0.0)
						cond_ent += pij*log(pij/hpj/hpi);
				}
			}
		}
	}

	return cond_ent;
}
/******************************************************************/

/*******************************************************************

 *******************************************************************/
void Delay::mutual(){ 	//TODO: work with one variate but at the end make it for multivariat. In multivariate, we need to separate variate and consider them separately

	for(int j=0; j< numVariate;j++){

	double *series=&(((*pOrigWindow)[j])[0]);
	long tau,i;
	  double min,interval,shannon;
	  FILE *file;

	//  double *series=&(origw1[0]);
	  rescale_data(series, length, &min, &interval);
	  check_alloc(h1=(long *)malloc(sizeof(long)*partitions));
	    check_alloc(h11=(long *)malloc(sizeof(long)*partitions));
	    check_alloc(h2=(long **)malloc(sizeof(long *)*partitions));
	    for (i=0;i<partitions;i++)
	      check_alloc(h2[i]=(long *)malloc(sizeof(long)*partitions));
	    check_alloc(array=(long *)malloc(sizeof(long)*length));
	    for (unsigned i=0;i<length;i++)
	      if (series[i] < 1.0)
	        array[i]=(long)(series[i]*(double)partitions);
	      else
	        array[i]=partitions-1;
	    free(series);
	    shannon=make_cond_entropy(0);
	      if ( (unsigned)corrlength >= length)
	        corrlength=length-1;
	      file=fopen("mutual","w");
	      fprintf(file,"#shannon= %e\n",shannon);
	          fprintf(file,"%d %e\n",0,shannon);
	          mutualArray[j][0]=shannon;
	          for (tau=1;tau<=corrlength;tau++) {
	        	  mutualArray[j][tau]=make_cond_entropy(tau);
	            fprintf(file,"%ld %e\n",tau,make_cond_entropy(tau));
	            fflush(file);
	          }
	          fclose(file);
	}
	/*for (int j =0 ; j <numVariate;j++){
		for (int i=0 ; i <20; i++){
			cout << mutualArray[j][i] <<" ";
		}
	}*/

}
/******************************************************************/
/*******************************************************************

 *******************************************************************/
void Delay::findDelay(){

	for (int j=0 ; j < numVariate; j++){
	double min=mutualArray[j][0];
	for(int i=1; i<corrlength;i++){
		if(mutualArray[j][i] <min){
			min=mutualArray[j][i];
		}
		if(mutualArray[j][i] >min){
			min=mutualArray[j][i-1];
			delays[j]=i-1;
			//cout <<endl <<"min" <<min <<endl;
			break;
		}
	}
	}

}
/******************************************************************/
/*******************************************************************

 *******************************************************************/
void Delay::calDelay(){
	//getInput();
	mutual();
	findDelay();
}
/******************************************************************/
/*******************************************************************

 *******************************************************************/
vector<int>*  Delay::getDelay(vector<vector<double>>* inp){
	pOrigWindow=inp;
	calDelay();
	return(&delays);
}
/******************************************************************/

/*******************************************************************

 *******************************************************************/
void testDelay(){
	Delay dl;
	//dl.calDelay();
InputStream inS;
//dl.getDelay(inS.getOrigWindow());
	for (auto i:*(dl.getDelay(inS.getOrigWindowN()))){
			cout << "delays" <<i << endl;
		}

}
/******************************************************************/

/*******************************************************************

 *******************************************************************/

/******************************************************************/
