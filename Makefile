CC = g++ -std=c++11

all : gen_graph.cpp algorithms.cpp heapAlgos.cpp mufAlgos.cpp 
	$(CC) -o final_executable gen_graph.cpp algorithms.cpp heapAlgos.cpp mufAlgos.cpp

clean : 
		rm -rf final_executable