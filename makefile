CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -finput-charset=UTF-8 -fexec-charset=UTF-8
TARGET = main
SOURCES = main.cpp funkcijos.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

.PHONY: clean