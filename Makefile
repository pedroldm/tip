# Compiler
CXX = g++

# Compiler flags for development build
CXXFLAGS = -Wall -Wextra -std=c++17 -O2 -I src/brkgaAPI

# Compiler flags for debug build
CXXFLAGS_DEBUG = -Wall -Wextra -std=c++17 -g -I src/brkgaAPI

# Compiler flags for production build
CXXFLAGS_PRD = -std=c++17 -O3 -march=native -I src/brkgaAPI

# Source files
SRCS = src/IOReport.cpp src/main.cpp src/SampleDecoder.cpp src/brkgaAPI/Population.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)
OBJS_DEBUG = $(SRCS:.cpp=.o.debug)
OBJS_PRD = $(SRCS:.cpp=.o.prd)

# Header files
HEADERS = src/brkgaAPI/BRKGA.hpp src/brkgaAPI/MTRand.hpp src/brkgaAPI/Population.hpp src/SampleDecoder.hpp src/IOReport.hpp src/json.hpp

# Target executable
TARGET = main
TARGET_DEBUG = main_debug
TARGET_PRD = main_prd

# Default target (development build)
dev: $(TARGET)

# Debug target
debug: $(TARGET_DEBUG)

# Production target
prd: $(TARGET_PRD)

# Link the object files to create the development executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Link the object files to create the debug executable
$(TARGET_DEBUG): $(OBJS_DEBUG)
	$(CXX) $(CXXFLAGS_DEBUG) -o $(TARGET_DEBUG) $(OBJS_DEBUG)

# Link the object files to create the production executable
$(TARGET_PRD): $(OBJS_PRD)
	$(CXX) $(CXXFLAGS_PRD) -o $(TARGET_PRD) $(OBJS_PRD)

# Compile source files into development object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile source files into debug object files
%.o.debug: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS_DEBUG) -c $< -o $@

# Compile source files into production object files
%.o.prd: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS_PRD) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(OBJS_DEBUG) $(OBJS_PRD) $(TARGET) $(TARGET_DEBUG) $(TARGET_PRD)

# Phony targets
.PHONY: dev debug prd clean