mgcbasic: ./src/mgcbasic.cpp
	g++ -O2 --std=c++14 ./src/mgcbasic.cpp -I ./src/include -o mgcbasic

install:
	cp ./mgcbasic /usr/local/bin/mgcbasic
	chmod +x /usr/local/bin/mgcbasic
clean:
	rm ./mgcbasic