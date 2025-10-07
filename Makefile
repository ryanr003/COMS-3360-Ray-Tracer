CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra
TARGET = raytracer
SOURCES = main.cpp material.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) output.ppm

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run