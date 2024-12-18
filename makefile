all: clean link

link: compile
	@echo 'Linking'
	g++ -o bin/simulation bin/main.o bin/Auxiliary.o bin/Settlement.o bin/Facility.o bin/SelectionPolicy.o bin/Simulation.o bin/Action.o bin/Plan.o

compile: src/main.cpp src/Auxiliary.cpp src/Facility.cpp src/Settlement.cpp src/SelectionPolicy.cpp src/Simulation.cpp src/Action.cpp src/Plan.cpp
	@echo "Compiling"
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Facility.o src/Facility.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Auxiliary.o src/Auxiliary.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Settlement.o src/Settlement.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/SelectionPolicy.o src/SelectionPolicy.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Simulation.o src/Simulation.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Plan.o src/Plan.cpp

clean: 
	@echo "Cleaning bin"
	rm -rf ./bin/*

run:
	@echo "Running"
	./bin/simulation config_file.txt