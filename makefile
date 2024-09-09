build: 
	g++ -std=c++20 -g -Wall -Wno-deprecated-declarations -o main.out main.cpp

memory: build
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main.out

run: build
	./main.out

test:
	g++ -std=c++20 -g -Wall -Wno-deprecated-declarations -o tests.out tests.cpp -lgtest -lpthread
	./tests.out

clean:
	rm *.out

.PHONY: build memory run test clean
