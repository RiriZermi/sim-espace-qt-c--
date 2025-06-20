# Makefile pour compiler avec g++ et Qt5

# Variables #aaa
CXX = g++                            # Le compilateur C++
CXXFLAGS = -Wall -g                 # Options de compilation (affichage des warnings et debug)
LIBS = -L/mingw64/lib -lQt5Widgets -lQt5Gui -lQt5Core -lopengl32 # Librairies par défaut

# Include paths pour Qt5 (modifie selon ton installation)
INCLUDE = -I/mingw64/include \
          -I/mingw64/include/QtWidgets \
          -I/mingw64/include/QtGui \
          -I/mingw64/include/QtCore \
          -I/mingw64/include/QtOpenGL\
		  -I/mingw64/include/GL

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
