CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -pthread
TARGET = raytracer
SRCS = main.cpp
HEADERS = wicked.h vec3.h ray.h color.h interval.h hittable.h \
          material.h sphere.h quad.h triangle.h camera.h \
          hittable_list.h bvh.h aabb.h texture.h perlin.h
OUTPUT = output.ppm


all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)


render: $(TARGET)
	./$(TARGET) > $(OUTPUT)
	@echo "Rendering complete! Output saved to $(OUTPUT)"


clean:
	rm -f $(TARGET) $(OUTPUT)


view: $(OUTPUT)
	@echo "Opening $(OUTPUT)..."
	@if command -v display > /dev/null; then \
		display $(OUTPUT); \
	elif command -v open > /dev/null; then \
		open $(OUTPUT); \
	elif command -v xdg-open > /dev/null; then \
		xdg-open $(OUTPUT); \
	else \
		echo "No suitable image viewer found. Please open $(OUTPUT) manually."; \
	fi

.PHONY: all render clean view help