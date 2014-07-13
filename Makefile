CC=g++
CFLAGS=-c -std=c++11 -Wall
LDFLAGS=
SOURCES_SCHEDULER=$(wildcard Scheduler/*.cpp)
SOURCES_SIMULATOR=$(wildcard InputSimulator/*.cpp)
OBJECTS_SCHEDULER=$(SOURCES_SCHEDULER:.cpp=.o)
OBJECTS_SIMULATOR=$(SOURCES_SIMULATOR:.cpp=.o)
EXECUTABLE=SchedulerBin InputSimulatorBin

all: $(EXECUTABLE)

InputSimulatorBin: $(OBJECTS_SIMULATOR) 
	$(CC) $(LDFLAGS) $(OBJECTS_SIMULATOR) -o $@

SchedulerBin: $(OBJECTS_SCHEDULER) 
	$(CC) $(LDFLAGS) $(OBJECTS_SCHEDULER) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf Scheduler/*.o InputSimulator/*.o $(EXECUTABLE)