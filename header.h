/*
 * header.h
 *
 *  Created on: Apr 17, 2015
 *      Author: sonbol
 */

#ifndef HEADER_H_
#define HEADER_H_
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <map>
#include <unordered_map>
#include <sstream>
#include <memory>
#include <iterator>
#include <fstream>
#include <complex>
#include <random>
#include<cfloat>
#include<cmath>
#include<algorithm>
#include<chrono>
#include "commandLine.h"
#include <memory.h>
#include <thread>
#include <thread>
#include <mutex>
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
 #include <fftw3.h>

/*
#include "commandLine.h"
#include "Delay.h"
#include "Dimension.h"
#include "InputStream.h"
#include "mfGrade.h"
#include "timeSeries.h"
#include"inputVector.h"
#include"AncfisNet.h"
#include"firingStrength.h"
#include"AncfisChecking.h"
#include "InputStream.h"
#include"DelayVector.h"

#include "MFParam.h"
#include "GetDelayDImension.h"
#include "BuildNet.h"
#include "BuildANCFIS.h"
*/

extern "C"{
	void check_alloc(void *pnt);
	void rescale_data(double *x, unsigned long l, double *min, double *interval);
}




//const double PI= 3.1415926535897932385E0;
const double PI= atan(1.0)*4;

//const char *TrainFile="datatrn.txt";
//const char *CheckFile="datachk.txt";



#endif /* HEADER_H_ */
