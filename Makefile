CXX = g++
CXXFLAGS = -std=c++17 -municode -Wall -O2
TARGET = delivery.exe
OBJS = MinHeap.o Solver.o main.o

.PHONY: all clean run-example run-test1 run-test2 run-test3

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

MinHeap.o: MinHeap.cpp MinHeap.h DataDef.h
	$(CXX) $(CXXFLAGS) -c MinHeap.cpp

Solver.o: Solver.cpp Solver.h DataDef.h MinHeap.h CircularQueue.h
	$(CXX) $(CXXFLAGS) -c Solver.cpp

main.o: main.cpp Solver.h DataDef.h
	$(CXX) $(CXXFLAGS) -c main.cpp

clean:
	del /f *.o $(TARGET) 2>nul || rm -f *.o $(TARGET)

run-example: $(TARGET)
	./$(TARGET) "测试数据/示例"

run-test1: $(TARGET)
	./$(TARGET) "测试数据/测试1"

run-test2: $(TARGET)
	./$(TARGET) "测试数据/测试2"

run-test3: $(TARGET)
	./$(TARGET) "测试数据/测试3"
