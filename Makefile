TARGET = Main
CXX = g++ --std=c++11 -g
FLAGS = -Wall 
LIB =
HDRS = AggieStack.h
SRCS = AggieStack.cpp Main.cpp
OBJS = AggieStack.o Main.o

$(TARGET):  $(OBJS)
	$(CXX) $(FLAGS) $(OBJS) $(LIB) -o $(TARGET) 

$(OBJS): $(HDRS)

clean:
	rm $(OBJS) $(TARGET)
	rm -rf aggiestack-log-*
