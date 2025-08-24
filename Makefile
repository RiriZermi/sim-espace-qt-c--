# Makefile pour compiler avec g++ et Qt5

# Variables #aaa
CXX = g++                            # Le compilateur C++
CXXFLAGS = -Wall -g                 # Options de compilation 
LIBS = -L/mingw64/lib -lQt5Widgets -lQt5Gui -lQt5Core -lopengl32

#---------- ABS_PATH VARIABLE---------
ABS_PATH = /mingw64/include
#--------------------------------------------

INCLUDE = -I$(ABS_PATH) \
          -I$(ABS_PATH)/QtWidgets \
          -I$(ABS_PATH)/QtGui \
          -I$(ABS_PATH)/QtCore \
          -I$(ABS_PATH)/QtOpenGL \
          -I$(ABS_PATH)/GL

# Sources et objets
SOURCES := $(filter-out moc_%.cpp, $(wildcard *.cpp))
MOCSOURCES := $(patsubst %.h,moc_%.cpp,$(wildcard *.h))
OBJECTS := $(SOURCES:.cpp=.o) $(MOCSOURCES:.cpp=.o) 


# Cible par défaut
all: $(MOCSOURCES) $(OBJECTS)
	@echo "YOOOOOOOO"
	@echo $(MOCSOURCES)
	@echo $(OBJECTS)
	$(CXX) -o main $(OBJECTS)  $(LIBS) 
	./main

# Règle pour compiler les .cpp en .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

moc_%.cpp: %.h
	moc $< -o $@


clean:
	rm -rf *.o main.exe moc*
