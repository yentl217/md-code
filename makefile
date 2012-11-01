CXX = g++
CXXFLAGS = -march=native -std=c++0x -mfpmath=sse -O3

BIN_DIR = ./bin
OBJ_DIR = ./obj

objects = $(OBJ_DIR)/acceleration.o $(OBJ_DIR)/analysis.o $(OBJ_DIR)/atom.o $(OBJ_DIR)/file_io.o $(OBJ_DIR)/integration_methods.o $(OBJ_DIR)/main.o $(OBJ_DIR)/potentials.o $(OBJ_DIR)/settings.o $(OBJ_DIR)/vectron.o $(OBJ_DIR)/thermostat.o

all: install $(BIN_DIR)/mmm

$(BIN_DIR)/mmm: $(objects)
	$(CXX) -o $(BIN_DIR)/mmm $(objects) $(CXXFLAGS)

$(OBJ_DIR)/acceleration.o: acceleration.cpp acceleration.h atom.h vectron.h potentials.h settings.h
	$(CXX) -c acceleration.cpp -o $(OBJ_DIR)/acceleration.o $(CXXFLAGS)

$(OBJ_DIR)/analysis.o: analysis.cpp analysis.h atom.h vectron.h settings.h potentials.h
	$(CXX) -c analysis.cpp -o $(OBJ_DIR)/analysis.o $(CXXFLAGS)

$(OBJ_DIR)/atom.o: atom.cpp atom.h vectron.h
	$(CXX) -c atom.cpp -o $(OBJ_DIR)/atom.o $(CXXFLAGS)

$(OBJ_DIR)/file_io.o: file_io.cpp analysis.h atom.h file_io.h settings.h vectron.h
	$(CXX) -c file_io.cpp -o $(OBJ_DIR)/file_io.o $(CXXFLAGS)

$(OBJ_DIR)/integration_methods.o: integration_methods.cpp integration_methods.h atom.h vectron.h acceleration.h potentials.h settings.h analysis.h thermostat.h
	$(CXX) -c integration_methods.cpp -o $(OBJ_DIR)/integration_methods.o $(CXXFLAGS)

$(OBJ_DIR)/main.o: main.cpp acceleration.h analysis.h atom.h file_io.h integration_methods.h settings.h vectron.h thermostat.h
	$(CXX) -c main.cpp -o $(OBJ_DIR)/main.o $(CXXFLAGS)

$(OBJ_DIR)/potentials.o: potentials.cpp potentials.h settings.h vectron.h
	$(CXX) -c potentials.cpp -o $(OBJ_DIR)/potentials.o $(CXXFLAGS)

$(OBJ_DIR)/settings.o: settings.cpp file_io.h settings.h
	$(CXX) -c settings.cpp -o $(OBJ_DIR)/settings.o $(CXXFLAGS)

$(OBJ_DIR)/vectron.o: vectron.cpp vectron.h
	$(CXX) -c vectron.cpp -o $(OBJ_DIR)/vectron.o $(CXXFLAGS)
	
$(OBJ_DIR)/thermostat.o: thermostat.cpp thermostat.h atom.h vectron.h
	$(CXX) -c thermostat.cpp -o $(OBJ_DIR)/thermostat.o $(CXXFLAGS)

install:
	@test -d $(BIN_DIR) || mkdir $(BIN_DIR)
	@test -d $(OBJ_DIR) || mkdir $(OBJ_DIR)

clean:
	@rm -f $(BIN_DIR)/mmm
	@rm -f $(OBJ_DIR)/*.o
