all: clean link

link: compile
	@echo 'Linking'
	g++ -o bin/simulation bin/main.o bin/Auxiliary.o

compile: src/main.cpp src/Auxiliary.cpp
	@echo "Compiling"
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Auxiliary.o src/Auxiliary.cpp

clean: 
	@echo "Cleaning bin"
	rm -rf ./bin/*

run:
	@echo "Running"
	./bin/simulation config_file.txt