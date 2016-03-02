ANCFIS_ELM:ANCFIS_ELM.o AncfisNet.o commandLine.o firingStrength.o inputVector.o inputVecWeight.o mfGrade.o timeSeries.o AncfisChecking.o
	g++ -std=c++11 -std=c++0x -g -Wall -pthread  ANCFIS_ELM.o AncfisNet.o commandLine.o firingStrength.o inputVector.o inputVecWeight.o mfGrade.o timeSeries.o AncfisChecking.o -o ANCFIS_ELM
		
ANCFIS_ELM.o:ANCFIS_ELM.cpp AncfisNet.h commandLine.h firingStrength.h header.h inputVector.h inputVecWeight.h mfGrade.h timeSeries.h AncfisChecking.h
	g++ -std=c++11 -std=c++0x -g -Wall -pthread  -c ANCFIS_ELM.cpp 

AncfisNet.o:AncfisNet.cpp header.h timeSeries.h inputVecWeight.h commandLine.cpp
	g++ -std=c++11 -std=c++0x -g -Wall -pthread -c AncfisNet.cpp 

commandLine.o: commandLine.cpp header.h
	g++ -std=c++11 -std=c++0x -g -Wall -pthread -c commandLine.cpp 

firingStrength.o:firingStrength.cpp header.h mfGrade.h commandLine.cpp
	g++ -std=c++11 -std=c++0x -g -Wall -pthread -c firingStrength.cpp
	
inputVector.o:inputVector.cpp header.h commandLine.h
	g++ -std=c++11 -std=c++0x -g -Wall -pthread -c inputVector.cpp

inputVecWeight.o:inputVecWeight.cpp header.h inputVector.h firingStrength.h commandLine.cpp
	g++ -std=c++11 -std=c++0x -g -Wall -pthread -c inputVecWeight.cpp
	
mfGrade.o:mfGrade.cpp header.h commandLine.cpp
	g++ -std=c++11 -std=c++0x -g -Wall -pthread -c mfGrade.cpp
	
timeSeries.o:timeSeries.cpp header.h commandLine.cpp
	g++ -std=c++11 -std=c++0x -g -Wall -pthread -c timeSeries.cpp
	
AncfisChecking.o: AncfisChecking.cpp header.h inputVector.h firingStrength.h commandLine.h timeSeries.h inputVecWeight.h
	g++ -std=c++11 -std=c++0x -g -Wall -pthread -c AncfisChecking.cpp
clean:
	\rm *.o ANCFIS_ELM
