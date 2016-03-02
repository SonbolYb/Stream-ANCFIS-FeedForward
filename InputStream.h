/*
 * InputStream.h
 *
 *  Created on: Jan 20, 2016
 *      Author: sonbol
 */

#include "header.h"
#ifndef INPUTSTREAM_H_
#define INPUTSTREAM_H_

class InputStream: public commandLine{
public:
	InputStream();
	virtual ~InputStream();

	std::vector<std::vector<double>> * getOrigWindowN();

	std::vector<std::vector<double>> * getOrigWindow();
	std::vector<std::vector<double>> *  getSurWindow();
	std::vector<std::vector<double>> *  getSurWindowN();
	//TODO: write this two methods
	std::vector<double>* getNewData();
	std::vector<double>* getNewDataN();


	std::vector<double> origWindowS;	//For univariate case
	std::vector<double> surWindowS;
	std::vector<std::vector<double>> origWindow;
	std::vector<std::vector<double>> origWindowN;
	std::vector<std::vector<double>> surWindow;

	std::vector<std::vector<double>> surWindowN;
	std::vector<double> newdata;
	std::vector<double> newdataN;
	int numpassedInput=0;

private:
	void readOrigWindow();		//read the first 10% of data
	void readNewData();
	void readSurData();			//Reading surogate data
	void endtoend();
	void readNewDataN();
	void readSurDataN();
	void readOrigWindowN();
	int nless(int);
	double jump(int nmax, int nmaxp, int nx,  std::vector<std::vector<double>> x, int mcmax,  double wjump, double * pejump,double * peslip, double *pnjump);
	double xjump(int, std::vector<double>);
	double sjump(int, std::vector<double>);
	void normalize(const std::vector<std::vector<double>> &,std::vector<std::vector<double>>&);
	int endtoendLength=0;
	std::vector<double> max;
	std::vector<double> min;
	std::fstream& GotoLine(std::fstream& file, unsigned int num);
	//int per10=35;

	//int numVar=1;

	friend void testInputStream();

};
void testInputStream();

#endif /* INPUTSTREAM_H_ */
