ANCFIS_ELM: ANCFIS_ELM.o commandLine.o firingStrength.o inputVector.o inputVecWeight.o mfGrade.o BuildNet.o BuildANCFIS.o Delay.o Dimension.o DelayVector.o InputStream.o MFParam.o AncfisChecking.o check_alloc.o rescale_data.o
	g++   -std=c++0x -std=gnu++11 -g  -Wall -pthread -I/home/sonbol/flann-1.8.4-src/src/cpp    ANCFIS_ELM.o commandLine.o firingStrength.o inputVector.o inputVecWeight.o mfGrade.o BuildNet.o BuildANCFIS.o Delay.o Dimension.o DelayVector.o InputStream.o MFParam.o AncfisChecking.o check_alloc.o rescale_data.o -o ANCFIS_ELM -lfftw3 -lm -fopenmp
	
ANCFIS_ELM.o: ANCFIS_ELM.cpp Delay.h Dimension.h InputStream.h firingStrength.h header.h inputVector.h inputVecWeight.h mfGrade.h InputStream.h AncfisChecking.h DelayVector.h MFParam.h BuildNet.h BuildANCFIS.h
	g++ -std=c++11 -std=c++0x -std=gnu++11 -g -Wall -pthread -I/home/sonbol/flann-1.8.4-src/src/cpp     -c ANCFIS_ELM.cpp 

BuildNet.o: BuildNet.cpp header.h inputVecWeight.h
	g++ -std=c++11 -g -std=c++0x -std=gnu++11 -Wall -pthread -c BuildNet.cpp 

Delay.o: Delay.cpp header.h
	g++  -std=c++11 -g -std=c++0x -std=gnu++11 -Wall -pthread  -c Delay.cpp 

DelayVector.o: DelayVector.cpp header.h 
	g++ -std=c++11 -g -std=c++0x -std=gnu++11 -Wall -pthread -I/home/sonbol/flann-1.8.4-src/src/cpp  -c DelayVector.cpp

Dimension.o: Dimension.cpp header.h DelayVector.h 
	g++ -std=c++11 -g -std=c++0x -std=gnu++11 -Wall -pthread -I/home/sonbol/flann-1.8.4-src/src/cpp -c Dimension.cpp

commandLine.o: commandLine.cpp header.h
	g++ -std=c++11 -g -std=c++0x -std=gnu++11 -Wall -pthread -c commandLine.cpp 

firingStrength.o: firingStrength.cpp header.h mfGrade.h 
	g++ -std=c++11 -g -std=c++0x -std=gnu++11 -Wall -pthread -c firingStrength.cpp

InputStream.o: InputStream.cpp header.h 
	g++ -std=c++11 -g -std=c++0x -std=gnu++11 -Wall -pthread -c InputStream.cpp

inputVector.o: inputVector.cpp header.h 
	g++ -std=c++11 -g -std=c++0x -std=gnu++11 -Wall -pthread -c inputVector.cpp

inputVecWeight.o: inputVecWeight.cpp header.h inputVector.h firingStrength.h 
	g++ -std=c++11 -g -std=c++0x -std=gnu++11 -Wall -pthread -c inputVecWeight.cpp

mfGrade.o: mfGrade.cpp header.h 
	g++ -std=c++11 -g -std=c++0x -std=gnu++11 -Wall -pthread -c mfGrade.cpp

MFParam.o: MFParam.cpp header.h InputStream.h
	g++ -std=c++11 -g -std=c++0x -std=gnu++11 -Wall -pthread -c MFParam.cpp

AncfisChecking.o: AncfisChecking.cpp header.h inputVector.h firingStrength.h InputStream.h inputVecWeight.h DelayVector.h
	g++ -std=c++11 -g -std=c++0x -std=gnu++11 -Wall -pthread -I/home/sonbol/flann-1.8.4-src/src/cpp -c AncfisChecking.cpp
	
BuildANCFIS.o: BuildANCFIS.cpp header.h BuildNet.h MFParam.h DelayVector.h Delay.h Dimension.h InputStream.h 
	g++ -std=c++11 -g -std=c++0x -std=gnu++11 -Wall -pthread -I/home/sonbol/flann-1.8.4-src/src/cpp -c BuildANCFIS.cpp 

clean:
	\rm *.o ANCFIS_ELM
