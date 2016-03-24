/*
 * firingStrength.h
 *
 *  Created on: Jun 19, 2015
 *      Author: sonbol
 */

#ifndef FIRINGSTRENGTH_H_
#define FIRINGSTRENGTH_H_

#include "header.h"
#include "mfGrade.h"
//#include"functions.h"

class firingStrength: public commandLine {
private:
	mfGrade mf;
	//std::vector<double> fS;
	std::unique_ptr<std::vector<double>> pFS;

	//std::vector<std::shared_ptr<std::vector<std::complex<double> >>> mfG;
	std::vector<std::unique_ptr<std::vector<std::complex<double> >>> mfG;
	std::shared_ptr<std::vector<std::vector<int>>>  rule;
	std::vector<std::complex<double>> fsRaw;
	std::vector<std::complex<double>> fsNorm;
	double SumNorm;
	std::complex<double> SumDP;
	std::vector<double> pFSDP;
	void getRawFS();
	std::complex<double> conjunctionOp(std::complex<double>,std::complex<double>);
	void dotP();
	void normalize();
	double dotOper(std::complex<double> a,std::complex<double> b);
	void getFinalFs(std::vector<double> * const);
	void conseq(std::vector<double> * const);

	public:
	firingStrength();

	std::unique_ptr<std::vector<double>> cal_firingStrenght(std::vector<double> * const,int,std::vector<std::vector<std::vector<double>>>*,std::vector<int>*,int);
	friend void FS_test(firingStrength &);
};
void FS_test(firingStrength &);


#endif /* FIRINGSTRENGTH_H_ */
