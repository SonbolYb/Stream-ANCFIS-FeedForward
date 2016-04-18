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
	std::vector<std::vector<double>> *  getSurWindow(std::vector<std::vector<double>> *);
	//std::vector<std::vector<double>> *  getSurWindowN(std::vector<std::vector<double>> *); //Do not need this one any more because we get input argument
	//TODO: write this two methods
	std::vector<double>* getNewData();
	std::vector<double>* getNewDataN();
	std::vector<std::vector<double>> * moveWindowbyOneNormal();
	std::vector<std::vector<double>> * moveWindowbyOne();
	std::vector<int> * getHeadEndInx();
	bool EndofFile();
	std::vector<std::vector<double>> * getCheckingN();
	std::vector<std::vector<double>> * getChecking();
	int getNumpassedWindow();

	std::vector<std::vector<double>> origWindow;
	std::vector<std::vector<double>> origWindowN;
	std::vector<std::vector<double>> surWindow;

	std::vector<std::vector<double>> surWindowN;
	std::vector<double> newdata;
	std::vector<double> newdataN;
	int numpassedInput=0;
	bool endoffile=false;
	std::vector<std::vector<double>> checking;
	std::vector<std::vector<double>> checkingN;

private:
	void readOrigWindow();		//read the first 10% of data
	void readNewData();
	void readSurData();			//Reading surogate data
	void endtoend();
	//void readSurDataN();
	void readOrigWindowN();
	int nless(int);
	double jump(int nmax, int nmaxp, int nx,  std::vector<std::vector<double>> x, int mcmax,  double wjump, double * pejump,double * peslip, double *pnjump);
	double xjump(int, std::vector<double>);
	double sjump(int, std::vector<double>);
	void normalize(const std::vector<std::vector<double>> &,std::vector<std::vector<double>>&);
	int endtoendLength=0;

	std::vector<double> max;
	std::vector<double> min;

	double * head;
	double* end;
	int headInx=0;
	int endInx=0;
	std::vector<int> headEndInx;
	std::fstream& GotoLine(std::fstream& file, unsigned int num);
	std::vector<std::vector<double>> * inputWindow;
	//int per10=35;

	//int numVar=1;

	friend void testInputStream();

};
void testInputStream();

#endif /* INPUTSTREAM_H_ */
