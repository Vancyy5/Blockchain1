CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -finput-charset=UTF-8 -fexec-charset=UTF-8
TARGET = main

SOURCES = main.cpp funkcijos.cpp failugeneravimas.cpp laikas.cpp testavimas.cpp visi.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: main.cpp funkcijos.h failugeneravimas.h laikas.h testavimas.h visi.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

funkcijos.o: funkcijos.cpp funkcijos.h
	$(CXX) $(CXXFLAGS) -c funkcijos.cpp -o funkcijos.o

failugeneravimas.o: failugeneravimas.cpp failugeneravimas.h
	$(CXX) $(CXXFLAGS) -c failugeneravimas.cpp -o failugeneravimas.o

laikas.o: laikas.cpp laikas.h
	$(CXX) $(CXXFLAGS) -c laikas.cpp -o laikas.o

testavimas.o: testavimas.cpp testavimas.h
	$(CXX) $(CXXFLAGS) -c testavimas.cpp -o testavimas.o

visi.o: visi.cpp visi.h
	$(CXX) $(CXXFLAGS) -c visi.cpp -o visi.o

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean run