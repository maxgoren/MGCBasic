mgcbasic: ./src/mgcbasic.cpp
	g++ -O2 --std=c++14 ./src/mgcbasic.cpp -I ./src/include -o mgcbasic

clean:
	rm ./mgcbasic